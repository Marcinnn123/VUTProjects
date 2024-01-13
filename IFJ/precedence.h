//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author:  Michal Wagner (xwagne12)                    //
//         author:  Adam Kala (xkalaa00)                        //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef PREC_H
#define PREC_H
#include "pstack.h"
#include "error.h"
#include "scanner.h"
#include "str.h"
#include "symtable.h"
#include "codegen.h"
#include "tokenqueue.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/**
 * Symbols for precedence analysis.
 */
typedef enum
{
    /*00*/pAdd,
    /*01*/pMinus,
    /*02*/pMultiply,
    /*03*/pDivide,
    /*04*/pLeftBracket,
    /*05*/pRightBracket,
    /*06*/pExclamation,
    /*07*/pEqual,
    /*08*/pNotEqual,
    /*09*/pSmaller,
    /*10*/pBigger,
    /*11*/pSmallerEq,
    /*12*/pBiggerEq,
    /*13*/pDoubleQwmark,
    /*14*/pID,
    /*15*/pDollar,
    /*16*/pOther,
} Element;

/**
 * Function gets precedence operation from table.
 *
 * @param new_token Current token.
 * @return Returns precedence operation symbol.
 */
char prec_op(token new_token);

/**
 * Function will inicialize stack with $ on top.
 *
 * @param stack Pointer to stack.
 * @param variable Variable with needed data.
 * @return Returns given error.
 */
int ExpStack(pStack *stack, TNode *variable);

/**
 * Function converts token type to symbol.
 *
 * @param token Current token.
 * @return Returns precedence operation element.
 */
int ConvertToken(token token);

/**
 * Checks if there are two operands or variables behind each other.
 *
 * @param stack Pointer to stack.
 * @param token Current token.
 * @return Returns precedence operation element.
 */
int checkTwoOperandsOrVars(pStack *stack, token token);

/**
 * Function to check type of returning value from function.
 *
 * @param stack Pointer to stack.
 * @param type Data type of function.
 * @return Returns given error.
 */
int ReturnType(pStack *stack, int type);

/**
 * Function to check semantic of operands and converts data type.
 *
 * @param stack Pointer to stack.
 * @param op Operator of expression.
 * @param root_variable_type Data type of variable we are assigning to.
 * @param in_while_or_if True if we are in if or while statement header.
 * @return Returns given error.
 */
int CheckExpression(pStack *stack, int op, int root_variable_type, bool in_while_or_if);

/**
 * Function to reduce brackets.
 *
 * @param stack Pointer to stack.
 * @return Returns given error.
 */
int ReduceBracket(pStack *stack);

/**
 * Function that uses rule for reduction of terminals.
 *
 * @param stack Pointer to stack.
 * @param queue Queue with tokens.
 * @param assign True if we are assigning to variable one value.
 * @param root_variable_type Data type of variable we are assigning to.
 * @param in_while_or_if True if we are in if or while statement header.
 * @return Returns given error.
 */
int ReduceExpression(pStack *stack, TokenQueue *queue, bool assign, int root_variable_type, bool in_while_or_if);

/**
 * Function that creates new element for stack.
 *
 * @param str Content of token.
 * @param index Converted symbol in precedence.
 * @param type Data type of token.
 * @param is_var True if token is variable.
 * @param qw True if variable is String?, Int?, Double?.
 * @return Returns created element.
 */
ExpElement *NewElement(mystring *str, int index, int type, TNode *variable, bool is_var, bool qw);

/**
 * Main function for precedence analysis.
 *
 * @param stack Pointer to stack.
 * @param token Current token.
 * @param queue Queue with tokens.
 * @param in_while_or_if True if we are in if or while statement header.
 * @param variable Variable with needed data.
 * @return Returns given error.
 */
int PrecedenceAnalysis(pStack *stack, token token, TokenQueue *queue, bool in_while_or_if, TNode *variable);

#endif