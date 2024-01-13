/*
** VUT FIT 2023 - ISA
** DNS resolver
** Marcin Sochacki
** xsocha03
*/
#include <stdio.h>	
#include <string.h>	
#include <stdlib.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <netinet/in.h>
#include <unistd.h>	
#include <stdbool.h>
#include <netdb.h>
#include <signal.h>
#include "dns.hpp"

int socketDes; 

void sigintHandler(int) {
    close(socketDes);
    exit(0);
}

DNS_OPTIONS parseCommandLine(int argc, char* argv[]) {
    DNS_OPTIONS options;
    options.recursionDesired = false;
    options.reverseQuery = false;
    options.useAAAA = false;
    strcpy((char*)options.server, "");
    options.port = 53;
    strcpy((char*)options.address, "");

    for (int i = 1; i < argc; ++i) {
        unsigned char* arg = (unsigned char*)argv[i];

        // Zpracování různých argumentů
        if (strcmp((const char*)arg, "-r") == 0) {
            // Možnost Recursion Desired
            if(options.recursionDesired){
                fprintf(stderr, "Nepodporuje se více než jednu možnost '-r'\n");
                exit(1);
            }
            options.recursionDesired = true;
        } else if (strcmp((const char*)arg, "-x") == 0) {
            // Možnost Reverse Query
            if(options.reverseQuery){
                fprintf(stderr, "Nepodporuje se více než jednu možnost '-x'\n");
                exit(1);
            }
            options.reverseQuery = true;
        } else if (strcmp((const char*)arg, "-6") == 0) {
            // Možnost AAAA
            if(options.useAAAA){
                fprintf(stderr, "Nepodporuje se více než jednu možnost '-6'\n");
                exit(1);
            }
            options.useAAAA = true;
        } else if (strcmp((const char*)arg, "-s") == 0) {
            // Možnost Server
            if(i + 1 == argc){
                fprintf(stderr, "Chybějící server nebo dotazovaná adresa.\n");
                exit(1);
            }
            if(strlen((const char*)options.server) != 0){
                fprintf(stderr, "Nepodporuje se více než jednu možnost '-s'\n");
                exit(1);
            }
            // Analýza a ověření adresy serveru
            unsigned char* serverArg = (unsigned char*)argv[++i];
            struct sockaddr_in sa;
            struct in6_addr sa6;

            if (inet_pton(AF_INET, (const char*)serverArg, &(sa.sin_addr)) == 1) {
                // Je to IPv4 adresa
                unsigned int ipParts[4];
                if (sscanf((const char*)serverArg, "%u.%u.%u.%u", &ipParts[0], &ipParts[1], &ipParts[2], &ipParts[3]) == 4) {
                    strcpy((char*)options.server, (const char*)serverArg);
                } else {
                    fprintf(stderr, "Neplatná IPv4 adresa serveru: %s\n", (const char*)serverArg);
                    exit(1);
                }
            } else if(inet_pton(AF_INET6, (const char*)serverArg, &(sa6)) == 1){
                // Je to IPv6 adresa
                fprintf(stderr, "Aplikace nepodporuje IPv6 adresu jako adresu serveru: %s\n", (const char*)serverArg);
                exit(1);
            } else {
                // Je to doménové jméno
                if (strlen((const char*)serverArg) > 100) {
                    fprintf(stderr, "Délka serveru přesahuje maximální povolenou délku (100 znaků).\n");
                    exit(1);
                }
                struct hostent* param = gethostbyname((const char*)serverArg);
                if (param) {
                    struct in_addr* address = (struct in_addr*)param->h_addr;
                    strcpy((char*)options.server, inet_ntoa(*address));
                } else {
                    fprintf(stderr, "Chyba při rozpoznávání serveru: %s\n", (const char*)serverArg);
                    exit(1);
                }
            }
        } else if (strcmp((const char*)arg, "-p") == 0) {
            // Možnost Port
            if(i +1 == argc){
                fprintf(stderr, "Musíte zadat port.\n");
                exit(1);
            }
            options.port = atoi(argv[++i]);
        } else if (strlen((const char*)options.address) == 0) {
            // Adresa dotazu
            if (strlen((const char*)arg) > 100) {
                fprintf(stderr, "Délka dotazované adresy přesahuje maximální povolenou délku (100 znaků).\n");
                exit(1);
            }
            struct in6_addr sa6;
            if(inet_pton(AF_INET6, (const char*)arg, &(sa6)) == 1){
                // Je to IPv6 adresa
                fprintf(stderr, "Aplikace nepodporuje IPv6 adresu jako dotazovanou adresu: %s\n", (const char*)arg);
                exit(1);
            }
            strcpy((char*)options.address, (const char*)arg);
        } else {
            // Neplatný argument
            fprintf(stderr, "Neplatný argument: %s\n", arg);
            exit(1);
        }
    }
    
    // Ověření konfliktních možností
    if(options.useAAAA && options.reverseQuery){
        fprintf(stderr, "Možnosti -6 a -x vzájemně nejsou podporovane. \n");
        exit(1);
    }

    // Ověření jesli máme server a adresu
    if (strlen((const char*)options.address) == 0 || strlen((const char*)options.server) == 0) {
        fprintf(stderr, "Chybějící server nebo dotazovaná adresa.\n");
        exit(1);
    }

    // Ověření rozsahu portu
    if(options.port < 1 || options.port > 65535){
        fprintf(stderr, "Port musí být mezi 1 a 65535.\n");
        exit(1);
    }

    // Vrátí zpracované možnosti
    return options;
}


