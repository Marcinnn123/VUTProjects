//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include "str.h"
#include <stdio.h>


#define INITIAL_CAPACITY 8
 

int mystrInit(mystring *s) {
    s->str = (char*)malloc(INITIAL_CAPACITY);
    if (s->str == NULL) {
        return 1;
    }
    s->str[0] = '\0';
    s->length = 0;
    s->symbol = 0;
    s->capacity = INITIAL_CAPACITY;
    s->variable_type = 0;
    return 0;
}

void mystrFree(mystring *s) {
    free(s->str);
}

void mystrClear(mystring *s) {
    
    s->str[0] = '\0';
    s->length = 0;
}

int mystrAppendChar(mystring *s, char c) {
    if (s->length + 1 >= s->capacity) {
        int newCapacity = s->capacity + INITIAL_CAPACITY;
        char *newStr = (char*)realloc(s->str, newCapacity);
        if (newStr == NULL) {
            return 1;
        }
        s->str = newStr;
        s->capacity = newCapacity;
    }
    s->str[s->length] = c;
    s->length++;
    s->str[s->length] = '\0';
    return 0;
}

int mystrCopy(mystring *dest, mystring *src) {
    int srcLength = src->length;
    if (srcLength >= dest->capacity) {
        int newCapacity = srcLength + 1;
        char *newStr = (char*)realloc(dest->str, newCapacity);
        if (newStr == NULL) {
            return 1;
        }
        dest->str = newStr;
        dest->capacity = newCapacity;
    }
    if(srcLength == 0){
        src->str = "";
    }
    strcpy(dest->str, src->str);
    dest->length = srcLength;
    return 0;
}

int mystrCompare(mystring *s1, mystring *s2) {
    return strcmp(s1->str, s2->str);
}

int mystrCompareConst(mystring *s1, char *s2) {
    return strcmp(s1->str, s2);
}

char *mystrGetString(mystring *s) {
    return s->str;
}

int mystrGetLength(mystring *s) {
    return s->length;
}

void mystrRemoveLastChar(mystring *s) {
    if (s->length > 0) {
        s->length--;
        s->str[s->length] = '\0';
    }
}

