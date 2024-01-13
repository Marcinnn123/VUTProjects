//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "scanner.h"
#include "str.h"

FILE *source; // source file

int setSourceFile(FILE *f){
    source = f;
    if (source == NULL){
        return INTERNAL_ERROR;
    }

    return 0;
}

int keywordCmp(mystring *str, token *mytoken) {
    // compare if we got keword, if yes set type to keyword and return 0
    if (mystrCompareConst(str, "if") == 0) {
        mytoken->type = KEYWORD_IF;
        return 0;
    } else if (mystrCompareConst(str, "else") == 0) {
        mytoken->type = KEYWORD_ELSE;
        return 0;
    } else if (mystrCompareConst(str, "func") == 0) {
        mytoken->type = KEYWORD_FUNC;
        return 0;
    } else if (mystrCompareConst(str, "Double") == 0) {
        mytoken->type = KEYWORD_DOUBLE;
        return 0;
    } else if (mystrCompareConst(str, "Int") == 0) {
        mytoken->type = KEYWORD_INT;
        return 0;
    } else if (mystrCompareConst(str, "nil") == 0) {
        mytoken->type = KEYWORD_NIL;
        return 0;
    } else if (mystrCompareConst(str, "return") == 0) {
        mytoken->type = KEYWORD_RETURN;
        return 0;
    } else if (mystrCompareConst(str, "String") == 0) {
        mytoken->type = KEYWORD_STRING;
        return 0;
    } else if (mystrCompareConst(str, "while") == 0) {
        mytoken->type = KEYWORD_WHILE;
        return 0;
    } else if (mystrCompareConst(str, "let") == 0) {
        mytoken->type = KEYWORD_LET;
        return 0;
    } else if (mystrCompareConst(str, "var") == 0) {
        mytoken->type = KEYWORD_VAR;
        return 0;
    } else if (mystrCompareConst(str, "Int?") == 0) {
        mytoken->type = KEYWORD_INT_QW;
        return 0;
    } else if (mystrCompareConst(str, "Double?") == 0) {
        mytoken->type = KEYWORD_DOUBLE_QW;
        return 0;
    } else if (mystrCompareConst(str, "String?") == 0) {
        mytoken->type = KEYWORD_STRING_QW;
        return 0;
    }
    // if we didnt get any, return 1
    return 1; 
}

void hexToDecimal(char hex1, char hex2, char *result) {
    // calculate the decimal value from the hexadecimal characters
    int decimalValue = 16 * hexCharToDecimal(hex1) + hexCharToDecimal(hex2);

    // convert the decimal value to a string and store it in the result
    result[0] = (decimalValue / 100) % 10 + '0';
    result[1] = (decimalValue / 10) % 10 + '0';
    result[2] = decimalValue % 10 + '0';
}

int hexCharToDecimal(char hex) {
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;
    } else if (hex >= 'a' && hex <= 'f') {
        return hex - 'a' + 10;
    } 
    return 1; // default return value if the character is not a valid hexadecimal digit
}


