//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author:  Michal Wagner (xwagne12)                    //
//         author:  Adam Kala (xkalaa00)                        //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "pstack.h"


unsigned int size = 0;

void pStack_init(pStack *stack){
    stack->first_item = NULL;
}

bool pStack_push(pStack *stack, ExpElement *elem){
    StackItem *new = ((StackItem*)malloc(sizeof(StackItem)));
    if(new != NULL){
        new->element = elem;
        new->next_element = stack->first_item;
        stack->first_item = new;
        size++;
        return true;
    }
    return false;
}

bool pStack_pop(pStack *stack){
    if(!pStack_empty(stack)){
        StackItem *del = stack->first_item;
        stack->first_item = stack->first_item->next_element;
        free(del);
        size--;
        return true;
    }
    else{
        return false;
    }
}

void pStack_pop3(pStack *stack){
    pStack_pop(stack);
    pStack_pop(stack);
    pStack_pop(stack);
}

StackItem *pStack_get_3(pStack *stack){
    if(size >= 3){
        return stack->first_item->next_element->next_element;
    }
    return NULL;
}

StackItem *pStack_get_2(pStack *stack){
    if(size >= 2){
        return stack->first_item->next_element;
    }
    return NULL;
}

StackItem *pStack_top(pStack *stack){
    if(!pStack_empty(stack)){
        return stack->first_item;
    }
    return NULL;
    //return stack->items[stack->top];
}

bool pStack_empty(pStack *stack){
    return stack->first_item == NULL;
}

void pStack_free(pStack *stack){
    while(stack->first_item != NULL){
        pStack_pop(stack);
    }
}