void initDNSHeader(struct DNS_HEADER* dns, bool recursionDesired) {
    dns->id = (unsigned short)htons(getpid());
    dns->qr = 0;               
    dns->opcode = 0;               
    dns->aa = 0;                    
    dns->tc = 0;                    
    dns->rd = recursionDesired ? 1 : 0; // Žádána rekurze: Ano/Ne
    dns->ra = 0;                    
    dns->z = 0;                    
    dns->rcode = 0;                 
    dns->qdcount = htons(1); // Počet otázek: 1
    dns->ancount = 0;               
    dns->nscount = 0;              
    dns->arcount = 0;               
}

u_char* extractDomainName(unsigned char* reader, unsigned char* buffer, int* labelCount) {
    unsigned char label[256]; // Buffer pro ukládání části doménového jména
    unsigned int index = 0; // Index do bufferu s aktuální částí jména
    int offset = 0; // Offset při skoku
    bool jumped = false; // Kontrola jestli došlo ke skoku
    size_t len = 0; // Celková délka jména
    int i;

    *labelCount = 1; 
    label[0] = '\0';

    // Cyklus pro čtení doménového jména
    while (*reader != 0) {
        if (*reader >= 192) {
            // Skok 
            offset = (*reader) * 256 + *(reader + 1) - 49152;
            reader = buffer + offset - 1;
            jumped = true;
        } else {
            // Čtení 
            label[index++] = *reader;
        }

        reader = reader + 1;

        if (!jumped) {
            // Pokud nenastal skok, zvyšujeme počet etiket
            *labelCount = *labelCount + 1;
        }

        if (index >= sizeof(label)) {
            // kontrola overflowu
            break;
        }
    }

    label[index] = '\0'; // Ukončení bufferu s aktuální částí jména

    if (jumped) {
        *labelCount = *labelCount + 1; // Pokud nastal skok, zvyšujeme počet etiket
    }

    char formatted[256]; // Buffer pro formátované jméno
    int formattedIndex = 0;

    // odstranění délky částí a přidání teček
    while (len < index) {
        int currentLen = label[len];
        len++;
        for (i = 0; i < currentLen && len < index; i++) {
            formatted[formattedIndex] = label[len];
            formattedIndex++;
            len++;
        }
        if (len < index && label[len] != 0) {
            formatted[formattedIndex] = '.';
            formattedIndex++;
        }
    }

    formatted[formattedIndex] = '\0';

    char* result = strdup(formatted); // Alokace paměti pro formátované jméno
    if (result == NULL) {
        fprintf(stderr, "Chyba domenového jména\n");
        exit(1);
    }
    strcat((char*)result, "."); // Přidání tečky na konec jména (standard v DNS)
    return (u_char*)result; // Vracíme výsledek jako u_char*
}


void printQuestionSection(struct QUESTION* qinfo, const char* address, struct DNS_HEADER * dns) {
    printf("Question section (%d)\n", ntohs(dns->qdcount));
    for (int i = 0; i < ntohs(dns->qdcount); i++)
    {
        const char* recordType = NULL;
        // Rozpozná typ otázky podle kódu v qtype
        if (qinfo->qtype == htons(T_A)) {
            recordType = "A";
        } else if (qinfo->qtype == htons(T_AAAA)) {
            recordType = "AAAA";
        } else if (qinfo->qtype == htons(T_PTR)) {
            recordType = "PTR";
        } else {
            recordType = "Unknown Type";
        }

        printf("  %s, %s, %s\n", address, recordType, qinfo->qclass == htons(1) ? "IN" : "Other");
    }
}

