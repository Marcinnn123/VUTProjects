//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author:  Michal Wagner (xwagne12)                    //
//         author:  Adam Kala (xkalaa00)                        //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef PSTACK
#define PSTACK
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include "str.h"
#include "scanner.h"


/*function for creating element from token*/
typedef struct exp{
    token token;
    int index;
    bool terminal;
    bool active_area;
    int var_type;
    int root_var;
    bool is_var;
} ExpElement;

typedef struct stack_item{
    ExpElement *element; //Element on stack used in prec.
    struct stack_item *next_element; //next element on stack
} StackItem;


typedef struct{
    StackItem *first_item; //First item in stack
}pStack;

/**
 * Function to inicialize stack
 *
 * @param stack Pointer to stack.
 */
void pStack_init(pStack *stack);

/**
 * Function to push element on stack
 *
 * @param stack Pointer to stack.
 * @param elem Pointer to element.
 * @return Returns true or false.
 */
bool pStack_push(pStack *stack, ExpElement *elem);


/**
 * Function to pop element from stack
 *
 * @param stack Pointer to stack.
 * @return Returns true or false.
 */
bool pStack_pop(pStack *stack);


/**
 * Pops three times
 *
 * @param stack Pointer to stack.
 */
void pStack_pop3(pStack *stack);


/**
 * Function to get third element on stack
 *
 * @param stack Pointer to stack.
 * @return Returns third element on stack
 */
StackItem *pStack_get_3(pStack *stack);


/**
 * Function to get second element on stack
 *
 * @param stack Pointer to stack.
 * @return Returns second element on stack
 */
StackItem *pStack_get_2(pStack *stack);


/**
 * Function to get first element on stack
 *
 * @param stack Pointer to stack.
 * @return Returns first element on stack
 */
StackItem *pStack_top(pStack *stack);


/**
 * Function to determine if stack is empty
 *
 * @param stack Pointer to stack.
 * @return Returns true or false
 */
bool pStack_empty(pStack *stack);


/**
 * Function to free stack
 *
 * @param stack Pointer to stack.
 */
void pStack_free(pStack *stack);
#endif