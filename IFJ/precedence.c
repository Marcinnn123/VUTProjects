//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author:  Michal Wagner (xwagne12)                    //
//         author:  Adam Kala (xkalaa00)                        //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "precedence.h"
#include "parse.h"

token new_token;
StackItem *first_terminal; //top terminal on stack
bool questionM = false;

extern bool inReturn;

extern mystring op1, op2, op3;
TNode *var;
extern Data *data;

const char precedence_Table[16][16] = {
//Entry
        // +   -   *   /   (   )   !   ==  !=  <   >   <=  >=  ??  i   $  Stack
        {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// +
        {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// -
        {'>', '>', '>', '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// *
        {'>', '>', '>', '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// /
        {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '_'},// (
        {'>', '>', '>', '>', '_', '>', '>', '>', '>', '>', '>', '>', '>', '>', '_', '>'},// )
        {'>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// !
        {'<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// ==
        {'<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// !=
        {'<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// <
        {'<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// >
        {'<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// <=
        {'<', '<', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>'},// >=
        {'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>'},// ??
        {'>', '>', '>', '>', '_', '>', '>', '>', '>', '>', '>', '>', '>', '>', '_', '>'},// i
        {'<', '<', '<', '<', '<', '_', '<', '<', '<', '<', '<', '<', '<', '<', '<', '_'},// $
};


char prec_op(token new_token) {
    int pt_index = ConvertToken(new_token);
    int stack_index = first_terminal->element->index;
    if (pt_index != pOther) {
        // get precedence operation
        return precedence_Table[stack_index][pt_index];
    } else {
        return '_';
    }
}

ExpElement *NewElement(mystring *str, int index, int type, TNode *variable, bool is_var, bool qw) {
    ExpElement *new_elem = (ExpElement *) malloc(sizeof(ExpElement));

    if (new_elem != NULL) {
        // setup needed data about new_element
        new_elem->token.content.str = str;
        new_elem->token.type = type;
        if (variable != NULL) {
            new_elem->token.content.str->symbol = variable->content->symbol;
        }
        new_elem->token.qw = qw;
        new_elem->index = index;
        new_elem->active_area = false;
        new_elem->terminal = true;
        new_elem->is_var = is_var;
        return new_elem;
    } else {
        return NULL;
    }
}

int ExpStack(pStack *stack, TNode *variable) {
    pStack_init(stack);
    ExpElement *new_element;

    mystring first_string;
    mystrInit(&first_string);
    mystrAppendChar(&first_string, '$');
    // create ending element and push to stack
    if ((new_element = NewElement(&first_string, pDollar, 15, variable, false, false)) != NULL) {
        if (!pStack_push(stack, new_element)) {
            return INTERNAL_ERROR;
        }
        first_terminal = pStack_top(stack);
        return OK;
    } else {
        return INTERNAL_ERROR;
    }
}


int ConvertToken(token token) {
    switch (token.type) {
        case TYPE_MULTIPLY:
            return pMultiply;
        case TYPE_DIVIDE:
            return pDivide;
        case TYPE_PLUS:
            return pAdd;
        case TYPE_MINUS:
            return pMinus;
        case TYPE_LEFT_BRACKET:
            return pLeftBracket;
        case TYPE_RIGHT_BRACKET:
            return pRightBracket;
        case TYPE_EXCLAMATION:
            return pExclamation;
        case TYPE_EQUAL:
            return pEqual;
        case TYPE_NOT_EQUAL:
            return pNotEqual;
        case TYPE_SMALLER:
            return pSmaller;
        case TYPE_BIGGER:
            return pBigger;
        case TYPE_SMALLER_EQ:
            return pSmallerEq;
        case TYPE_BIGGER_EQ:
            return pBiggerEq;
        case TYPE_DOUBLE_QWMARK:
            return pDoubleQwmark;
        case TYPE_EXPONENT:
            return pID;
        case TYPE_INTEGER:
            return pID;
        case TYPE_DOUBLE:
            return pID;
        case TYPE_STRING:
            return pID;
        case TYPE_VARIABLE:
            return pID;
        case KEYWORD_NIL:
            return pID;
        default:
            return pDollar;
    }
    return pOther;
}

int checkTwoOperandsOrVars(pStack *stack, token token) {
    int converted_token = ConvertToken(token);
    // if new token is operator there cant be another operator
    if (converted_token == pAdd || converted_token == pMinus || converted_token == pMultiply ||
        converted_token == pDivide || converted_token == pEqual ||
        converted_token == pNotEqual || converted_token == pSmaller || converted_token == pBigger ||
        converted_token == pSmallerEq || converted_token == pBiggerEq ||
        converted_token == pDoubleQwmark || converted_token == pExclamation) {
        if (stack->first_item->element->index == pAdd ||
            stack->first_item->element->index == pMinus ||
            stack->first_item->element->index == pMultiply ||
            stack->first_item->element->index == pDivide ||
            stack->first_item->element->index == pEqual ||
            stack->first_item->element->index == pNotEqual ||
            stack->first_item->element->index == pSmaller ||
            stack->first_item->element->index == pBigger ||
            stack->first_item->element->index == pSmallerEq ||
            stack->first_item->element->index == pBiggerEq ||
            stack->first_item->element->index == pExclamation ||
            stack->first_item->element->index == pDoubleQwmark) {
            return SYNTAX_ERROR;
        }
        return OK;
        //if new token is operand there cant be another operand
    } else if (converted_token == pID) {
        if (stack->first_item->element->index == pID) {
            return SYNTAX_ERROR;
        }
        return OK;
    }
    return OK;
}

int CheckExpression(pStack *stack, int op, int root_variable_type, bool in_while_or_if) {
    if (stack != NULL) {
        StackItem *firstn; // first operand
        StackItem *operator;
        StackItem *third; // second operand

        if (op == pExclamation) {
            firstn = pStack_get_2(stack); // operand
            operator = pStack_top(stack); // !
            third = pStack_get_3(stack);
            // check if firstn is operand
            if ((firstn->element->index != pID) || (firstn->element->terminal != false)) {
                return SYNTAX_ERROR;
            }
        } else {
            firstn = pStack_top(stack);
            operator = pStack_get_2(stack);
            third = pStack_get_3(stack);

            // Check if there are operands in variables.
            if ((firstn->element->index != pID && firstn->element->terminal != false) ||
                (third->element->index != pID && third->element->terminal != false)) {
                return UNDEFINED_VARIABLE_ERROR;
            }
        }
        // Check if there is operator in variable.
        if (operator->element->terminal != true || operator->element->index != op) {
            return SYNTAX_ERROR;
        }
        if (op == pMultiply || op == pMinus) {
            if (firstn->element->token.type == third->element->token.type) {
                // operands have same type but root variable is different type
                if ((root_variable_type != firstn->element->token.type) ||
                    (root_variable_type != third->element->token.type)) {
                    if(root_variable_type == TYPE_INTEGER){
                        // operands of type double can't be asserted to variable with type integer
                        if(third->element->token.type == TYPE_DOUBLE){
                            return ARITMETIC_EXPRESSION_ERROR;
                        }
                        if(firstn->element->token.type == TYPE_DOUBLE){
                            return ARITMETIC_EXPRESSION_ERROR;
                        }
                    }
                    else if(root_variable_type == TYPE_DOUBLE){
                        // convert integer to double but not if the operand is variable
                        if(firstn->element->token.type == TYPE_INTEGER){
                            if(firstn->element->is_var){
                                return ARITMETIC_EXPRESSION_ERROR;
                            }
                            firstn->element->token.type = TYPE_DOUBLE;

                            op2.str = "EXP";
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                            setInstruction(0, pops, POPS, &op2, NULL, NULL);
                            setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                            symbolClear(&op1);
                            op2.str = "EXP";
                            setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                        }
                        if(third->element->token.type == TYPE_INTEGER){
                            // convert integer to double but not if the operand is variable
                            if(third->element->is_var){
                                return ARITMETIC_EXPRESSION_ERROR;
                            }
                            third->element->token.type = TYPE_DOUBLE;

                            op2.str = "EXP";
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                            setInstruction(0, pops, POPS, &op2, NULL, NULL);
                            setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                            symbolClear(&op1);
                            op2.str = "EXP";
                            setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                        }
                    }
                    else if(root_variable_type == TYPE_STRING){
                        // this operation cannot produce string
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                }
            }
            if (firstn->element->token.type == TYPE_STRING || third->element->token.type == TYPE_STRING) {
                // this operation cannot operate with strings
                return ARITMETIC_EXPRESSION_ERROR;
            }
            if (root_variable_type == TYPE_STRING) {
                // this operation cannot be saved into type string
                return ARITMETIC_EXPRESSION_ERROR;
            }
            if (firstn->element->token.type == TYPE_DOUBLE && third->element->token.type == TYPE_INTEGER) {
                // we cant assert double to variable with different type
                if (root_variable_type != TYPE_DOUBLE && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we cant change type of variable
                if (third->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }

                third->element->token.type = TYPE_DOUBLE;

                op2.str = "EXP";
                data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                setInstruction(0, pops, POPS, &op2, NULL, NULL);
                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                symbolClear(&op1);
                op2.str = "EXP";
                setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
            }
            if (firstn->element->token.type == TYPE_INTEGER && third->element->token.type == TYPE_DOUBLE) {
                // we cant assert double to variable with different type
                if (root_variable_type != TYPE_DOUBLE && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we cant change type of variable
                if (firstn->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                firstn->element->token.type = TYPE_DOUBLE;

                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
            }
        } else if (op == pAdd) {
            if (firstn->element->token.type == third->element->token.type) {
                // operands have same type but root variable is different type
                if ((root_variable_type != firstn->element->token.type) ||
                    (root_variable_type != third->element->token.type)) {
                    if(root_variable_type == TYPE_INTEGER){
                        // operands of type double can't be asserted to variable with type integer
                        if(third->element->token.type == TYPE_DOUBLE){
                            return ARITMETIC_EXPRESSION_ERROR;
                        }
                        if(firstn->element->token.type == TYPE_DOUBLE){
                            return ARITMETIC_EXPRESSION_ERROR;
                        }
                    }
                    else if(root_variable_type == TYPE_DOUBLE){
                        // convert integer to double but not if the operand is variable
                        if(firstn->element->token.type == TYPE_INTEGER){
                            if(firstn->element->is_var){
                                return ARITMETIC_EXPRESSION_ERROR;
                            }
                            firstn->element->token.type = TYPE_DOUBLE;

                            op2.str = "EXP";
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                            setInstruction(0, pops, POPS, &op2, NULL, NULL);
                            setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                            symbolClear(&op1);
                            op2.str = "EXP";
                            setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                        }
                        if(third->element->token.type == TYPE_INTEGER){
                            // convert integer to double but not if the operand is variable
                            if(third->element->is_var){
                                return ARITMETIC_EXPRESSION_ERROR;
                            }
                            third->element->token.type = TYPE_DOUBLE;

                            op2.str = "EXP";
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                            setInstruction(0, pops, POPS, &op2, NULL, NULL);
                            setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                            symbolClear(&op1);
                            op2.str = "EXP";
                            setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                        }
                    }
                    else if(root_variable_type == TYPE_STRING && (firstn->element->token.type != TYPE_STRING ||
                            third->element->token.type != TYPE_STRING)){
                        // to concat we need operands to be type of string
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                }
            }
            if (firstn->element->token.type == TYPE_STRING && third->element->token.type == TYPE_STRING) {
                // we cant assert string to variable with different type
                if (root_variable_type != TYPE_STRING && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }

                op1.str = "EXP";
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

                if(third->element->is_var){
                    // find the variable
                    if (data->inFunction) {
                        if ((var = BVSSearch(data->local_variable->rootPtr, third->element->token)) == NULL) {
                            return UNDEFINED_VARIABLE_ERROR;
                        }
                    } else {
                        if ((var = BVSSearch(data->rootVar->rootPtr, third->element->token)) == NULL) {
                            return UNDEFINED_VARIABLE_ERROR;
                        }
                    }
                    op2.str = var->content->str;
                    data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                } else {
                    op2.str = third->element->token.content.str->str;
                    op2.symbol = SYMBOL_STRING;
                }

                if(firstn->element->is_var){
                    // find the variable
                    if (data->inFunction) {
                        if ((var = BVSSearch(data->local_variable->rootPtr, firstn->element->token)) == NULL) {
                            return UNDEFINED_VARIABLE_ERROR;
                        }
                    } else {
                        if ((var = BVSSearch(data->rootVar->rootPtr, firstn->element->token)) == NULL) {
                            return UNDEFINED_VARIABLE_ERROR;
                        }
                    }
                    op3.str = var->content->str;
                    data->inFunction ? (op3.symbol = SYMBOL_LF) : (op3.symbol = SYMBOL_GF);
                } else {
                    op3.str = firstn->element->token.content.str->str;
                    op3.symbol = SYMBOL_STRING;
                }
                // if we use concat on two strings
                if(op2.length == 0){
                    setInstruction(0, concat, CONCAT, &op1, &op2, &op3);
                    op2.length++;
                } else { // if we use concat on more strings
                    setInstruction(0, concat, CONCAT, &op1,  &op1, &op3);
                }

                setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
                setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                // if variable we are assigning to doesn't have type
                if (var->content->variable_type == 0 && var->content->variable_type == TYPE_STRING){
                   updateBVS(data->variable, TYPE_STRING,0);
                }
                data->concatenate = true;
                return OK;
            }
            if (firstn->element->token.type == TYPE_DOUBLE && third->element->token.type == TYPE_INTEGER) {
                // we cant assert double to variable with different type
                if (root_variable_type != TYPE_DOUBLE && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we cant change type of variable
                if (third->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }

                third->element->token.type = TYPE_DOUBLE;

                op2.str = "EXP";
                data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                setInstruction(0, pops, POPS, &op2, NULL, NULL);
                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                symbolClear(&op1);
                op2.str = "EXP";
                setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
            }
            if (firstn->element->token.type == TYPE_INTEGER && third->element->token.type == TYPE_DOUBLE) {
                // we cant assert double to variable with different type
                if (root_variable_type != TYPE_DOUBLE && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we cant change type of variable
                if (firstn->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                firstn->element->token.type = TYPE_DOUBLE;

                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
            }
            // both operands need to be string
            if ((firstn->element->token.type == TYPE_STRING && third->element->token.type != TYPE_STRING) ||
                (firstn->element->token.type != TYPE_STRING && third->element->token.type == TYPE_STRING)) {
                return ARITMETIC_EXPRESSION_ERROR;
            }


        } else if (op == pDivide) {
            if (firstn->element->token.type == third->element->token.type) {
                // operands have same type but root variable is different type
                if ((root_variable_type != firstn->element->token.type) ||
                    (root_variable_type != third->element->token.type)) {
                    if(root_variable_type == TYPE_INTEGER){
                        // operands of type double can't be asserted to variable with type integer
                        if(third->element->token.type == TYPE_DOUBLE){
                            return ARITMETIC_EXPRESSION_ERROR;
                        }
                        if(firstn->element->token.type == TYPE_DOUBLE){
                            return ARITMETIC_EXPRESSION_ERROR;
                        }
                    }
                    else if(root_variable_type == TYPE_DOUBLE){
                        // convert integer to double but not if the operand is variable
                        if(firstn->element->token.type == TYPE_INTEGER){
                            if(firstn->element->is_var){
                                return ARITMETIC_EXPRESSION_ERROR;
                            }
                            firstn->element->token.type = TYPE_DOUBLE;

                            op2.str = "EXP";
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                            setInstruction(0, pops, POPS, &op2, NULL, NULL);
                            setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                            symbolClear(&op1);
                            op2.str = "EXP";
                            setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                        }
                        if(third->element->token.type == TYPE_INTEGER){
                            // convert integer to double but not if the operand is variable
                            if(third->element->is_var){
                                return ARITMETIC_EXPRESSION_ERROR;
                            }
                            third->element->token.type = TYPE_DOUBLE;

                            op2.str = "EXP";
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                            setInstruction(0, pops, POPS, &op2, NULL, NULL);
                            setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                            symbolClear(&op1);
                            op2.str = "EXP";
                            setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                        }
                    }
                    else if(root_variable_type == TYPE_STRING){
                        // this operation cannot produce string
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                }
            }
            if (firstn->element->token.type == TYPE_STRING && third->element->token.type == TYPE_STRING) {
                // this operation cannot operate with strings
                return ARITMETIC_EXPRESSION_ERROR;
            }
            if (root_variable_type == TYPE_STRING) {
                // this operation cannot be saved into type string
                return ARITMETIC_EXPRESSION_ERROR;
            }
            // if we are dividing by zero
            if (strcmp(firstn->element->token.content.integerNumber->str, "0") == 0 ||
                strcmp(firstn->element->token.content.doubleNumber->str, "0.0") == 0) {
                return ARITMETIC_EXPRESSION_ERROR;
            }

            if (firstn->element->token.type == TYPE_DOUBLE && third->element->token.type == TYPE_INTEGER) {
                // we cant assert double to variable with different type
                if (root_variable_type != TYPE_DOUBLE && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we cant change type of variable
                if (third->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                third->element->token.type = TYPE_DOUBLE;
                op2.str = "EXP";
                data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                setInstruction(0, pops, POPS, &op2, NULL, NULL);
                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                symbolClear(&op1);
                op2.str = "EXP";
                setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
            }
            if (firstn->element->token.type == TYPE_INTEGER && third->element->token.type == TYPE_DOUBLE) {
                // we cant assert double to variable with different type
                if (root_variable_type != TYPE_DOUBLE && root_variable_type != 0) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we cant change type of variable
                if (firstn->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                firstn->element->token.type = TYPE_DOUBLE;
                op2.str = third->element->token.content.str->str;
                op2.symbol = SYMBOL_DOUBLE;

                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
            }
        } else if (op <= pBiggerEq && op >= pSmaller) {
            // we cant compare nil
            if ((firstn->element->token.type == KEYWORD_NIL) || (third->element->token.type == KEYWORD_NIL)) {
                return ARITMETIC_EXPRESSION_ERROR;
            } else if (firstn->element->token.type != TYPE_STRING && third->element->token.type != TYPE_STRING) {
                if (firstn->element->token.type == TYPE_DOUBLE && third->element->token.type == TYPE_INTEGER) {
                    // we cant convert type of variable
                    if (third->element->is_var) {
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                    third->element->token.type = TYPE_DOUBLE;

                    op2.str = "EXP";
                    data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                    setInstruction(0, pops, POPS, &op2, NULL, NULL);
                    setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                    symbolClear(&op1);
                    op2.str = "EXP";
                    setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
                }
                if (firstn->element->token.type == TYPE_INTEGER && third->element->token.type == TYPE_DOUBLE) {
                    // we cant convert type of variable
                    if (firstn->element->is_var) {
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                    firstn->element->token.type = TYPE_DOUBLE;

                    setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                }
            }
            // we cant compare type string with different type
            else if (firstn->element->token.type != TYPE_STRING && third->element->token.type == TYPE_STRING) {
                return ARITMETIC_EXPRESSION_ERROR;
            }
            else if (firstn->element->token.type == TYPE_STRING && third->element->token.type != TYPE_STRING) {
                return ARITMETIC_EXPRESSION_ERROR;
            }

        } else if (op == pEqual || op == pNotEqual) {
            if ((firstn->element->is_var == true) && (third->element->is_var == true)) {
                // if both operands are variables they need to be same of type
                if (firstn->element->token.type != third->element->token.type) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
            }
            if (firstn->element->token.type == TYPE_DOUBLE && third->element->token.type == TYPE_INTEGER) {
                // we cant convert type of variable
                if (third->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }

                third->element->token.type = TYPE_DOUBLE;

                op2.str = "EXP";
                data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                setInstruction(0, pops, POPS, &op2, NULL, NULL);
                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
                symbolClear(&op1);
                op2.str = "EXP";
                setInstruction(0, pushs, PUSHS, &op2, NULL, NULL);
            }
            if (firstn->element->token.type == TYPE_INTEGER && third->element->token.type == TYPE_DOUBLE) {
                // we cant convert type of variable
                if (firstn->element->is_var) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                firstn->element->token.type = TYPE_DOUBLE;

                setInstruction(0, int2floats, INT2FLOATS, NULL, NULL, NULL);
            }
            // we cant compare type string with different type
            if ((firstn->element->token.type == TYPE_STRING && third->element->token.type != TYPE_STRING) ||
                       (firstn->element->token.type != TYPE_STRING && third->element->token.type == TYPE_STRING)) {
                return ARITMETIC_EXPRESSION_ERROR;
            }
        } else if (op == pExclamation) {
            // if there is let variable
            if (firstn->element->is_var == true && firstn->element->token.content.str->symbol == KEYWORD_LET){
                return ARITMETIC_EXPRESSION_ERROR;
            }
            if (firstn->element->token.type == KEYWORD_NIL) {
                return ARITMETIC_EXPRESSION_ERROR;
            }

        } else if (op == pDoubleQwmark) {
            if (third->element->token.type != KEYWORD_NIL) {
                if (firstn->element->token.type == TYPE_STRING && third->element->token.type != TYPE_STRING) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
            } else if (third->element->token.type == KEYWORD_NIL && firstn->element->token.type == TYPE_STRING) {
                return ARITMETIC_EXPRESSION_ERROR;
            } else if ((firstn->element->token.type != third->element->token.type) && questionM == true) {
                return ARITMETIC_EXPRESSION_ERROR;
            }
        } else {
            return SYNTAX_ERROR;
        }
        return OK;
    } else {
        return SYNTAX_ERROR;
    }
}

int ReduceBracket(pStack *stack) {
    if (stack != NULL) {
        StackItem *second = pStack_get_2(stack); // left bracket
        StackItem *third = pStack_get_3(stack); // element/empty
        StackItem *firstI = pStack_top(stack); // right bracket
        ExpElement *save = second->element; // save the element between brackets


        if (firstI->element->index == pRightBracket && second->element->index == pLeftBracket) {
            // if there is only ()
            pStack_pop(stack);
            pStack_pop(stack);
            first_terminal = pStack_top(stack);
        } else if (second->element->terminal == false && third->element->index == pLeftBracket &&
                   firstI->element->index == pRightBracket) {
            // pop brackets and element
            pStack_pop3(stack);
            pStack_push(stack, save); // push element back
            if (stack->first_item->next_element->element->terminal == true) {
                first_terminal = stack->first_item->next_element;
                first_terminal->element->active_area = false;
            } else {
                return SYNTAX_ERROR;
            }
        } else {
            return SYNTAX_ERROR;
        }
        return OK;
    } else {
        return SYNTAX_ERROR;
    }
}

int ReduceExpression(pStack *stack, TokenQueue *queue, bool assign, int root_variable_type, bool in_while_or_if) {
    int result = OK;
    if (stack != NULL) {
        // operand is nil and operator is ??
        if(questionM == false && new_token.type == TYPE_DOUBLE_QWMARK && stack->first_item->element->token.type == KEYWORD_NIL){
            token tmp_var = getFront(queue);
            if(isalpha(tmp_var.content.str->str[0])){ // if we have number, string or var
                if (data->inFunction) {
                    // get variable if doesnt exists -> type to string
                    if (BVSSearch(data->local_variable->rootPtr, tmp_var) == NULL) {
                        tmp_var.type = TYPE_STRING;
                    } else {
                        tmp_var.type = TYPE_VARIABLE;
                    }
                } else {
                    if (BVSSearch(data->rootVar->rootPtr, tmp_var) == NULL) {
                        tmp_var.type = TYPE_STRING;
                    } else {
                        tmp_var.type = TYPE_VARIABLE;
                    }
                }
            }
            questionM = true;
            if(tmp_var.type == TYPE_VARIABLE){ // get type for codegen
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
            } else if(tmp_var.type == TYPE_INTEGER) {
                op1.symbol = SYMBOL_INTEGER;
            } else if(tmp_var.type == TYPE_DOUBLE) {
                op1.symbol = SYMBOL_DOUBLE;
            } else if(tmp_var.type == TYPE_STRING) {
                op1.symbol = SYMBOL_STRING;
            }
            op1.str = tmp_var.content.str->str;
            setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
        } else if(questionM == false && new_token.type == TYPE_DOUBLE_QWMARK && stack->first_item->element->token.type != KEYWORD_NIL){
            // if there is no nil
            questionM = true;
            if(isalpha(stack->first_item->element->token.content.str->str[0])){
                if (data->inFunction) {
                    if (BVSSearch(data->local_variable->rootPtr, stack->first_item->element->token) == NULL) {
                        stack->first_item->element->token.type = TYPE_STRING;
                    } else {
                        stack->first_item->element->token.type = TYPE_VARIABLE;
                    }
                } else {
                    if (BVSSearch(data->rootVar->rootPtr, stack->first_item->element->token) == NULL) {
                        stack->first_item->element->token.type = TYPE_STRING;
                    } else {
                        stack->first_item->element->token.type = TYPE_VARIABLE;
                    }
                }
            }
            if(stack->first_item->element->token.type == TYPE_VARIABLE){
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
            } else if(stack->first_item->element->token.type == TYPE_INTEGER) {
                op1.symbol = SYMBOL_INTEGER;
            } else if(stack->first_item->element->token.type == TYPE_DOUBLE) {
                op1.symbol = SYMBOL_DOUBLE;
            } else if(stack->first_item->element->token.type == TYPE_STRING) {
                op1.symbol = SYMBOL_STRING;
            }
            op1.str = stack->first_item->element->token.content.str->str;
            setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
        }
        if (first_terminal->element->index == pID) {
            stack->first_item->element->terminal = false;
            stack->first_item->element->active_area = false;
            stack->first_item->next_element->element->active_area = false;
            first_terminal = pStack_get_2(stack);

            // if there is nil but no operand ??
            if (root_variable_type == 0 && stack->first_item->element->token.type == KEYWORD_NIL && questionM == false) {
                return NOT_SPECIFIED_ERROR;
            }
            // if there is type of variable and variable doesnt have suffix ?
            if (root_variable_type != 0 && stack->first_item->element->token.qw == false) {
                // variable is nil
                if (stack->first_item->element->token.type == KEYWORD_NIL && questionM == false) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
            }
            if (stack->first_item->element->token.type == TYPE_INTEGER) {
                if(in_while_or_if && assign){
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we are assigning only one value
                else if ((assign == true) && (inReturn == false)) {
                    if (root_variable_type == 0) {
                        // variable has no type -> give it type
                        root_variable_type = stack->first_item->element->token.type;
                        updateBVS(data->variable, stack->first_item->element->token.type,
                                  stack->first_item->element->token.qw);
                    }
                    if (root_variable_type != TYPE_INTEGER && root_variable_type != TYPE_DOUBLE) {
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                }
                // we are not working with operand ??
                if(questionM == false){
                    if (stack->first_item->element->is_var == true) {
                        // if operand is variable get type for codegen
                        data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                        op1.str = stack->first_item->element->token.content.str->str;
                        setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                    } else {
                        if (data->variable->content->variable_type == TYPE_DOUBLE) {
                            op1.symbol = SYMBOL_DOUBLE;
                        } else {
                            op1.symbol = SYMBOL_INTEGER;
                        }
                        op1.str = stack->first_item->element->token.content.str->str;
                        setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                    }
                }
            } else if (stack->first_item->element->token.type == TYPE_DOUBLE) {
                if(in_while_or_if && assign){
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                else if ((assign == true) && (inReturn == false)) {
                    if (root_variable_type == 0) {
                        root_variable_type = stack->first_item->element->token.type;
                        updateBVS(data->variable, stack->first_item->element->token.type,
                                  stack->first_item->element->token.qw);
                    }
                    if (root_variable_type != TYPE_INTEGER && root_variable_type != TYPE_DOUBLE) {
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                }
                // we are not working with operand ??
                if(questionM == false) {
                    // we are assigning only one value
                    if (stack->first_item->element->is_var == true) {
                        // variable has no type -> give it type
                        data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                        op1.str = stack->first_item->element->token.content.str->str;
                        setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                    } else {
                        op1.symbol = SYMBOL_DOUBLE;
                        op1.str = stack->first_item->element->token.content.str->str;
                        setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                    }
                }
            } else if (stack->first_item->element->token.type == TYPE_STRING) {
                if(in_while_or_if && assign){
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                // we are assigning only one value
                else if ((assign == true) && (inReturn == false)) {
                    if (root_variable_type == 0) {
                        // variable has no type -> give it type
                        root_variable_type = stack->first_item->element->token.type;
                        updateBVS(data->variable, stack->first_item->element->token.type,
                                  stack->first_item->element->token.qw);
                    }
                    // string needs to be in string
                    if (root_variable_type != TYPE_STRING) {
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                    if(in_while_or_if){
                        return ARITMETIC_EXPRESSION_ERROR;
                    }
                }
                // we are not working with operand ??
                if(questionM == false) {
                    if (stack->first_item->element->is_var == true) { // get type for codegen
                        data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                        op1.str = stack->first_item->element->token.content.str->str;
                        setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                    } else {
                        op1.symbol = SYMBOL_STRING;
                        op1.str = stack->first_item->element->token.content.str->str;
                        setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                    }
                }
            } else if (stack->first_item->element->token.type == KEYWORD_NIL) {
                if(questionM == false) {
                    op1.str = stack->first_item->element->token.content.str->str;
                    data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                    setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                }
            } else if (stack->first_item->element->token.type == TYPE_EXPONENT) {
                if(questionM == false) {
                    // Creates double from exponent
                    double result = strtod(stack->first_item->element->token.content.str->str, NULL);

                    // Gets the lenght of the number
                    size_t resultLength = strlen(stack->first_item->element->token.content.str->str);

                    // Creates temporary buffer for the number
                    // +1 for the \0
                    char buffer[resultLength + 1];

                    // The double is converted again to the string
                    snprintf(buffer, sizeof(buffer), "%f", result);


                    // Saves the string to our stack token
                    strcpy(stack->first_item->element->token.content.str->str, buffer);
                    stack->first_item->element->token.type = TYPE_DOUBLE;

                    // Saves it as a double number
                    op1.symbol = SYMBOL_DOUBLE;
                    op1.str = stack->first_item->element->token.content.str->str;
                    setInstruction(0, pushs, PUSHS, &op1, NULL, NULL);
                }
            }
            return OK;
        } else if (first_terminal->element->index == pMultiply) {
            if ((result = CheckExpression(stack, pMultiply, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, muls, MULS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pDivide) {
            if ((result = CheckExpression(stack, pDivide, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            // we need to control if instruction is divs or div
            if (data->variable->content->variable_type == TYPE_INTEGER) {
                setInstruction(0, idivs, IDIVS, NULL, NULL, NULL);
            } else if (data->variable->content->variable_type == TYPE_DOUBLE) {
                setInstruction(0, divs, DIVS, NULL, NULL, NULL);
            } else {
                if (stack->first_item->element->token.type == TYPE_INTEGER) {
                    setInstruction(0, idivs, IDIVS, NULL, NULL, NULL);
                } else if (stack->first_item->element->token.type == TYPE_VARIABLE) {
                    if (data->variable->content->variable_type == TYPE_INTEGER) {
                        setInstruction(0, idivs, IDIVS, NULL, NULL, NULL);
                    } else if (data->variable->content->variable_type == TYPE_DOUBLE) {
                        setInstruction(0, divs, DIVS, NULL, NULL, NULL);
                    }
                } else {
                    setInstruction(0, divs, DIVS, NULL, NULL, NULL);
                }
            }
        } else if (first_terminal->element->index == pAdd) {
            if ((result = CheckExpression(stack, pAdd, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            if(!data->concatenate){
                setInstruction(0, adds, ADDS, NULL, NULL, NULL);
            }
            data->concatenate = false;
        } else if (first_terminal->element->index == pMinus) {
            if ((result = CheckExpression(stack, pMinus, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, subs, SUBS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pSmaller) {
            if ((result = CheckExpression(stack, pSmaller, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, lts, LTS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pBigger) {
            if ((result = CheckExpression(stack, pBigger, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, gts, GTS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pEqual) {
            if ((result = CheckExpression(stack, pEqual, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, eqs, EQS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pNotEqual) {
            if ((result = CheckExpression(stack, pNotEqual, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, eqs, EQS, NULL, NULL, NULL);
            setInstruction(0, nots, NOTS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pSmallerEq) {
            if ((result = CheckExpression(stack, pSmallerEq, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, gts, GTS, NULL, NULL, NULL);
            setInstruction(0, nots, NOTS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pBiggerEq) {
            if ((result = CheckExpression(stack, pBiggerEq, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            setInstruction(0, lts, LTS, NULL, NULL, NULL);
            setInstruction(0, nots, NOTS, NULL, NULL, NULL);
        } else if (first_terminal->element->index == pExclamation) {
            if ((result = CheckExpression(stack, pExclamation, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
            first_terminal = pStack_get_3(stack);
            pStack_pop(stack);
            return OK;
        } else if (first_terminal->element->index == pDoubleQwmark) {
            questionM = false;
            if ((result = CheckExpression(stack, pDoubleQwmark, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
        } else if (first_terminal->element->index == pRightBracket) {
            if (ReduceBracket(stack) != OK) {
                return SYNTAX_ERROR;
            }
            return OK;
        }

        pStack_pop(stack);
        pStack_pop(stack);
        first_terminal = pStack_get_2(stack);
        first_terminal->element->active_area = false;
        return result;
    } else {
        return SYNTAX_ERROR;
    }
}

int ReturnType(pStack *stack, int type) {
    TNode *returned_type;
    if (data->inFunction) {
        if (type == TYPE_VARIABLE) {
            // find variable
            if ((returned_type = BVSSearch(data->local_variable->rootPtr, stack->first_item->element->token)) == NULL) {
                return UNDEFINED_VARIABLE_ERROR;
            }
        } else if (type == data->function_type) {
            // check type
            return OK;
        }
    }
    // check if return type is same as expected
    if ((type == TYPE_DOUBLE || type == KEYWORD_DOUBLE) && stack->first_item->element->token.type == TYPE_INTEGER){
        // convert type
        setInstruction(0, int2float, INT2FLOAT, stack->first_item->element->token.content.str, NULL, NULL);
    } else if ((type == TYPE_INTEGER || type == KEYWORD_INT) &&
               stack->first_item->element->token.type != TYPE_INTEGER) {
        return ARITMETIC_EXPRESSION_ERROR;
    }
    if((type == TYPE_DOUBLE || type == KEYWORD_DOUBLE) && stack->first_item->element->token.type == TYPE_STRING){
        return ARITMETIC_EXPRESSION_ERROR;
    }
    if ((type == TYPE_STRING || type == KEYWORD_STRING) && stack->first_item->element->token.type != TYPE_STRING) {
        return ARITMETIC_EXPRESSION_ERROR;
    }
    return OK;
}

int PrecedenceAnalysis(pStack *stack, token token0, TokenQueue *queue, bool in_while_or_if, TNode *variable) {
    ExpStack(stack, variable);    // Init stack
    char operation_sign; // Sign in precedence table
    int result = OK;
    token end_check; // token to check if precedence should end
    bool assign = true; // if we are currently assigning only one value to variable
    int root_variable_type; // type of variable we are assigning to
    bool root_variable_qw; // is variable with ?
    if (variable == NULL) {
        root_variable_type = 0;
        root_variable_qw = false;
    } else {
        root_variable_type = variable->content->variable_type;
        root_variable_qw = variable->content->qw;
    }

    new_token = token0; //Current token

    while (true) {
        operation_sign = prec_op(new_token); // <, >, =

        if (in_while_or_if == true) {
            // precedence must end with right bracket in condition or loop
            if ((first_terminal->element->index == pDollar && ConvertToken(new_token) == pRightBracket) ||
                (first_terminal->element->index == pDollar && ConvertToken(new_token) == pDollar)) {
                end_check = getFront(queue);
                if(end_check.type != TYPE_LEFT_VINCULUM){
                    // check if there is left vinculum next
                    return ARITMETIC_EXPRESSION_ERROR;
                }
                return OK;
            }
        } else if (first_terminal->element->index == pDollar &&
                   ConvertToken(new_token) == pDollar) { // there is $ on stack and in input
            if (inReturn == true) { // we are in return so we need to check return type
                if (ReturnType(stack, data->function_type) != OK) {
                    return FUNCTION_CALL_ERROR;
                }
                inReturn = false;
            }
            if (root_variable_type == 0) { // if variable has no type -> assert type and update variable in tree
                updateBVS(data->variable, stack->first_item->element->token.type, stack->first_item->element->token.qw);
            }
            return OK;
        }
        if (operation_sign == '=') { // reduction of brackets
            int index = ConvertToken(new_token);
            ExpElement *new;
            if (new_token.type == TYPE_VARIABLE) {
                // get variable
                if (data->inFunction) {
                    if ((var = BVSSearch(data->local_variable->rootPtr, new_token)) == NULL) {
                        return UNDEFINED_VARIABLE_ERROR;
                    }
                } else {
                    if ((var = BVSSearch(data->rootVar->rootPtr, new_token)) == NULL) {
                        return UNDEFINED_VARIABLE_ERROR;
                    }
                }
                // create new element
                new = NewElement(new_token.content.str, index, var->content->variable_type, var, true,
                                 root_variable_qw);
            } else {
                new = NewElement(new_token.content.str, index, new_token.type, var, false, root_variable_qw);
            }
            // check if there are two operands or operators behind each other
            if (checkTwoOperandsOrVars(stack, new_token) != OK) {
                return SYNTAX_ERROR;
            }
            pStack_push(stack, new);
            if (ReduceBracket(stack) != OK) {
                return SYNTAX_ERROR;
            }
            end_check = getFront(queue);
            int converted_token = ConvertToken(end_check);
            // multi line expression check of end
            if (queue != NULL && queue->front != NULL && queue->front->next != NULL
                && queue->front->next->data.type == TYPE_ASSIGN
                && stack != NULL && stack->first_item != NULL
                && stack->first_item->element != NULL
                && stack->first_item->element->index == pID) {
                new_token.type = 15; //insert dollar
            } else if ((pAdd <= converted_token) && (converted_token < pDollar)) {
                new_token = dequeue(queue);
            } else {
                new_token = end_check;
            }
        } else if (operation_sign == '<') {
            first_terminal->element->active_area = true;
            int index = ConvertToken(new_token);
            ExpElement *new_element;
            if (new_token.type == TYPE_VARIABLE) {
                // get variable
                if (data->inFunction) {
                    if ((var = BVSSearch(data->local_variable->rootPtr, new_token)) == NULL) {
                        return UNDEFINED_VARIABLE_ERROR;
                    }
                } else {
                    if ((var = BVSSearch(data->rootVar->rootPtr, new_token)) == NULL) {
                        return UNDEFINED_VARIABLE_ERROR;
                    }
                }
                new_element = NewElement(new_token.content.str, index, var->content->variable_type, var, true,
                                         root_variable_qw);
            } else {
                new_element = NewElement(new_token.content.str, index, new_token.type, var, false, root_variable_qw);
            }

            if (checkTwoOperandsOrVars(stack, new_token) != OK) {
                return SYNTAX_ERROR;
            }

            pStack_push(stack, new_element);
            first_terminal = pStack_top(stack);

            end_check = getFront(queue);
            int converted_token = ConvertToken(end_check);
            // multi line expression check of end
            if (queue != NULL && queue->front != NULL && queue->front->next != NULL
                && queue->front->next->data.type == TYPE_ASSIGN
                && stack != NULL && stack->first_item != NULL
                && stack->first_item->element != NULL
                && stack->first_item->element->index == pID) {
                new_token.type = 15; //insert dollar
            } else if ((pAdd <= converted_token) && (converted_token < pDollar)) {
                assign = false;
                // check if there is only operand in brackets in loop or condition
                if(queue != NULL && queue->front->next != NULL){
                    if(converted_token == pRightBracket && in_while_or_if == true &&
                       queue->front->next->data.type == TYPE_LEFT_VINCULUM &&
                       stack != NULL && stack->first_item != NULL &&
                       stack->first_item->element != NULL && stack->first_item->next_element->element->index == pDollar){
                        assign = true;
                    }
                }
                new_token = dequeue(queue);
            } else {
                new_token = end_check;
            }
        } else if (operation_sign == '>') {
            if ((result = ReduceExpression(stack, queue, assign, root_variable_type, in_while_or_if)) != OK) {
                return result;
            }
        } else {
            return SYNTAX_ERROR;
        }
    }
    return SYNTAX_ERROR;
}