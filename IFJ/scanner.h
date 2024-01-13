//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef SCANNER_H
#define SCANNER_H

#include "str.h"
#include "errors.h"

/**
 * Definitions for keywords
 */
#define KEYWORD_IF                  100   // if        
#define KEYWORD_ELSE                101   // else        
#define KEYWORD_INT                 102   // Int  
#define KEYWORD_DOUBLE              103   // Double     
#define KEYWORD_STRING              104   // String      
#define KEYWORD_NIL                 105   // nil      
#define KEYWORD_FUNC                106   // func    
#define KEYWORD_RETURN              107   // return            
#define KEYWORD_WHILE               108   // while
#define KEYWORD_VAR                 109   // var
#define KEYWORD_LET                 110   // let
#define KEYWORD_INT_QW              111   // Int?
#define KEYWORD_STRING_QW           112   // String?
#define KEYWORD_DOUBLE_QW           113   // Double?

/**
 * Definitions for token types.
 */
#define TYPE_START                  199
#define TYPE_FUNCTION_NAME          200
#define TYPE_INTEGER                201
#define TYPE_DOUBLE                 202   
#define TYPE_STRING                 203
#define TYPE_PLUS                   204   // +
#define TYPE_MINUS                  205   // -
#define TYPE_MULTIPLY               206   // *
#define TYPE_DIVIDE                 207   // /
#define TYPE_SMALLER                208   // <
#define TYPE_SMALLER_EQ             209   // <=
#define TYPE_BIGGER                 210   // >
#define TYPE_BIGGER_EQ              211   // >=
#define TYPE_EQUAL                  212   // ==
#define TYPE_NOT_EQUAL              213   // !=
#define TYPE_ASSIGN                 214   // =
#define TYPE_EOF                    215   
#define TYPE_LEFT_VINCULUM          216   // {
#define TYPE_RIGHT_VINCULUM         217   // }
#define TYPE_LEFT_BRACKET           218   // (
#define TYPE_RIGHT_BRACKET          219   // )
#define TYPE_UNDERSCORE             220   // _
#define TYPE_COLON                  221   // :
#define TYPE_COMMA                  222   // ,
#define TYPE_POINT                  223   // ->
#define TYPE_DOUBLE_QWMARK          224   // ??
#define TYPE_VARIABLE               225   //
#define TYPE_EXPONENT               226
#define TYPE_EXCLAMATION            227   // !

/**
 * Definitions for scanner states
 */
#define STATE_INITIAL                 300
#define STATE_IDENTF_OR_KEYW          302
#define STATE_COMMENT_OR_DIVIDE       304
#define STATE_LINE_COMMENT            305
#define STATE_BLOCK_COMMENT           306
#define STATE_BLOCK_COMMENT_END       307
#define STATE_DIGIT                   308
#define STATE_FLOAT                   309
#define STATE_FLOAT_END               310
#define STATE_ASSIGN_OR_EQUAL         311
#define STATE_BIGGER_START            312
#define STATE_SMALLER_START           313
#define STATE_NOT_EQUAL_OR_EX         314
#define STATE_STRING_START            315
#define STATE_IDENTF_OR_UNDERSCORE    316
#define STATE_MINUS_OR_POINT          317
#define STATE_DOUBLE_QW               319
#define STATE_FUNC_OR_VAR             320
#define STATE_EXPONENT                321
#define STATE_EXPONENT_END            322
#define STATE_TRIPLE_STRING           323
#define STATE_ANOTHER_BLOCK_COMMENT   324
#define STATE_EXPONENT_DIGIT          325
#define STATE_ESCAPE_BACKLASH         326
#define STATE_STRING_EMPTY_OR_TRIPLE  327
#define STATE_STRING_END              328
#define STATE_TRIPLE_END_OR_NEXT      329
#define STATE_TRIPLE_POSSIBLE_END     330
#define STATE_HEXA_PREPARE            331
#define STATE_FIRST_HEXA              332
#define STATE_SECOND_HEXA             333
#define STATE_PRINT_HEXA_IN_DEC       334
#define STATE_TRIPLE_STRING_START     335

/**
 * Union for token content, including integer, double, and string
 */
typedef union {
    mystring *integerNumber; // Pointer to the string representation of an integer 
    mystring *doubleNumber;  // Pointer to the string representation of a double
    mystring *str;           // Pointer to the string content
} tokenContent;

/**
 * @brief Structure representing a token, including content and type
 */
typedef struct {
    tokenContent content; // Union representing the content of the token
    int type;             // Type of the token
    int qw;
} token;

/**
 * @brief Compare a string with a token's content to check if it matches a keyword
 *
 * @param str Pointer to the string for comparison
 * @param mytoken Pointer to the token where we will update type
 * @return Integer representing the result of the comparison
 *         0 if the string matches the keyword, 1 otherwise
 */
int keywordCmp(mystring *str, token* mytoken);

/**
 * @brief Set the source file for scanning
 *
 * @param f Pointer to the source file
 * @return Integer representing the result of the operation
 */
int setSourceFile(FILE *f);

/**
 * @brief Convert hexadecimal characters to decimal
 *
 * @param hex1 First hexadecimal character
 * @param hex2 Second hexadecimal character
 * @param result Pointer to the result string for decimal representation
 */
void hexToDecimal(char hex1, char hex2, char *result);

/**
 * @brief Convert a hexadecimal character to decimal
 *
 * @param hex Hexadecimal character
 * @return Integer representing the decimal equivalent
 */
int hexCharToDecimal(char hex);

/**
 * @brief Scan the next token in the source file
 *
 * @param mytoken Pointer to the token structure for storing the scanned token
 * @return Integer representing the result of the scanning operation
 *         OK if successful, or an error code if an issue is encountered
 */
int scanNextToken(token * mytoken);

#endif