int scanNextToken(token *mytoken) {
    int state = STATE_INITIAL; // initial state
    char character = '\0'; 
    char hexaEscape1 = '0'; // escape characters
    char hexaEscape2 = '0';
    char result[3]; // result of converting hex to decima;
    bool isInTriple = false; // check if we should return to triple or classic string
    bool wasNewLine = false; // check if we got newline in triple string
    int block_comment_counter = 0; // nested comment check
    bool canTripleEnd = true;

    
    mystrInit(mytoken->content.str); // initcialize mystring
    mystrClear(mytoken->content.str);

    while (1) {
        character = (char) fgetc(source); // load the character

        switch (state) {
            case STATE_INITIAL:
                if (isspace(character)){
                    state = STATE_INITIAL; // skipping empty characters
                }
                else if (isalpha(character)){
                    state = STATE_IDENTF_OR_KEYW;
                    ungetc(character, source);
                }
                else if (character == '/'){
                    state = STATE_COMMENT_OR_DIVIDE;
                }
                else if (isdigit(character)){
                    state = STATE_DIGIT;
                    ungetc(character, source);
                }
                else if (character == '+'){
                    mytoken->type = TYPE_PLUS;
                    return OK;
                }
                else if (character == '-'){
                    state = STATE_MINUS_OR_POINT;
                }
                else if (character == '*'){
                    mytoken->type = TYPE_MULTIPLY;
                    return OK;
                }
                else if(character == '_'){
                    state = STATE_IDENTF_OR_UNDERSCORE;
                }
                else if(character == '{'){
                    mytoken->type = TYPE_LEFT_VINCULUM;
                    return OK;
                }
                else if(character == '}'){
                    mytoken->type = TYPE_RIGHT_VINCULUM;
                    return OK;
                }
                else if(character == '('){
                    mytoken->type = TYPE_LEFT_BRACKET;
                    return OK;
                }
                else if(character == ')'){
                    mytoken->type = TYPE_RIGHT_BRACKET;
                    return OK;
                }
                else if(character == ',') {
                    mytoken->type = TYPE_COMMA;
                    return OK;
                }
                else if(character == ':') {
                    mytoken->type = TYPE_COLON;
                    return OK;
                }
                else if(character == '='){
                    state = STATE_ASSIGN_OR_EQUAL;
                }
                else if(character == '>'){
                    state = STATE_BIGGER_START;
                }
                else if(character == '<'){
                    state = STATE_SMALLER_START;
                }
                else if(character == '!'){
                    state = STATE_NOT_EQUAL_OR_EX;
                }
                else if(character == '"'){
                    state = STATE_STRING_START;
                }
                else if(character == '?'){
                    state = STATE_DOUBLE_QW;
                }
                else if (character == EOF){
                    mytoken->type = TYPE_EOF;
                    return OK;
                }else{
                    return SCANNER_ERROR;
                }
                break; 

            case STATE_IDENTF_OR_UNDERSCORE:
                // we got identifier or keyword
                if(isalpha(character) || isdigit(character) || character == '_'){
                    mystrAppendChar(mytoken->content.str, '_');
                    ungetc(character, source);
                    state = STATE_IDENTF_OR_KEYW;
                } else{
                    // we got underscore
                    ungetc(character, source);
                    mytoken->type = TYPE_UNDERSCORE;
                    return OK;
                }
                break;
            case STATE_IDENTF_OR_KEYW:
                // continiue loading identifier or keyword 
                if (isalpha(character) || isdigit(character) || character == '_'){
                    mystrAppendChar(mytoken->content.str, character);
                }
                else if(character == '?'){
                    mystrAppendChar(mytoken->content.str, character);
                    if(keywordCmp(mytoken->content.str, mytoken)){ // check if we got keword
                        ungetc(character, source);
                        mystrRemoveLastChar(mytoken->content.str);
                        if(!(keywordCmp(mytoken->content.str, mytoken))){ // ckeck if it was keyword without ?
                            return OK;
                        }
                        mytoken->type = TYPE_VARIABLE;
                        return OK;
                    }
                    return OK;
                }
                else{
                    ungetc(character, source);
                    if(!(keywordCmp(mytoken->content.str, mytoken))){ // keyword check
                        return OK;
                    }
                    state = STATE_FUNC_OR_VAR;
                }
                break;
            case STATE_FUNC_OR_VAR:
                if(isspace(character)){
                    state = STATE_FUNC_OR_VAR;
                }
                else if(character == '('){
                    mytoken->type = TYPE_FUNCTION_NAME; // we got function name
                    ungetc(character, source);
                    return OK;
                }
                else{
                    mytoken->type = TYPE_VARIABLE; // identifier was a variable
                    ungetc(character, source);
                    return OK;
                }
                break;
            case STATE_COMMENT_OR_DIVIDE:
                if(character == '*'){ 
                    state = STATE_BLOCK_COMMENT; // we got block comment
                    block_comment_counter++;
                }
                else if(character == '/'){
                    state = STATE_LINE_COMMENT; // one line comment
                }
                else{
                    ungetc(character, source);
                    mytoken->type = TYPE_DIVIDE; // divide token
                    return OK;
                }
                break;
            case STATE_BLOCK_COMMENT:
                if(character == '*'){ // possible end of comment
                    state = STATE_BLOCK_COMMENT_END;
                    break;
                }
                else if(character == '/'){ // possible another block comment
                    state = STATE_ANOTHER_BLOCK_COMMENT;
                }
                else if(character == EOF){ // cant end without finishing comment
                    return SCANNER_ERROR;
                }
                break;
            case STATE_ANOTHER_BLOCK_COMMENT:
                if(character == '*'){
                    block_comment_counter++; // we got another block commment
                    state = STATE_BLOCK_COMMENT;
                }else if(character == EOF){
                    return SCANNER_ERROR;
                }else{
                    state = STATE_BLOCK_COMMENT;
                }
                break;
            case STATE_BLOCK_COMMENT_END:
                if(character == '/'){
                    block_comment_counter--; // block comment finished
                    if(block_comment_counter == 0){ // check if we ended all nested commets
                        state = STATE_INITIAL;
                    }else{
                        state = STATE_BLOCK_COMMENT;
                    }
                }else if(character == EOF){
                    return SCANNER_ERROR;
                }
                else{
                    state = STATE_BLOCK_COMMENT;
                }
                break;
            case STATE_LINE_COMMENT:
                if(character == '\n'){ // one line comment ended
                    state = STATE_INITIAL;
                }
                else if(character == EOF){
                    mytoken->type = TYPE_EOF;
                    return OK;
                }
                break;
            case STATE_DIGIT:
                if(isdigit(character)){
                    mystrAppendChar(mytoken->content.str, character); // loading integer
                }
                else if(character == '.'){
                    mystrAppendChar(mytoken->content.str, character); // double number
                    state = STATE_FLOAT;
                }
                else if(character == 'e' || character == 'E'){ // exponent number
                    mystrAppendChar(mytoken->content.str, character);
                    state = STATE_EXPONENT;
                }else{
                    ungetc(character, source);
                    mytoken->type = TYPE_INTEGER; // we got integer
                    mystrCopy(mytoken->content.integerNumber, mytoken->content.str);
                    return OK;
                }
                break;
            case STATE_FLOAT:
                if(isdigit(character)){
                    mystrAppendChar(mytoken->content.str, character); // float can end
                    state = STATE_FLOAT_END;
                }
                else if(character == 'e' || character == 'E'){ // we got exponent
                    mystrAppendChar(mytoken->content.str, character);
                    state = STATE_EXPONENT;
                }
                else{
                    return SCANNER_ERROR; // there has to be digit after .
                }
                break;
            case STATE_FLOAT_END:
                if(isdigit(character)){
                    mystrAppendChar(mytoken->content.str, character); // loading double
                }
                else if(character == 'e' || character == 'E'){ // we got exponent
                    mystrAppendChar(mytoken->content.str, character);
                    state = STATE_EXPONENT;
                }else{
                    mytoken->type = TYPE_DOUBLE; // we got double token
                    ungetc(character, source);
                    mystrCopy(mytoken->content.doubleNumber,mytoken->content.str);
                    return OK;
                }
                break;
            case STATE_EXPONENT:
                if(isdigit(character)){ 
                    mystrAppendChar(mytoken->content.str, character);
                    state = STATE_EXPONENT_END; // exponent can now end
                }
                else if(character == '+' || character == '-'){
                    mystrAppendChar(mytoken->content.str, character);
                    state = STATE_EXPONENT_DIGIT; // needed digint after +-
                }
                else{
                    return SCANNER_ERROR; // digit or +- needed after e
                }
                break;
            case STATE_EXPONENT_DIGIT:
                if(isdigit(character)){
                    mystrAppendChar(mytoken->content.str, character);
                    state = STATE_EXPONENT_END; // exponent number can end
                }else{
                    return SCANNER_ERROR;
                }
                break;
            case STATE_EXPONENT_END:
                if(isdigit(character)){
                    mystrAppendChar(mytoken->content.str,character); // loading to exponent number
                }
                else {
                    ungetc(character, source);
                    mytoken->type = TYPE_EXPONENT; // return exponent
                    return OK;
                }
                break;
            case STATE_ASSIGN_OR_EQUAL:
                if(character == '='){
                    mytoken->type = TYPE_EQUAL;
                    return OK;
                }
                else{
                    mytoken->type = TYPE_ASSIGN;
                    ungetc(character, source);
                    return OK;
                }
                break;
            case STATE_BIGGER_START:
                if(character == '='){
                    mytoken->type = TYPE_BIGGER_EQ;
                    return OK;
                }
                else{
                    ungetc(character, source);
                    mytoken->type = TYPE_BIGGER;
                    return OK;

                }
            case STATE_SMALLER_START:
                if(character == '='){
                    mytoken->type = TYPE_SMALLER_EQ;
                    return OK;
                }
                else{
                    ungetc(character, source);
                    mytoken->type = TYPE_SMALLER;
                    return OK;
                }
            case STATE_NOT_EQUAL_OR_EX:
                if(character == '='){
                    mytoken->type = TYPE_NOT_EQUAL;
                    return OK;
                }
                else{
                    ungetc(character, source);
                    mytoken->type = TYPE_EXCLAMATION;
                    return OK;
                }
                break;
            case STATE_STRING_START:
                if(character == '"'){
                    state = STATE_STRING_EMPTY_OR_TRIPLE; // check if we get "" or triple string
                }else if(character != EOF && character != '\n'){
                    ungetc(character, source);
                    state = STATE_STRING_END; // loading string
                }else{
                    return SCANNER_ERROR; // cant get newline after one "
                }
                break;
            case STATE_STRING_END:
                if(character != '"' && character != EOF && character != '\n'){
                    if(character == 92){
                        mystrAppendChar(mytoken->content.str, 92); // backlash
                        state = STATE_ESCAPE_BACKLASH;
                    }else if(isspace(character)){ // space for codegen
                        mystrAppendChar(mytoken->content.str, 92);
                        mystrAppendChar(mytoken->content.str, '0');
                        mystrAppendChar(mytoken->content.str, '3');
                        mystrAppendChar(mytoken->content.str, '2');
                    }else if(character == 35){ // # for codegen
                        mystrAppendChar(mytoken->content.str, 92);
                        mystrAppendChar(mytoken->content.str, '0');
                        mystrAppendChar(mytoken->content.str, '3');
                        mystrAppendChar(mytoken->content.str, '5');
                    }else{
                        mystrAppendChar(mytoken->content.str, character); // load character to string
                    }
                }
                else if(character == '"'){
                   mytoken->type = TYPE_STRING; // string ended
                   return OK;
                }
                else{
                    return SCANNER_ERROR; // error if EOF or newline
                }
                break;
            case STATE_STRING_EMPTY_OR_TRIPLE:
                if(character == '"'){
                    state = STATE_TRIPLE_STRING_START; // if another " then it is triple string
                }else{
                    ungetc(character, source);
                    mytoken->type = TYPE_STRING; // empty string
                    return OK;
                }
                break;
            case STATE_TRIPLE_STRING_START:
                if(character == '\n'){
                    state = STATE_TRIPLE_STRING; // newline so we can load string
                }else if(isspace(character)){
                    state = STATE_TRIPLE_STRING_START; // waiting for newline
                }else{
                    return SCANNER_ERROR;
                }
                break;
            case STATE_TRIPLE_STRING:
                isInTriple = true;
                if(character != '"' && character != EOF){
                    // same as in normal string but newline possible
                    if(character == '\n'){
                        wasNewLine = true;
                        canTripleEnd = true;
                    }else if(isspace(character)){
                        if(wasNewLine){
                            mystrAppendChar(mytoken->content.str, 92);
                            mystrAppendChar(mytoken->content.str, '0');
                            mystrAppendChar(mytoken->content.str, '1');
                            mystrAppendChar(mytoken->content.str, '0');
                            wasNewLine = false;
                        }
                        mystrAppendChar(mytoken->content.str, 92);
                        mystrAppendChar(mytoken->content.str, '0');
                        mystrAppendChar(mytoken->content.str, '3');
                        mystrAppendChar(mytoken->content.str, '2');
                    }else if(character == 35){
                        canTripleEnd = false;
                        if(wasNewLine){
                            mystrAppendChar(mytoken->content.str, 92);
                            mystrAppendChar(mytoken->content.str, '0');
                            mystrAppendChar(mytoken->content.str, '1');
                            mystrAppendChar(mytoken->content.str, '0');
                            wasNewLine = false;
                        }
                        mystrAppendChar(mytoken->content.str, 92);
                        mystrAppendChar(mytoken->content.str, '0');
                        mystrAppendChar(mytoken->content.str, '3');
                        mystrAppendChar(mytoken->content.str, '5');
                    }else if(character == 92){
                        canTripleEnd = false;
                        if(wasNewLine){
                            mystrAppendChar(mytoken->content.str, 92);
                            mystrAppendChar(mytoken->content.str, '0');
                            mystrAppendChar(mytoken->content.str, '1');
                            mystrAppendChar(mytoken->content.str, '0');
                            wasNewLine = false;
                        }
                        mystrAppendChar(mytoken->content.str, 92);
                        state = STATE_ESCAPE_BACKLASH;
                    }else{
                        canTripleEnd = false;
                        if(wasNewLine){
                            mystrAppendChar(mytoken->content.str, 92);
                            mystrAppendChar(mytoken->content.str, '0');
                            mystrAppendChar(mytoken->content.str, '1');
                            mystrAppendChar(mytoken->content.str, '0');
                            wasNewLine = false;
                        }
                        mystrAppendChar(mytoken->content.str, character);
                    }
                }
                else if(character == '"'){
                    state = STATE_TRIPLE_END_OR_NEXT; // possible end
                }
                else{
                    return SCANNER_ERROR;
                }
                break;
            case STATE_TRIPLE_END_OR_NEXT:
                if(character == '"'){ 
                    state = STATE_TRIPLE_POSSIBLE_END; // if we get another then end
                }else if(character == EOF){
                    return SCANNER_ERROR;
                }else{
                    if(wasNewLine){
                        mystrAppendChar(mytoken->content.str, 92);
                        mystrAppendChar(mytoken->content.str, '0');
                        mystrAppendChar(mytoken->content.str, '1');
                        mystrAppendChar(mytoken->content.str, '0');
                        wasNewLine = false;
                    }
                    mystrAppendChar(mytoken->content.str, '"'); // print "
                    ungetc(character, source);
                    state = STATE_TRIPLE_STRING;
                }
                break;
            case STATE_TRIPLE_POSSIBLE_END:
                if(character == '"'){
                    if(canTripleEnd){
                        mytoken->type = TYPE_STRING; // end of triple string
                        return OK;
                    }else{
                        return SCANNER_ERROR;
                    }
                    
                }else if(character == EOF){
                    return SCANNER_ERROR;
                }else{
                    if(wasNewLine){
                        mystrAppendChar(mytoken->content.str, 92);
                        mystrAppendChar(mytoken->content.str, '0');
                        mystrAppendChar(mytoken->content.str, '1');
                        mystrAppendChar(mytoken->content.str, '0');
                        wasNewLine = false;
                    }
                    mystrAppendChar(mytoken->content.str, '"'); // print ""
                    mystrAppendChar(mytoken->content.str, '"');
                    ungetc(character, source);
                    state = STATE_TRIPLE_STRING;
                }
                break;
            case STATE_ESCAPE_BACKLASH:
                // print escape sequences
                if(character == 't'){
                    mystrAppendChar(mytoken->content.str, '0');
                    mystrAppendChar(mytoken->content.str, '0');
                    mystrAppendChar(mytoken->content.str, '9');
                }else if(character == '"'){
                    mystrAppendChar(mytoken->content.str, '0');
                    mystrAppendChar(mytoken->content.str, '3');
                    mystrAppendChar(mytoken->content.str, '4');
                }else if(character == 92){
                    mystrAppendChar(mytoken->content.str, '0');
                    mystrAppendChar(mytoken->content.str, '9');
                    mystrAppendChar(mytoken->content.str, '2');
                }else if(character == 'n'){
                    mystrAppendChar(mytoken->content.str, '0');
                    mystrAppendChar(mytoken->content.str, '1');
                    mystrAppendChar(mytoken->content.str, '0');
                }else if(character == 'u'){
                    state = STATE_HEXA_PREPARE; // prepare for hexa numbers
                    break;
                } else if(isdigit(character)){
                    ungetc(character, source);
                }else{
                    return SCANNER_ERROR;
                }
                if(isInTriple){
                    state = STATE_TRIPLE_STRING;
                }else{
                    state = STATE_STRING_END;
                }                                                                                                                                                                                                             
                break;
            case STATE_HEXA_PREPARE:
                if(character == '{'){
                    state = STATE_FIRST_HEXA; // prepare for first hexa
                }else{
                    mystrAppendChar(mytoken->content.str, character);
                    if(isInTriple){
                        state = STATE_TRIPLE_STRING;
                    }else{
                        state = STATE_STRING_END;
                    } 
                }
                break;
            case STATE_FIRST_HEXA:
                if(character == '0'){
                    state = STATE_FIRST_HEXA; // skipping 0
                }else if((character > '0' && character <= '9') || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F')){
                    hexaEscape1 = character; // set it as first hexa and prepare for second
                    state = STATE_SECOND_HEXA;
                }else if(character == '}'){
                    ungetc(character, source);
                    state = STATE_PRINT_HEXA_IN_DEC; // we got only 0
                }else{
                    mystrAppendChar(mytoken->content.str, character);
                    if(isInTriple){
                        state = STATE_TRIPLE_STRING;
                    }else{
                        state = STATE_STRING_END;
                    }
                }
                break;
            case STATE_SECOND_HEXA:
                    if(character == '}'){ 
                        ungetc(character, source); // we got only one hexa
                        hexaEscape2 = hexaEscape1;
                        hexaEscape1 = '0';
                        state = STATE_PRINT_HEXA_IN_DEC;
                    }
                    else if((character >= '0' && character <= '9') || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F')){
                        hexaEscape2 = character; // load second hexa
                        state = STATE_PRINT_HEXA_IN_DEC; 
                    }else{
                        if(!isdigit(character)){
                            return SCANNER_ERROR; 
                        }
                        mystrAppendChar(mytoken->content.str, character);
                        if(isInTriple){
                            state = STATE_TRIPLE_STRING;
                        }else{
                            state = STATE_STRING_END;
                        }
                    }
                break;
            case STATE_PRINT_HEXA_IN_DEC:
                if(character == '}'){
                    hexToDecimal(hexaEscape1, hexaEscape2, result); // convert hex to dec
                    mystrAppendChar(mytoken->content.str, result[0]);
                    mystrAppendChar(mytoken->content.str, result[1]); // print it in needed version
                    mystrAppendChar(mytoken->content.str, result[2]);
                }else{
                    mystrAppendChar(mytoken->content.str, character);
                }
                if(isInTriple){
                    state = STATE_TRIPLE_STRING;
                }else{
                    state = STATE_STRING_END;
                }
                break;
            case STATE_MINUS_OR_POINT:
                if(character == '>'){
                    mytoken->type = TYPE_POINT;
                    return OK;
                }
                else{
                    ungetc(character, source);
                    mytoken->type = TYPE_MINUS;
                    return OK;
                }
                break;
            case STATE_DOUBLE_QW:
                if(character == '?'){
                    mytoken->type = TYPE_DOUBLE_QWMARK;
                    return OK;
                }
                else{
                    return SCANNER_ERROR;
                }
                break;
            break;
        }
    }
}
