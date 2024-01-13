//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Adam Nieslanik (xniesl00)                    //
//         author: Michal Wagner (xwagne12)                     //
//         author: Adam Kala (xkalaa00)                         //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef PARSE
#define PARSE
#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "scanner.h"
#include "symtable.h"
#include "precedence.h"
#include "tokenqueue.h"

#define SYMBOL_STRING 400
#define SYMBOL_INTEGER 401
#define SYMBOL_DOUBLE 405

#define SYMBOL_GF 402
#define SYMBOL_LF 403
#define SYMBOL_TF 404

typedef struct
{
    token token; // current token
    TokenQueue *queue; // queue with stored tokens
    pStack stack; // expression stack
    TNode *variable; // helps with current variable
    TRoot *local_variable; // root for BVS with local variables
    TRoot *rootVar;       // root for BVS with global variables
    bool in_while_or_if; // indicates if expression is in while/if
    int function_type; // function's return type
    bool inReturn; // return expression
    bool inFunction; // indicates that we are in function
    int ifCounter;    // helps with nested if-elses
    int cnt; // counter for unique if/else, while labels
    int whileCounter; // helps with nested loops
    int helper; // counter that helps with label's name
    int caller; // helps with function's param check
    bool substring; // indicates that substring built-in was defined
    bool concatenate; // indicates concatenation instead of addittion
} Data;

/**
* @brief Initalize structure's parameters
* @param data Data structure 
*/
void InitData(Data *data);

/**
* @brief Main parsing function
* @param rootf BST for functions 
* @param queue Queue with tokens
* @return Error code
*/
int parse(TRootf *rootf,TokenQueue *queue);

/**
* @brief Performs expression parsing
* @param my_token Active token
* @param queue Queue with tokens
* @param in_while_or_if Indicates that expression is while/if condition 
* @param data Data structure 
* @param variable Variable to store result to
* @return Error code
*/
int Expression(token my_token, TokenQueue *queue, bool in_while_or_if,Data *data, TNode *variable);

/**
* @brief Checks, for function's ID
* @param rootf BST with functions
* @param queue Queue with tokens
* @return Error code
*/
int fundef(TRootf *rootf,TokenQueue *queue);

/**
* @brief Parse statement/ statement list
* @param rootf BST with functions
* @param queue Queue with tokens
* @return Error code
*/
int statement(TRootf *rootf,TokenQueue *queue);

/**
* @brief Checks for variable's/function's ID
* @param queue Queue with tokens
* @return Error code
*/
int ID(TokenQueue *queue);

/**
* @brief Checks for left bracket
* @return Error code
*/
int Lbracket();

/**
* @brief Checks for right bracket
* @param queue Queue with tokens
* @return Error code
*/
int Rbracket(TokenQueue *queue);

/**
* @brief Checks for left curly bracket
* @param queue Queue with tokens
* @return Error code
*/
int Lvinculum(TokenQueue *queue);

/**
* @brief Checks for right curly bracket
* @param queue Queue with tokens
* @return Error code
*/
int Rvinculum(TokenQueue *queue);

/**
* @brief Checks function's arguments
* @param rootf BST with functions
* @param queue Queue with tokens
* @param caller Indicates, if it's function's declaration or calling
* @return Error code
*/
int args(TRootf *rootf,TokenQueue *queue,int caller);

/**
* @brief Checks variable's type/function's return type
* @param queue Queue with tokens
* @param variable Queue with tokens
* @return Error code
*/
int type(TokenQueue *queue,TNode *variable);

/**
* @brief Returns variable/function data type
* @param queue Queue with tokens
* @return data type/error code
*/
int type_spec(TokenQueue *queue);

/**
* @brief Writes to stdout
* @param queue Queue with tokens
* @return Error code
*/
int writeFunc(TokenQueue *queue);

/**
* @brief Reads from stdin
* @param queue Queue with tokens
* @param type Type to read
* @return Error code
*/
int readFunc(TokenQueue *queue, int type);

/**
* @brief Convert integer number to double number
* @param queue Queue with tokens
* @return Error code
*/
int int2double(TokenQueue *queue);

/**
* @brief Convert double number to integer number
* @param queue Queue with tokens
* @return Error code
*/
int double2int(TokenQueue *queue);

/**
* @brief Returns character of ASCII value
* @param queue Queue with tokens
* @return Error code
*/
int chr(TokenQueue *queue);

/**
* @brief Returns ASCII value
* @param queue Queue with tokens
* @return Error code
*/
int ord(TokenQueue *queue);

/**
* @brief Gets substring of a string
* @return Error code
*/
int substring();

/**
* @brief Gets string's length
* @param queue Queue with tokens
* @return Error code
*/
int strlen_f(TokenQueue *queue);

/**
* @brief Clears operand's symbol
* @param op Operand
*/
void symbolClear(mystring *op);
#endif