void printData(struct RES_RECORD* answers, struct RES_RECORD* authority, struct RES_RECORD* additional, unsigned char* buffer, int stop, unsigned char* reader, struct DNS_HEADER * dns) {
    struct sockaddr_in a;
    unsigned int ttl_value;

    printf("Answer section (%d)\n", ntohs(dns->ancount));

    for (int i = 0; i < ntohs(dns->ancount); i++) {
        // Extrahuje doménové jméno odpovědi
        answers[i].name = extractDomainName(reader, buffer, &stop);
        reader = reader + stop;

        // Extrahuje obsah odpovědi a posune ukazatel na další pozici
        answers[i].content = (struct R_DATA *)(reader);
        reader = reader + sizeof(struct R_DATA);

        // Podle typu odpovědi zpracuje a vytiskne informace
        if (ntohs(answers[i].content->type) == T_A) {
            answers[i].rdata = (unsigned char *)malloc(ntohs(answers[i].content->rdlength) + 1);
            for (int j = 0; j < ntohs(answers[i].content->rdlength); j++) {
                answers[i].rdata[j] = reader[j];
            }
            answers[i].rdata[ntohs(answers[i].content->rdlength)] = '\0';
            reader += sizeof(unsigned int);
            ttl_value = ntohl(answers[i].content->ttl);

            printf("  %s, A, IN, %u, %s\n", answers[i].name, ttl_value, inet_ntoa(*(struct in_addr *)answers[i].rdata));
        } else if (ntohs(answers[i].content->type) == T_CNAME) {
            answers[i].rdata = extractDomainName(reader, buffer, &stop);
            reader = reader + stop;
            ttl_value = ntohl(answers[i].content->ttl);

            printf("  %s, CNAME, IN, %d, %s\n", answers[i].name, ttl_value, answers[i].rdata);
        } else if (ntohs(answers[i].content->type) == T_AAAA) {
            char ipv6_address[INET6_ADDRSTRLEN];
            answers[i].rdata = (unsigned char *)malloc(ntohs(answers[i].content->rdlength) + 1);
            for (int j = 0; j < ntohs(answers[i].content->rdlength); j++) {
                answers[i].rdata[j] = reader[j];
            }
            answers[i].rdata[ntohs(answers[i].content->rdlength)] = '\0';

            ttl_value = ntohl(answers[i].content->ttl);
            reader += ntohs(answers[i].content->rdlength);  

            inet_ntop(AF_INET6, answers[i].rdata, ipv6_address, sizeof(ipv6_address));

            printf("  %s, AAAA, IN, %u, %s\n", answers[i].name, ttl_value, ipv6_address);
        } else if (ntohs(answers[i].content->type) == T_PTR) {
            answers[i].rdata = extractDomainName(reader, buffer, &stop);
            reader = reader + stop;
            ttl_value = ntohl(answers[i].content->ttl);
            reader += sizeof(unsigned int);

            printf("  %s, PTR, IN, %u, %s\n", answers[i].name, ttl_value, answers[i].rdata);
        }

        // Uvolní alokovanou paměť pro rdata a jméno odpovědi
        if (answers[i].rdata != NULL) {
            free(answers[i].rdata);
        }
        free(answers[i].name);
    }

    printf("Authority section (%d)\n", ntohs(dns->nscount));

    for (int i = 0; i < ntohs(dns->nscount); i++) {
        // Extrahuje doménové jméno autoritativního záznamu
        authority[i].name = extractDomainName(reader, buffer, &stop);
        reader = reader + stop;

        // Extrahuje obsah autoritativního záznamu a posune ukazatel na další pozici
        authority[i].content = (struct R_DATA *)(reader);
        reader = reader + sizeof(struct R_DATA);

        // Podle typu autoritativního záznamu zpracuje a vytiskne informace
        if (ntohs(authority[i].content->type) == T_NS) {
            authority[i].rdata = extractDomainName(reader, buffer, &stop);
            reader = reader + ntohs(authority[i].content->rdlength);
            ttl_value = ntohl(authority[i].content->ttl);
            printf(" %s, NS, IN, %u, %s\n", authority[i].name, ttl_value, authority[i].rdata);
        } else if (ntohs(authority[i].content->type) == T_SOA) {
            // Primary name server (MNAME)
            char *mname = (char *)extractDomainName(reader, buffer, &stop);
            reader = reader + stop;
            // Responsible person's email (RNAME)
            char *rname = (char *)extractDomainName(reader, buffer, &stop);
            // Serial number
            uint32_t serialNumber = ntohl(*(uint32_t *)reader);
            reader += sizeof(uint32_t);
            // Refresh Interval
            uint32_t refreshInt = ntohl(*(uint32_t *)reader);
            reader += sizeof(uint32_t);
            // Retry Interval
            uint32_t retryInt = ntohl(*(uint32_t *)reader);
            reader += sizeof(uint32_t);
            // Expire Limit
            uint32_t expire = ntohl(*(uint32_t *)reader);
            reader += sizeof(uint32_t);
            uint32_t minimum = ntohl(*(uint32_t *)reader);
            reader += sizeof(uint32_t);

            ttl_value = ntohl(authority[i].content->ttl);
            printf(" %s, SOA, IN, %u, %s, %s, %u, %u, %u, %u, %u\n", authority[i].name, ttl_value, mname, rname, serialNumber, refreshInt, retryInt, expire, minimum);
            free(mname);
            free(rname);
        } else {
            reader = reader + ntohs(authority[i].content->rdlength);
        }

        // Uvolní alokovanou paměť pro rdata a jméno autoritativního záznamu
        if (authority[i].rdata != NULL) {
            free(authority[i].rdata);
        }
        free(authority[i].name);
    }

    printf("Additional section (%d)\n", ntohs(dns->arcount));

    for (int i = 0; i < ntohs(dns->arcount); i++) {
        // Extrahuje doménové jméno dodatečného záznamu
        additional[i].name = extractDomainName(reader, buffer, &stop);
        reader += stop;

        // Extrahuje obsah dodatečného záznamu a posune ukazatel na další pozici
        additional[i].content = (struct R_DATA *)(reader);
        reader += sizeof(struct R_DATA);

        // Podle typu dodatečného záznamu zpracuje a vytiskne informace
        if (ntohs(additional[i].content->type) == T_A) {
            additional[i].rdata = (unsigned char *)malloc(ntohs(additional[i].content->rdlength));
            memcpy(additional[i].rdata, reader, ntohs(additional[i].content->rdlength));
            reader += ntohs(additional[i].content->rdlength);
            ttl_value = ntohl(additional[i].content->ttl);
            long *ipv4Address  = (long *)additional[i].rdata;
            a.sin_addr.s_addr = (*ipv4Address);
            printf(" %s, A, IN, %u, %s\n", additional[i].name, ttl_value, inet_ntoa(a.sin_addr));
        } else if (ntohs(additional[i].content->type) == T_AAAA) {
            additional[i].rdata = (unsigned char *)malloc(ntohs(additional[i].content->rdlength));
            memcpy(additional[i].rdata, reader, ntohs(additional[i].content->rdlength));
            reader += ntohs(additional[i].content->rdlength);

            printf(" %s, AAAA, IN, %u, ", additional[i].name, ntohl(additional[i].content->ttl));

            for (int j = 0; j < ntohs(additional[i].content->rdlength); j += 2) {
                printf("%02X%02X", additional[i].rdata[j], additional[i].rdata[j + 1]);
                if (j < ntohs(additional[i].content->rdlength) - 2) {
                    printf(":");
                }
            }
            printf("\n");
        } else if (ntohs(additional[i].content->type) == T_PTR || ntohs(additional[i].content->type) == T_CNAME) {
            additional[i].rdata = extractDomainName(reader, buffer, &stop);
            reader += stop;

            const char *recordType = (ntohs(additional[i].content->type) == T_PTR) ? "PTR" : "CNAME";
            printf(" %s, %s, IN, %u, %s\n", additional[i].name, recordType, ntohl(additional[i].content->ttl), additional[i].rdata);
        } else {
            reader += ntohs(additional[i].content->rdlength);
        }

        // Uvolní alokovanou paměť pro rdata a jméno dodatečného záznamu
        if (additional[i].rdata != NULL) {
            free(additional[i].rdata);
        }
        free(additional[i].name);
    }
}

