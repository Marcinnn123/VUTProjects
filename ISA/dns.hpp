/*
** VUT FIT 2023 - ISA
** DNS resolver
** Marcin Sochacki
** xsocha03
*/
#define T_A 1                /* IPv4 adresový záznam. */
#define T_AAAA 28             /* IPv6 adresový záznam. */
#define T_NS 2                /* Autoritativní záznam jmenného serveru. */
#define T_CNAME 5             /* Kanonický název záznamu. */
#define T_SOA 6               /* Záznam začátku autority. */
#define T_PTR 12              /* Záznam ukazatele. */
#define BUFFER_SIZE 65536     /* Velikost bufferu pro DNS. */

/**
 * @struct DNS_OPTIONS
 * @brief Struktura obsahující nastavení DNS dotazu
 */
struct DNS_OPTIONS {
    bool recursionDesired;          /* Příznak požadavku na rekurzi. */
    bool reverseQuery;              /* Příznak reverzního dotazu. */
    bool useAAAA;                   /* Příznak použití AAAA pro IPv6. */
    unsigned char server[100];      /* Adresa DNS serveru. */
    int port;                       /* Port DNS serveru. */
    unsigned char address[100];     /* Adresa pro dotaz. */
};

/**
 * @struct DNS_HEADER
 * @brief Struktura DNS hlavičky
 */
struct DNS_HEADER
{
	unsigned short id :16;              //  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5                              
	unsigned char rd :1;                // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	unsigned char tc :1;                // |                      ID                       |
	unsigned char aa :1;                // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	unsigned char opcode :4;            // |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
	unsigned char qr :1;                // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    unsigned char rcode :4;             // |                    QDCOUNT                    |
	unsigned char z :3;             	// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	unsigned char ra :1;				// |                    ANCOUNT                    |
	unsigned short qdcount : 16;		// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	unsigned short ancount : 16;        // |                    NSCOUNT                    |
	unsigned short nscount : 16;        // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    unsigned short arcount : 16;        // |                    ARCOUNT                    |
};	        							// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

/**
 * @struct QUESTION
 * @brief Struktura DNS otázky
 */    							
struct QUESTION
{													// 0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
	unsigned short qtype;							//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	unsigned short qclass;							//|                                               |
};													//|                     QNAME                     |
													//}                                               |
/**													//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * @struct QUERY									//|                     QTYPE                     |
 * @brief Struktura pro reprezentaci DNS dotazu 	//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */													//|                     QCLASS                    |
typedef struct										//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+							
{										
	unsigned char *name;				
	struct QUESTION *question;			
} QUERY;								

/**
 * @struct R_DATA
 * @brief Struktura dat DNS záznamu
 */								
#pragma pack(push, 1)
struct R_DATA								// 0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
{											//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+	
	unsigned short type;					//|                      NAME                     |
	unsigned short _class;					//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	unsigned int ttl;						//|                      TYPE                     |
	unsigned short rdlength;				//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
};											//|                     CLASS                     |
#pragma pack(pop)							//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
											//|                      TTL                      |
/**											//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * @struct RES_RECORD						//|                   RDLENGTH                    |
 * @brief Struktura DNS záznamu 			//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 */											//|                     RDATA                     |
struct RES_RECORD							//+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+						
{										
	unsigned char *name;				
	struct R_DATA *content;				
	unsigned char *rdata;				
};										
										
/**
 * @brief Obsluha signálu SIGINT
 *
 * @param signal Příchozí signál
 */
void sigintHandler(int);

/**
 * @brief Zpracovává argumenty příkazového řádku a extrahuje nastavení DNS dotazu
 *
 * @param argc Počet argumentů příkazového řádku
 * @param argv Pole argumentů příkazového řádku
 * @return Struktura DNS_OPTIONS obsahující zpracované nastavení
 */
DNS_OPTIONS parseCommandLine(int argc, char* argv);

/**
 * @brief Inicializuje hlavičku DNS dotazu
 *
 * @param dns Ukazatel na strukturu DNS_HEADER, kterou inicializuje
 * @param recursionDesired Určuje, zda je žádána rekurze
 */
void initDNSHeader(struct DNS_HEADER* dns, bool recursionDesired, bool reverseQuery, bool useAAAA);

/**
 * @brief Extrahuje doménové jméno ze zprávy DNS
 *
 * @param reader Ukazatel na čtený blok dat
 * @param buffer Ukazatel na buffer dat
 * @param labelCount Počet etiket v doménovém jméně
 * @return Ukazatel na extrahované doménové jméno
 */
unsigned char* extractDomainName(unsigned char*,unsigned char*,int*);

/**
 * Vytiskne informace o sekci otázek v DNS dotazu
 *
 * @param qinfo Ukazatel na strukturu QUESTION obsahující informace o otázce
 * @param address Adresa, na kterou je dotaz směřován
 * @param dns Ukazatel na strukturu DNS_HEADER obsahující hlavičku DNS zprávy
 */
void printQuestionSection(struct QUESTION* qinfo, const char* address, struct DNS_HEADER * dns);

/**
 * Vytiskne informace o sekci odpovědí, autoritativních záznamech a doplňkových informacích v DNS zprávě
 *
 * @param answers Pole struktur obsahující odpovědi na DNS dotaz
 * @param auth Pole struktur obsahující autoritativní záznamy DNS dotazu
 * @param addit Pole struktur obsahující doplňkové informace DNS dotazu
 * @param buffer Ukazatel na buffer s DNS zprávou
 * @param stop Počet oktetů přečtených při extrakci doménového jména
 * @param reader Ukazatel na aktuální pozici v bufferu s DNS zprávou
 * @param dns Ukazatel na strukturu DNS_HEADER obsahující hlavičku DNS zprávy
 */
void printData(struct RES_RECORD* answers, struct RES_RECORD* auth, struct RES_RECORD* addit, unsigned char* buffer, int stop, unsigned char* reader, struct DNS_HEADER * dns);
/**
 * Převede IP adresu na formát pro reverzní DNS záznam typu PTR
 *
 * @param ipAddress Řetězec reprezentující IP adresu ve formátu "x.x.x.x"
 * @param result Výstupní řetězec pro uložení výsledného reverzního DNS záznamu
 */
void reverseIPinAddrArpa(const char* ipAddress, char* result); 

/**
 * Spracuje DNS dotaz na zadaný server s použitím zadaných možností
 *
 * @param options Struktura obsahující nastavení DNS dotazu.
 */
void performDnsQuery(struct DNS_OPTIONS *);