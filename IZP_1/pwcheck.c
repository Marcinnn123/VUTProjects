#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PASSWORD_LENGHT 100
#define SUCCESS 0
#define FAILURE 1

struct stats
{
    bool stats_print;
    float pw_count;
    char nchars[256];
    float all_pw_lenght;
    int min_lenght;
};

struct password_info
{
    char password[MAX_PASSWORD_LENGHT];
    int small;
    int caps;
    int chara;
    int num;
    int level;
    int param;
};

bool arguments_processing(int argc, char **argv)
{
    if(argc < 3 || argc > 4)
    {
        fprintf(stderr, "Too many arguments!\n");
        return FAILURE;
    }

    int lvl = atoi(argv[1]);
    int parm = atoi(argv[2]);

    if(lvl < 1 || lvl > 4)
    {
        fprintf(stderr, "Wrong value of argument: LEVEL!\n");
        return FAILURE;
    }
    if(parm < 1)
    {
        fprintf(stderr, "Wrong value of argument: PARAM!\n");
        return FAILURE;
    }

    if(argc == 4)
    {
        int are_stats = strcmp("--stats", argv[3]);
        if(are_stats)
        {
            fprintf(stderr, "Wrong value of argument: --stats!\n");
            return FAILURE;
        }
    }    
    return SUCCESS;
}
void struct_init(int argc, char **argv, struct password_info *s, struct stats *stats)
{
    int lvl = atoi(argv[1]);
    int parm = atoi(argv[2]);

    stats->min_lenght = 0;
    stats->pw_count = 0;
    stats->all_pw_lenght = 0;
    s->small = 0;
    s->caps = 0;
    s->chara = 0;
    s->num = 0;
    s->level = lvl;
    s->param = parm;
    for(int i = 0; i <256; i++)
    {
        stats->nchars[i] = '\0';
    }
    switch(argc)
    {
        case 4:
            stats->stats_print = true;
            break;
        default:
            stats->stats_print = false;
            break;
    }
} 
bool lvl1(struct password_info *s)
{
    if(s->small > 0 && s->caps > 0)
    {
        return true;
    }

    return false;
}
bool lvl2(struct password_info *s)
{
    if(s->param > 4)
    {
        s->param = 4;
    }
    int param_check = 0;
    if(s->small > 0)
    {
        param_check++;
    }
    if(s->caps > 0)
    {
        param_check++;
    }
    if(s->num > 0)
    {
        param_check++;
    }
    if(s->chara > 0)
    {
        param_check++;
    }
    if(param_check >= s->param)
    {
        return true;
    }
    return false;
}
bool lvl3(struct password_info *s)
{
    int param_check = 1;

    for(int i = 0; s->password[i] != '\0'; i++)
    {
        if(s->password[i] == s->password[i + 1])
        {
            param_check++;
        }
        else
        {
            param_check = 1;
        }

        if(param_check == s->param)
        {
            return false;
        }
            
    }
    return true;
}
bool lvl4(struct password_info *s)
{
    for (int i = 0; s->password[i] != '\0'; i++) {
        for (int j = i + 1; s->password[j] != '\0'; j++) {
            bool found = true;
            for (int k = 0; k < s->param; k++) {
                if (s->password[i + k] == '\0' || s->password[j + k] == '\0' || s->password[i + k] != s->password[j + k])
                {
                    found = false;
                    break;
                }
            }
            if (found) {
                return false;
            }
        }
    }

    return true;
}
void character_check(int c, struct password_info *s)
{
    if(c >= 97 && c <= 122)
    {
        s->small++;
    }
    else if(c >= 65 && c <= 90)
    {
        s->caps++;
    }
    else if(c >= 48 && c <= 57)
    {
        s->num++;
    }
    else if(c >= 32 && c <= 126)
    {
        s->chara++;
    }
   
}
void minimal_lenght(struct password_info *s, struct stats *stats)
{   
    int i = 0;

    while (s->password[i] != '\0' && s->password[0] != '\n') 
    {
        i++;
    }
    if(((stats->min_lenght > i) || (stats->min_lenght == 0)) && (s->password[0] != '\n'))
    {
        stats->min_lenght = i-1;
    }
}
void check_all_chars(int c, struct stats *stats)
{
    if(stats->nchars[0] == '\0')
    {
        stats->nchars[0] = c;
    }else if((char)(c) == '\n')
    {
        
    }
    else
    {
        bool char_found = false;
        for(int i = 0; stats->nchars[i] != '\0';i++)
        {
            if(c == stats->nchars[i])
            {
                char_found = true;
            }
        }
        if(!char_found)
        {
            stats->nchars[strlen(stats->nchars)] = c;
        }
    }
    
}  
void average_lenght(struct password_info *s, struct stats *stats)
{
    stats->all_pw_lenght = stats->all_pw_lenght + ((int)strlen(s->password)-1);
    stats->pw_count++;  
} 
void password_print(struct password_info *s)
{
    if(s->level == 1)
    {
        if(lvl1(s))
        {
            printf("%s", s->password);
        }
    }
    if(s->level == 2)
    {
        if(lvl1(s) && lvl2(s))
        {
            printf("%s", s->password);
        }
    }
    if(s->level == 3)
    {
        if(lvl1(s) && lvl2(s) && lvl3(s))
        {
            printf("%s", s->password);
        }
    }
    if(s->level == 4)
    {
        if(lvl1(s) && lvl2(s) && lvl3(s) && lvl4(s))
        {
            printf("%s", s->password);
        }
    }
    
}
void stats_print(struct stats *stats)
{   if(stats->stats_print)
    {
        float avg;
        if(stats->pw_count == 0)
        {
            avg = 0;
        }else
        {
            avg = stats->all_pw_lenght / stats->pw_count;
        }
        printf("\nStatistiky:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", ((int)strlen(stats->nchars)-1), stats->min_lenght, avg);
    }
}
void password_info_reset(struct password_info *s)
{
    password_print(s);
    for(int j = 0; j < 100;j++)
    {
        s->password[j] = '\0';
    }
    s->small = 0;
    s->caps = 0;
    s->chara = 0;
    s->num = 0;
}
void password_proccesing(struct password_info *s, struct stats *stats)
{
    int c;
    int i = 0;

    
    while(!feof(stdin))
    {
        c = fgetc(stdin);

            if(c == '\n')
            {
                average_lenght(s, stats);
                minimal_lenght(s,stats);
                password_info_reset(s);
                i = 0;
            } 
        check_all_chars(c, stats);
        character_check(c, s);
        s->password[i] = (char)(c);
        s->password[i+1] = '\0';
        i++;
        
        
    }
    stats_print(stats);
}

int main(int argc, char **argv)
{
    struct stats stats;
    struct password_info password_info;

    if(arguments_processing(argc, argv))
    {
        return FAILURE;
    }
    struct_init(argc, argv, &password_info, &stats);
    password_proccesing(&password_info, &stats);

    return SUCCESS;
}