void reverseIPinAddrArpa(const char* ipAddress, char* result) {
    // Pole pro uchování částí IP adresy
    unsigned int ipParts[4];

    // Načte části IP adresy ze vstupního řetězce
    if (sscanf(ipAddress, "%u.%u.%u.%u", &ipParts[0], &ipParts[1], &ipParts[2], &ipParts[3]) == 4) {
        // Sestaví reverzní DNS záznam
        sprintf(result, "%d.%d.%d.%d.in-addr.arpa", ipParts[3], ipParts[2], ipParts[1], ipParts[0]);
    } else {
        // Pokud načtení selže, vypíše chybové hlášení a ukončí program
        fprintf(stderr, "Neplatná IP adresa: %s\n", ipAddress);
        exit(1);
    }
}

void performDnsQuery(struct DNS_OPTIONS *options) {
    // Vytvoření socketu pro UDP komunikaci
    socketDes = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 

    // Signál pro ukončení načítání
    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        fprintf(stderr, "Chyba signalu CTRL C");
        exit(1);
    }

    // Nastavení informací o serveru
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(options->port);
    serverAddress.sin_addr.s_addr = inet_addr((char *)options->server); 

    // Inicializace bufferu pro DNS dotaz
    unsigned char buffer[BUFFER_SIZE];
    struct DNS_HEADER *dnsHeader = (struct DNS_HEADER *)&buffer;
    
    // Inicializace DNS hlavičky
    initDNSHeader(dnsHeader, options->recursionDesired);

    // Nastavení ukazatele na místo pro uložení otázky
    unsigned char *questionName = (unsigned char *)&buffer[sizeof(struct DNS_HEADER)];
    
    // Příprava informací o DNS dotazu
    char reverseQuery[64];
    struct QUESTION questionInfo;
    if (options->reverseQuery) {
        questionInfo.qtype = htons(T_PTR);
        reverseIPinAddrArpa((char*)options->address, reverseQuery);
    } else {
        questionInfo.qtype = options->useAAAA ? htons(T_AAAA) : htons(T_A);
        strcat((char*)options->address, ".");
    }
    char* address = options->reverseQuery ? (char*)reverseQuery : (char*)options->address;
    int len = strlen(address);
    int index = 0;
    int stop = 0;

    // Vytvoření DNS dotazovacího jména
    while (index < len) {
        int labelLen = 0;
        while (index + labelLen < len && address[index + labelLen] != '.') {
            labelLen++;
        }
        questionName[stop] = labelLen;
        stop++;
        for (int i = 0; i < labelLen; i++) {
            questionName[stop] = address[index + i];
            stop++;
        }
        index += labelLen;
        if (address[index] == '.') {
            index++;
        }
    }
    questionName[stop] = 0;
    
    // Nastavení třídy dotazu
    questionInfo.qclass = htons(1);
    // Kopírování informací o dotazu na správné místo v bufferu
    memcpy(questionName + strlen((char*)questionName) + 1, &questionInfo, sizeof(struct QUESTION));
    
    // Odeslání dotazu na server
    if (sendto(socketDes, (char *)buffer, sizeof(struct DNS_HEADER) + (strlen((const char *)questionName) + 1) + sizeof(struct QUESTION), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Sendto failed");
        exit(1);
    }
    // Příjem odpovědi od serveru
    int addressLength = sizeof serverAddress;
    if (recvfrom(socketDes, (char *)buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serverAddress, (socklen_t *)&addressLength) < 0)
    {
        perror("Recvfrom failed");
        exit(1);
    }
    
    // Zpracování odpovědi
    dnsHeader = (struct DNS_HEADER *)buffer;
    // Nastavení na pozici, kde začína odpověd
    unsigned char *reader = &buffer[sizeof(struct DNS_HEADER) + (strlen((const char *)questionName) + 1) + sizeof(struct QUESTION)];
    
    // Výpis informací o autoritě, rekurzi a dalších
    printf("Authoritative: %s, Recursive: %s, Truncated: %s\n",
           dnsHeader->aa ? "Yes" : "No",
           dnsHeader->rd ? "Yes" : "No",
           dnsHeader->tc ? "Yes" : "No");
  
    stop=0;
    struct RES_RECORD answers[20], authority[20], additional[20]; 
   
    // Výpis informací o dotazu
    printQuestionSection(&questionInfo, questionInfo.qtype == htons(T_PTR) ? reverseQuery : (const char*)options->address, dnsHeader);
    // Výpis ostatních dat
    printData(answers, authority, additional, buffer, stop, reader, dnsHeader);

    // Uzavření socketu
    close(socketDes);
}



int main(int argc, char *argv[])
{
	DNS_OPTIONS options = parseCommandLine(argc, argv);

    performDnsQuery(&options);

    return 0;
}