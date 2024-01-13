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

#include <stdlib.h>
#include <stdio.h>
#include "scanner.h"
#include "str.h"
#include "codegen.h"
#include "parse.h"
#include <stdbool.h>
#include <string.h>
#include "tokenqueue.h"
#include "precedence.h"
#include "pstack.h"

int result; // stands for error type
bool init = false; // indicates structure's initialization
token activeToken; // token that is being parsed
token function_token; // token that represent's function
token variable_token; // token that represent's variable

mystring op1,op2,op3; // instruction's operands
TNode *variable; // variable stored in BST
TNodef *func; // function stored in BST

pStack *stack; // expression stack

bool inReturn; // indicates expression after return
Data *data; // helping structure with important parameters


int parse(TRootf *rootf, TokenQueue *queue)
{
    // Initialize BST, structures and define variables in global scope
    if (!init)
    {
        // Structure containing important parameters
        Data d;
        data = &d;
        InitData(data);

        // Stack used in expression parser
        pStack pStack;
        stack = &pStack;
        pStack_init(stack);

        // Initialize BST for global/local scope
        TRoot VARroot;
        TRoot VARrootLocal;
        data->rootVar = &VARroot;
        data->local_variable = &VARrootLocal;

        BVSInit(data->rootVar);
        BVSInit(data->local_variable);

        setActiveFunction(queue); // Parse function's body first

        // Initialize structures that represents instruction's operands
        mystrInit(&op1);
        mystrInit(&op2);
        mystrInit(&op3);

        // Print header, important global variables
        setInstruction(0,header,HEADER,NULL,NULL,NULL);
        op1.str = "EXP";
        op1.symbol = SYMBOL_GF;
        setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
        op1.str = "EXP_b";
        op1.symbol = SYMBOL_GF;
        setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
        op1.str = "EXP_s";
        op1.symbol = SYMBOL_GF;
        setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
        op1.str = "EXP_i";
        op1.symbol = SYMBOL_GF;
        setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);

        init = true; // Set initialization indicator
    }

    do
    {
        activeToken = dequeue(queue); // get token from queue
        switch (activeToken.type)
        {
            // Start parsing
            case KEYWORD_FUNC:
            case KEYWORD_IF:
            case KEYWORD_WHILE:
            case KEYWORD_LET:
            case KEYWORD_VAR:
            case TYPE_VARIABLE:
            case TYPE_FUNCTION_NAME:
                result = statement(rootf,queue);
                break;

            default:
                return SYNTAX_ERROR;
        }
        if(result != 0) // Error found in program
        {
            return result;
        }
    }while(!(isEmpty(queue))); // Continue parsing while queue is not empty

    return result; // Return error code or success
}

int statement(TRootf *rootf,TokenQueue *queue)
{
    char* function_name; // Variable for storing FUNCTION'S name
    char* var_name; // Variable for storing VARIABLE'S name
    int type_of_var = activeToken.type; // Helps with token's type
    switch (activeToken.type)
    {
        // EOF or ε
        case TYPE_EOF:
            return OK;

            // 21. <statement> -> func id (<arg>) <type_spec> { <statement> } <prog_con>
        case KEYWORD_FUNC:
            data->inFunction = true; // Indicates that we are in function's body
            data->caller = 0; // Function was called

            activeToken = dequeue(queue);

            function_name = activeToken.content.str->str;
            function_token = activeToken;

            // Check if the next activeToken is ID of the function
            if ((result = fundef(rootf,queue)) != OK)
            {
                return result;
            }
            symbolClear(&op1);
            symbolClear(&op2);

            // Set instruction to jump over function's definition
            op1.str = FUN_END;
            op2.str = function_name;
            setInstruction(0,jump,JUMP,&op1,&op2,NULL);

            symbolClear(&op1);
            symbolClear(&op2);

            // Set function's label with function's name
            op1.str = FUN;
            op2.str = function_name;
            setInstruction(0,label,LABEL,&op1,&op2,NULL);

            // Define important local variables
            op1.str = "EXP";
            op1.symbol = SYMBOL_LF;
            setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
            op1.str = "EXP_s";
            op1.symbol = SYMBOL_LF;
            setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
            op1.str = "EXP_b";
            op1.symbol = SYMBOL_LF;
            setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
            op1.str = "EXP_i";
            op1.symbol = SYMBOL_LF;
            setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);


            activeToken = dequeue(queue); // Next activeToken has to be left bracket according to the rule
            if ((result = Lbracket()) != OK)
            {
                return result;
            }

            // Checking function's arguments
            if ((result = args(rootf,queue,data->caller)) != OK)
            {
                return result;
            }

            activeToken = dequeue(queue);
            if(activeToken.type == TYPE_LEFT_VINCULUM)
            {
                activeToken = dequeue(queue);
            }
            // If theres no return type for no-void function
            if (activeToken.type == TYPE_RIGHT_VINCULUM) {
                if (rootf->rootPtr->return_type != 0) {
                    return RETURN_COUNT_ERROR;
                }
            }
            bool returned = false;
            while (activeToken.type != TYPE_RIGHT_VINCULUM) {
                // Found RETURN token
                if (activeToken.type == KEYWORD_RETURN) {
                    if (returned == true) {
                        return RETURN_COUNT_ERROR;
                    } else {
                        returned = true;
                    }
                }
                // After parsing function's argument, get statement tokens
                if ((result = statement(rootf, queue)) != OK) {
                    return result;
                }
                activeToken = dequeue(queue);
            }
            
            // If the function is non-void and it wasn't returned
            if (rootf->rootPtr->return_type != 0 && returned == false) {
                return RETURN_COUNT_ERROR;
            }

            // Next activeToken has to be right viniculum
            if((result = Rvinculum(queue)) != OK)
            {
                return result;
            }

            // Pop local frame to temporary frame
            setInstruction(0,popframe,POPFRAME,NULL,NULL,NULL);
            // Return to data->caller
            setInstruction(0,return_ins,RETURN,NULL,NULL,NULL);

            symbolClear(&op1);
            symbolClear(&op2);

            // Set label for skipping function's definition
            op1.str = FUN_END;
            op2.str = function_name;
            setInstruction(0,label,LABEL,&op1,&op2,NULL);

            // Dispose local scope
            BVSDispose(data->local_variable);

            // Set activeElement to the FirstElement in queue
            setActiveFunction(queue);

            symbolClear(&op1);
            symbolClear(&op2);
            data->inFunction = false; // closing function's scope
            return OK;

            // 22. id (<arg>) <prog_con>
            // If the id name is function
        case TYPE_FUNCTION_NAME:
            // BUILT - IN functions

            // found write() function
            if(strcmp(activeToken.content.str->str,"write") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in write
                if((result = writeFunc(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found readString() function
            if(strcmp(activeToken.content.str->str,"readString") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in readString
                if((result = readFunc(queue,SYMBOL_STRING)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found readInt() function
            if(strcmp(activeToken.content.str->str,"readInt") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in readInt
                if((result = readFunc(queue,SYMBOL_INTEGER)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found readDouble() function
            if(strcmp(activeToken.content.str->str,"readDouble") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in readDouble
                if((result = readFunc(queue,SYMBOL_DOUBLE)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found Int2Double() function
            if(strcmp(activeToken.content.str->str,"Int2Double") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in Int2Double
                if((result = int2double(queue)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found Double2Int() function
            if(strcmp(activeToken.content.str->str,"Double2Int") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in Double2Int
                if((result = double2int(queue)) != OK)
                {
                    return result;
                }

                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found chr() function
            if(strcmp(activeToken.content.str->str,"chr") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in chr
                if((result = chr(queue)) != OK)
                {
                    return result;
                }

                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }

                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }

            // Found ord() function
            if(strcmp(activeToken.content.str->str,"ord") == 0)
            {
                activeToken = dequeue(queue);
                // Call function for built-in ord
                if((result = ord(queue)) != OK)
                {
                    return result;
                }

                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }

                symbolClear(&op1);
                symbolClear(&op2);
                symbolClear(&op3);
                return OK;
            }

            // Found substring() function
            if(strcmp(activeToken.content.str->str,"substring") == 0)
            {
                activeToken = dequeue(queue);
                if((result = Lbracket()) != OK)
                {
                    return result;
                }
                symbolClear(&op1);
                symbolClear(&op2);
                // Create temporary frame for substring() function
                setInstruction(0,createframe,CREATEFRAME,NULL,NULL,NULL);

                // FIRST ARGUMENT
                activeToken = dequeue(queue);
                // Check for argument's name
                if(strcmp(activeToken.content.str->str,"of") != 0)
                {
                    return SYNTAX_ERROR;
                }
                activeToken = dequeue(queue);
                // Check for colon after name
                if(activeToken.type != TYPE_COLON)
                {
                    return SYNTAX_ERROR;
                }

                // Set instruction for first parameter definition
                op1.str = "param1";
                op1.symbol = SYMBOL_TF;
                setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);

                activeToken = dequeue(queue);
                // Get string from first argument
                if(activeToken.type != TYPE_STRING)
                {
                    if(activeToken.type == TYPE_VARIABLE) // String stored in variable
                    {
                        // Check if variable was defined
                        if(data->inFunction)
                        {
                            if((variable = BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            else{
                                if(variable->content->variable_type != TYPE_STRING)
                                {
                                    return FUNCTION_CALL_ERROR; // Variable does not contain string
                                }

                                // Move this string to first parameter
                                op2.str = variable->content->str;
                                op2.symbol = SYMBOL_LF;
                                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                            }
                        }
                        else{ // Global scope
                            if((variable = BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            else{
                                if(variable->content->variable_type != TYPE_STRING)
                                {
                                    return FUNCTION_CALL_ERROR; // Variable does not contain string
                                }

                                // Move this string to first parameter
                                op2.str = variable->content->str;
                                op2.symbol = SYMBOL_GF;
                                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                            }
                        }

                    }
                    else{
                        return FUNCTION_CALL_ERROR; // Argument type is not a string or variable
                    }
                }
                else{
                    // First argument is a type of string
                    op2.str = activeToken.content.str->str;
                    op2.symbol = SYMBOL_STRING;
                    setInstruction(0,move,MOVE,&op1,&op2,NULL);
                }

                symbolClear(&op1);
                symbolClear(&op2);

                activeToken = dequeue(queue);
                // There has to be comma between argument's
                if(activeToken.type != TYPE_COMMA)
                {
                    return SYNTAX_ERROR;
                }

                // SECOND ARGUMENT
                // Set instruction for second parameter definition
                op1.str = "param2";
                op1.symbol = SYMBOL_TF;
                setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);

                activeToken = dequeue(queue);
                // Check for argument's name
                if(strcmp(activeToken.content.str->str,"startingAt") != 0)
                {
                    return SYNTAX_ERROR;
                }

                activeToken = dequeue(queue);
                // Check for colon after name
                if(activeToken.type != TYPE_COLON)
                {
                    return SYNTAX_ERROR;
                }

                activeToken = dequeue(queue);
                // Second argument has to be integer
                if(activeToken.type != TYPE_INTEGER)
                {
                    if(activeToken.type == TYPE_VARIABLE) // Integer stored in variable
                    {
                        // Check if variable was defined
                        if(data->inFunction)
                        {
                            if((variable = BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            else{
                                if(variable->content->variable_type != TYPE_INTEGER)
                                {
                                    return FUNCTION_CALL_ERROR; // Variable does not contain an integer number
                                }

                                // Move this integer to second parameter
                                op2.str = variable->content->str;
                                op2.symbol = SYMBOL_LF;
                                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                            }
                        }
                        else{ // Global scope
                            if((variable = BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            else{
                                if(variable->content->variable_type != TYPE_INTEGER)
                                {
                                    return FUNCTION_CALL_ERROR; // Variable does not contain an integer number
                                }

                                // Move this integer to second parameter
                                op2.str = variable->content->str;
                                op2.symbol = SYMBOL_GF;
                                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                            }
                        }

                    }
                    else{
                        return FUNCTION_CALL_ERROR; // Variable in not an integer number or variable
                    }
                }
                else{
                    // Second argument is a type of integer number
                    op2.str = activeToken.content.str->str;
                    op2.symbol = SYMBOL_INTEGER;
                    setInstruction(0,move,MOVE,&op1,&op2,NULL);
                }

                activeToken = dequeue(queue);
                // There has to be comma between arguments
                if(activeToken.type != TYPE_COMMA)
                {
                    return SYNTAX_ERROR;
                }

                symbolClear(&op1);
                symbolClear(&op2);

                // THIRD ARGUMENT
                // Set instruction for second parameter definition
                op1.str = "param3";
                op1.symbol = SYMBOL_TF;
                setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);

                activeToken = dequeue(queue);
                // Checks for argument's name
                if(strcmp(activeToken.content.str->str,"endingBefore") != 0)
                {
                    return SYNTAX_ERROR;
                }

                activeToken = dequeue(queue);
                // Check for colon after name
                if(activeToken.type != TYPE_COLON)
                {
                    return SYNTAX_ERROR;
                }

                activeToken = dequeue(queue);
                // Third argument has to be integer
                if(activeToken.type != TYPE_INTEGER)
                {
                    if(activeToken.type == TYPE_VARIABLE) // Integer stored in variable
                    {
                        // Check if variable was defined
                        if(data->inFunction)
                        {
                            if((variable = BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            else{
                                if(variable->content->variable_type != TYPE_INTEGER)
                                {
                                    return FUNCTION_CALL_ERROR; // Variable does not contain integer number
                                }

                                // Move this integer to third parameter
                                op2.str = variable->content->str;
                                op2.symbol = SYMBOL_LF;
                                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                            }
                        }
                        else{ // Global scope
                            if((variable = BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            else{
                                if(variable->content->variable_type != TYPE_INTEGER)
                                {
                                    return FUNCTION_CALL_ERROR; // Variable does not contain an integer numbere
                                }

                                // Move this integer to third parameter
                                op2.str = variable->content->str;
                                op2.symbol = SYMBOL_GF;
                                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                            }
                        }

                    }
                    else{
                        return FUNCTION_CALL_ERROR; // Thitd argument is not an integer number or variable
                    }
                }
                else{
                    // Third argument is an integer number
                    op2.str = activeToken.content.str->str;
                    op2.symbol = SYMBOL_INTEGER;
                    setInstruction(0,move,MOVE,&op1,&op2,NULL);
                }

                symbolClear(&op1);

                // Define variable for storing function's return value and push frame to local frame
                op1.str = RETURN_VALUE;
                op1.symbol = SYMBOL_TF;
                setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
                setInstruction(0,pushframe,PUSHFRAME,NULL,NULL,NULL);
                symbolClear(&op1);
                symbolClear(&op2);

                // Call function
                op1.str = "substring_func";
                setInstruction(SKIP,call,CALL,&op1,NULL,NULL);

                // Print substring's body just once
                if(!data->substring)
                {
                    // Call substring()
                    if((result = substring(queue)) != OK)
                    {
                        return result;
                    }
                }
                data->substring = true;

                activeToken = dequeue(queue);
                // Token after arguments has to be right bracket
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }
                // Move function's return value to the variable
                op1.str = data->variable->content->str;
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                op2.str = RETURN_VALUE;
                op2.symbol = SYMBOL_TF;
                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                symbolClear(&op1);
                symbolClear(&op2);
                symbolClear(&op3);

                // Update variable's type in BST
                updateBVS(data->variable, TYPE_STRING, false);
                return OK;
            }

            // Found length() function
            if(strcmp(activeToken.content.str->str,"length") == 0)
            {
                activeToken = dequeue(queue);
                // Call built-in length
                if((result = strlen_f(queue)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
                // Check for right bracket at the end of the function
                if((result = Rbracket(queue)) != OK)
                {
                    return result;
                }

                // Move function's result to the variable
                op1.str = data->variable->content->str;
                op2.str = "EXP_i";
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                setInstruction(0,move,MOVE,&op1,&op2,NULL);
                symbolClear(&op1);
                symbolClear(&op2);
                return OK;
            }
            // NOT A BUILT - IN function

            data->caller = 1; // This function is caller
            function_name = activeToken.content.str->str;
            if ((result = ID(queue)) != OK) // Following activeToken must be ID
            {
                return result;
            }

            // Create temporary frame
            setInstruction(0,createframe,CREATEFRAME,NULL,NULL,NULL);

            // Checks function's definition
            if((func = BVSSearch_function(rootf->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_FUNCTION_ERROR;
            }

            // Check for left bracket after function's ID
            activeToken = dequeue(queue);
            if((result = Lbracket()) != OK)
            {
                return result;
            }

            // Check function's arguments, move argument's to temporary frame
            if ((result = args(rootf,queue,data->caller)) != OK)
            {
                return result;
            }

            // Variable for function's return value
            op1.str = RETURN_VALUE;
            op1.symbol = SYMBOL_TF;
            setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);

            symbolClear(&op1);
            symbolClear(&op2);

            // Push temporary variables to local frame
            setInstruction(0,pushframe,PUSHFRAME,NULL,NULL,NULL);

            // CALL function
            op1.str = FUN;
            op2.str = function_name;
            setInstruction(0,call,CALL,&op1,&op2,NULL);

            if(func->return_type != 0 && variable != NULL){
                // If function was called after assign, return value will be moved to variable
                op1.str = variable->content->str;
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

                op2.str = RETURN_VALUE; // Function's result is stored here
                op2.symbol = SYMBOL_TF;
                setInstruction(0,move,MOVE,&op1,&op2,NULL);
            }

            symbolClear(&op1);
            symbolClear(&op2);
            return OK;

            // 23. <define> id <assign> <prog_con>
        case KEYWORD_VAR:
        case KEYWORD_LET:

            activeToken = dequeue(queue);
            activeToken.content.str->symbol = type_of_var;

            if ((result = ID(queue)) != OK) // Following activeToken must be ID
            {
                return result;
            }

            // Checks redefinition of variable
            if(data->inFunction)
            {
                if(BVSSearch(data->local_variable->rootPtr,activeToken) != NULL)
                {
                    return UNDEFINED_FUNCTION_ERROR;
                }

                // Insert variable to local BST
                data->token = activeToken;
                data->local_variable->rootPtr = BVSInsert(data->local_variable->rootPtr,data->token);

                // Store this variable to VARIABLE for next use
                variable = BVSSearch(data->local_variable->rootPtr,activeToken);
                data->variable = variable;

            }
            else{
                if(BVSSearch(data->rootVar->rootPtr, activeToken) != NULL)
                {
                    return UNDEFINED_FUNCTION_ERROR;
                }

                // Insert variable to global BST
                data->token = activeToken;
                data->rootVar->rootPtr = BVSInsert(data->rootVar->rootPtr,data->token);

                // Store this variable to VARIABLE for next use
                variable = BVSSearch(data->rootVar->rootPtr,activeToken);
                data->variable = variable;

            }

            var_name = variable->content->str;

            // Set variable for definition
            op1.str = var_name;
            data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

            setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
            symbolClear(&op1);
            symbolClear(&op2);

            activeToken = dequeue(queue);

            // If there is no assign check for variable's type according to the rule
            if (activeToken.type != TYPE_ASSIGN)
            {
                if((result = type(queue,variable)) != OK)
                {
                    return result;
                }

                if(getFront(queue).type != TYPE_ASSIGN)
                {
                    return OK;
                }
                activeToken = dequeue(queue);
            }

            activeToken = dequeue(queue);

            // After assign in not a function
            if(activeToken.type != TYPE_FUNCTION_NAME)
            {
                if(strcmp(activeToken.content.str->str, "nil") == 0 && data->variable->content->variable_type != 0){
                    updateBVS(data->variable, KEYWORD_NIL, data->variable->content->qw);
                }
                // Call Expression() to parse expression after assign
                if((result = Expression(activeToken, queue, false,data,variable)) != OK) {
                    return result;
                }

                op1.str = var_name;
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

                // Store expression's result to variable
                setInstruction(0,pops,POPS,&op1,NULL,NULL);
            }
            else{ // Function called after assign
                if((result = statement(rootf,queue)) != OK)
                {
                    return result;
                }
            }

            symbolClear(&op1);
            symbolClear(&op2);

            return OK;

            // 24. if <define> id {<statement>} <statement> DODELAT ELSE TODO
            // 25. if (<expression>) {<statement>} <else_statement> <prog_con>
        case KEYWORD_IF:
            data->ifCounter++; // Increment ifCounter to have unique if's label name
            data->cnt = data->ifCounter; // Get counter cnt for current if-else

            activeToken = dequeue(queue); // Get first token after keyword if

            switch (activeToken.type)
            {
                case KEYWORD_LET:
                    // RULE: 24
                    activeToken = dequeue(queue);
                    if ((result = ID(queue)) != OK) // Following activeToken must be ID
                    {
                        return result;
                    }

                    // Check if variable was defined
                    if(data->inFunction)
                    {
                        if ((variable = BVSSearch(data->local_variable->rootPtr, activeToken)) == NULL)
                        {
                            return UNDEFINED_VARIABLE_ERROR;
                        }
                    }
                    else{ // global scope
                        if ((variable = BVSSearch(data->rootVar->rootPtr, activeToken)) == NULL)
                        {
                            return UNDEFINED_VARIABLE_ERROR;
                        }
                    }

                    var_name = variable->content->str;

                    // Compare variable's content to nil
                    op3.str = TYPE_NIL;

                    data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                    op2.str = var_name;

                    op1.str = IF; // set IF label

                    setInstruction(data->cnt, jumpifeq, JUMPIFEQ, &op1, &op2, &op3);
                    break;

                case TYPE_LEFT_BRACKET:
                    // RULE: 25
                    activeToken = dequeue(queue); // Skip left bracket before parsing expression

                default:
                    // Calling expression() to parse expression
                    if((result = Expression(activeToken, queue, true,data,variable)) != OK) {
                        return result;
                    }

                    // Pop expression result to EXP_b variable
                    op2.str = "EXP_b";
                    data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                    setInstruction(0,pops,POPS,&op2,NULL,NULL);
                    symbolClear(&op1);
                    symbolClear(&op2);

                    op3.str = BOOL_TRUE; // To compare with expression's result
                    op2.str = "EXP_b";
                    data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                    op1.str = IF; // set IF label
                    // Compare expression's result to bool: true
                    setInstruction(data->cnt, jumpifneq, JUMPIFNEQ, &op1, &op2, &op3);
                    break;
            }

            symbolClear(&op1);
            symbolClear(&op2);

            activeToken = dequeue(queue);
            // Following token must be left viniculum
            if((result = Lvinculum(queue)) != OK)
            {
                return result;
            }

            // If statement
            activeToken = dequeue(queue); // Get first statement token

            // Get all IF statement's tokens
            while(activeToken.type != TYPE_RIGHT_VINCULUM){
                if ((result = statement(rootf,queue)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
            }

            symbolClear(&op1);
            symbolClear(&op2);

            op1.str = ELSE; // set ELSE label
            setInstruction(data->cnt, jump, JUMP, &op1, NULL, NULL); // After if statement, skip else{}

            op1.str = IF; // set IF label
            setInstruction(data->cnt, label, LABEL, &op1, NULL, NULL); // Label for else statement

            // Following token must be right viniculum
            if((result = Rvinculum(queue)) != OK)
            {
                return result;
            }

            symbolClear(&op1);
            symbolClear(&op2);

            // Else statement
            activeToken = dequeue(queue);
            if(activeToken.type != KEYWORD_ELSE)
            {
                return SYNTAX_ERROR; // No else found
            }

            // Parse Else's statement
            if ((result = statement(rootf,queue)) != OK)
            {
                return result;
            }

            return OK;

            // 26. while (<expression>) {<statement>} <prog_con>
        case KEYWORD_WHILE:
            data->whileCounter++; // Increment whileCounter to have unique while names
            data->cnt = data->whileCounter; // Get counter for current loop
            symbolClear(&op1);

            op1.str = WHILE; // Set a while label
            setInstruction(data->cnt,label,LABEL,&op1,NULL,NULL);

            activeToken = dequeue(queue);
            // Skip left bracket before parsing expression
            if(activeToken.type == TYPE_LEFT_BRACKET)
            {
                activeToken = dequeue(queue);
            }

            // Parse expression
            if((result = Expression(activeToken, queue, true,data,variable)) != OK) {
                return result;
            }

            op2.str = "EXP_b"; // Expression() result stored
            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
            setInstruction(0,pops,POPS,&op2,NULL,NULL);
            symbolClear(&op1);

            op1.str = WHILE_END; // Set a while end label

            op3.str = BOOL_TRUE; // To compare with expression's result
            setInstruction(data->cnt,jumpifneq,JUMPIFNEQ,&op1,&op2,&op3);
            symbolClear(&op1);
            symbolClear(&op2);

            activeToken = dequeue(queue);
            // Following token must be left viniculum
            if ((result = Lvinculum(queue)) == OK)
            {
                // Get first statement token
                activeToken = dequeue(queue);

                //Get all statement's tokens
                while(activeToken.type != TYPE_RIGHT_VINCULUM){

                    if ((result = statement(rootf,queue)) != OK)
                    {
                        return result;
                    }
                    activeToken = dequeue(queue);
                }

                // Following token must be right viniculum
                if ((result = Rvinculum(queue)) != OK)
                {
                    return result;
                }
                symbolClear(&op1);

                op1.str = WHILE;
                setInstruction(data->cnt, jump, JUMP, &op1, NULL, NULL);

                op1.str = WHILE_END;
                setInstruction(data->cnt, label, LABEL, &op1, NULL, NULL);
            }
            else
            {
                return result;
            }

            return OK;

        case KEYWORD_ELSE:

            activeToken = dequeue(queue);
            // Following token must be left viniculum
            if((result = Lvinculum(queue)) != OK)
            {
                return result;
            }

            // Get first statement token
            activeToken = dequeue(queue);
            // Get all statement's tokens
            while(activeToken.type != TYPE_RIGHT_VINCULUM){

                if ((result = statement(rootf,queue)) != OK)
                {
                    return result;
                }
                activeToken = dequeue(queue);
            }
            symbolClear(&op1);

            op1.str = ELSE;
            setInstruction(data->cnt, label, LABEL, &op1, NULL, NULL); // Label for skipping else's statement
            data->cnt--;


            return OK;

        case TYPE_VARIABLE:
            // Check if variable was defined in the right scope
            if(data->inFunction)
            {
                if((variable = BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                {
                    return UNDEFINED_VARIABLE_ERROR;
                }
                argList* tmp;
                if(rootf->rootPtr != NULL && variable != NULL){
                    tmp = rootf->rootPtr->arguments;
                    while(tmp != NULL){
                        if(strcmp(variable->content->str, tmp->identifier) == 0){
                            return OTHER_SEMANTIC_ERROR;
                        }
                        tmp = tmp->next;
                    }
                }
            }
            else{ // Global scope
                if((variable = BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                {
                    return UNDEFINED_VARIABLE_ERROR;
                }
            }

            if(variable != NULL && rootf->rootPtr != NULL) {
                if (variable->content->variable_type == TYPE_STRING &&
                    (rootf->rootPtr->return_type != KEYWORD_STRING_QW || rootf->rootPtr->return_type != KEYWORD_STRING)) {
                    return ARITMETIC_EXPRESSION_ERROR;
                } else if (variable->content->variable_type == TYPE_STRING &&
                           (rootf->rootPtr->return_type != KEYWORD_STRING_QW ||
                            rootf->rootPtr->return_type != KEYWORD_STRING)) {
                    return ARITMETIC_EXPRESSION_ERROR;
                } else if (variable->content->variable_type == TYPE_INTEGER &&
                           (rootf->rootPtr->return_type != KEYWORD_INT_QW ||
                            rootf->rootPtr->return_type != KEYWORD_INT)) {
                    return ARITMETIC_EXPRESSION_ERROR;
                } else if (variable->content->variable_type == TYPE_DOUBLE &&
                           (rootf->rootPtr->return_type != KEYWORD_DOUBLE_QW ||
                            rootf->rootPtr->return_type != KEYWORD_DOUBLE)) {
                    return ARITMETIC_EXPRESSION_ERROR;
                }
            }
            // Set parameters to the helping structure
            data->token = activeToken;
            data->variable = variable;

            activeToken = dequeue(queue);
            // Check if there is an assign after variable's ID
            if (activeToken.type != TYPE_ASSIGN)
            {
                return SYNTAX_ERROR;
            }
            var_name = variable->content->str;

            activeToken = dequeue(queue);
            // There is expression after assign
            if(activeToken.type != TYPE_FUNCTION_NAME){
                // Perform expression parsing
                if((result = Expression(activeToken, queue, false,data,variable)) != OK) {
                    return result;
                }
                op1.str = var_name;
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

                // Store expression result to variable
                setInstruction(0,pops,POPS,&op1,NULL,NULL);
            }

            else{ // Function name after assign
                if((result = statement(rootf,queue)) != OK)
                {
                    return result;
                }

            }
            return OK;

            // 28. return <return_value>
        case KEYWORD_RETURN:
            if(!data->inFunction)
            {
                return SYNTAX_ERROR; // Found return in global scope
            }

            inReturn = true;

            // Search for function's parameters
            func = BVSSearch_function(rootf->rootPtr, function_token);
            data->function_type = func->return_type;

            if (func->return_type != 0){
                activeToken = dequeue(queue);
                // Return in non-void function with returning no value
                if((result = Rvinculum(queue)) == OK)
                {
                    return RETURN_COUNT_ERROR;
                }

                // Parse expression after return
                if((result = Expression(activeToken, queue, false,data,variable)) != OK) {
                    return result;
                }

                op1.str = RETURN_VALUE;
                op1.symbol = SYMBOL_LF;
                setInstruction(0,pops,POPS,&op1,NULL,NULL);
            }
            else{
                switch (getFront(queue).type)
                {
                    case TYPE_INTEGER:
                    case TYPE_STRING:
                    case TYPE_DOUBLE:
                    case TYPE_VARIABLE:
                        return SYNTAX_ERROR;
                        break;

                    default:
                        break;
                }
            }

            symbolClear(&op1);
            return OK;

        case TYPE_RIGHT_VINCULUM:
            // Empty statement
            return OK;

        default:
            return SYNTAX_ERROR;
    }

    return OK;
}


// Checking function's ID
int fundef(TRootf *rootf,TokenQueue *queue)
{

    if ((result = ID(queue)) != OK) // Following Token must be ID
    {
        return result;
    }

    return OK;

}

// Checking function's arguments, moving arguments to temporary frame variables
int args(TRootf *rootf,TokenQueue *queue,int caller)
{
    // Get function's parameters
    func = BVSSearch_function(rootf->rootPtr, function_token);
    int paramCounter = 0; // For naming unique parameters
    TNode *var;
    switch (caller) {
        case 0: // Function's body
            do {
                activeToken = dequeue(queue); // Get first token after left bracket or comma
                // Checking if theres _ or some indetifier
                if (activeToken.type == TYPE_UNDERSCORE || activeToken.type == TYPE_VARIABLE) {
                    // Next token has to be variable
                    if(activeToken.type == TYPE_UNDERSCORE){
                        activeToken = dequeue(queue);
                        if (activeToken.type != TYPE_VARIABLE && activeToken.type != TYPE_UNDERSCORE) {
                            return SYNTAX_ERROR; // Token has to be variable
                        }
                    } else {
                        activeToken = dequeue(queue);
                        if (activeToken.type != TYPE_VARIABLE) {
                            return SYNTAX_ERROR; // Name has to be type_variable
                        }
                    }

                    // Check for smilar argument's name and variable's name
                    if(strcmp(func->arguments->name,activeToken.content.str->str) == 0)
                    {
                        return OTHER_SEMANTIC_ERROR;
                    }
                    paramCounter++;

                    // Variable is "_", define parameter as "undefined"
                    if(strcmp(activeToken.content.str->str, "") == 0){
                        op1.str = "undefined";
                    } else {
                        // Define variable
                        op1.str = activeToken.content.str->str;
                    }
                    op1.symbol = SYMBOL_LF;
                    setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
                    symbolClear(&op1);

                    // Move argument to local variable
                    if(strcmp(activeToken.content.str->str, "") == 0){
                        op1.str = "undefined";
                    } else {
                        op1.str = activeToken.content.str->str;
                    }
                    op1.symbol = SYMBOL_LF;

                    op2.str = PARAM;
                    op2.symbol = SYMBOL_LF;
                    setInstruction(paramCounter,move,MOVE,&op1,&op2,NULL);
                    symbolClear(&op1);
                    symbolClear(&op2);

                    // Insert variable into local symtable
                    data->local_variable->rootPtr = BVSInsert(data->local_variable->rootPtr,activeToken);
                    variable = BVSSearch(data->local_variable->rootPtr,activeToken);

                    activeToken = dequeue(queue);
                    // Check if there's colon
                    if (activeToken.type != TYPE_COLON)
                    {
                        return SYNTAX_ERROR;
                    }

                    // Check for the type, set variable's type
                    if ((result = type(queue,variable)) != OK)
                    {
                        return result;
                    }

                    // Right bracket or comma if function has more params
                    activeToken = dequeue(queue);
                    // Check for the right bracket
                    if(activeToken.type == TYPE_RIGHT_BRACKET)
                    {
                        activeToken = dequeue(queue);
                        // Void function
                        if(activeToken.type == TYPE_LEFT_VINCULUM)
                        {
                            return OK;
                        }

                        // Check for the function's return type
                        if((result = type(queue,NULL)) != OK)
                        {
                            return result;
                        }
                        else{
                            return OK;
                        }
                    }
                        // Check if there's comma or right bracket after argument
                    else if(activeToken.type != TYPE_COMMA && activeToken.type != TYPE_RIGHT_BRACKET)
                    {
                        return SYNTAX_ERROR;
                    }
                }
                else // Empty argument list
                {
                    if(activeToken.type == TYPE_RIGHT_BRACKET)
                    {
                        activeToken = dequeue(queue);
                        // Void function
                        if(activeToken.type == TYPE_LEFT_VINCULUM)
                        {
                            return OK;
                        }

                        // Check for function's return type
                        if((result = type(queue,NULL)) != OK)
                        {
                            return result;
                        }
                        else{
                            return OK;
                        }
                    }
                    return SYNTAX_ERROR;
                }
                // Continue while there is another argument in the list
            }while(activeToken.type == TYPE_COMMA);

        case 1: // Function caller
            do{
                activeToken = dequeue(queue); // Get first token after left bracket or comma
                if (activeToken.type == TYPE_VARIABLE || activeToken.type == TYPE_INTEGER || activeToken.type == TYPE_STRING
                    || activeToken.type == TYPE_DOUBLE)
                {
                    // Named parameter
                    if(strcmp(func->arguments->name,"_") != 0)
                    {
                        // Function called with wrong name of parameter
                        if(strcmp(activeToken.content.str->str,func->arguments->name) != 0)
                        {
                            return FUNCTION_CALL_ERROR;
                        }
                    } else{ // Unnamed parameter
                        if(getFront(queue).type == TYPE_COLON)
                        {
                            return OTHER_SEMANTIC_ERROR;
                        }
                    }
                    token tmp = activeToken;
                    activeToken = dequeue(queue);

                    // Check the token between parameters
                    if(activeToken.type != TYPE_COLON && activeToken.type != TYPE_COMMA && activeToken.type != TYPE_RIGHT_BRACKET)
                    {
                        return SYNTAX_ERROR;
                    }
                    if(activeToken.type == TYPE_COMMA || activeToken.type == TYPE_RIGHT_BRACKET){
                        if(func->arguments != NULL){ // Function's argument list is not empty
                            paramCounter++;
                            // Unnamed parameter
                            if(strcmp(func->arguments->name, "_") == 0){
                                // Variable used in function's parameters
                                if(tmp.type == TYPE_VARIABLE){
                                    // Check if variable was defined
                                    if((var = BVSSearch(data->rootVar->rootPtr, tmp)) == NULL)
                                    {
                                        return UNDEFINED_VARIABLE_ERROR;
                                    }
                                    // Variable has wrong data type
                                    if(func->arguments->data_type != var->content->variable_type){
                                        return FUNCTION_CALL_ERROR;
                                    }
                                }

                                // Define parameter in temporary frame
                                op1.str = PARAM;
                                op1.symbol = SYMBOL_TF;
                                setInstruction(paramCounter,defvar,DEFVAR,&op1,NULL,NULL);
                                symbolClear(&op1);

                                op1.str = PARAM;
                                op1.symbol = SYMBOL_TF;

                                op2.str = tmp.content.str->str;
                                if(tmp.type == TYPE_VARIABLE)
                                {
                                    // Set varaible's symbol
                                    data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                                }
                                else{ // Set symbols
                                    switch (tmp.type)
                                    {
                                        case TYPE_STRING:
                                            op2.symbol = SYMBOL_STRING;
                                            break;
                                        case TYPE_INTEGER:
                                            op2.symbol = SYMBOL_INTEGER;
                                            break;
                                        case TYPE_DOUBLE:
                                            op2.symbol = SYMBOL_DOUBLE;
                                            break;

                                        default:
                                            break;
                                    }
                                }

                                // Move content to the parameter in temporary frame
                                setInstruction(paramCounter,move,MOVE,&op1,&op2,NULL);
                                symbolClear(&op1);
                                symbolClear(&op2);

                                // Check if there should be more arguments
                                if(func->arguments->next != NULL){
                                    func->arguments = func->arguments->next;
                                    if (activeToken.type == TYPE_RIGHT_BRACKET)
                                    {
                                        // Wrong parameters number
                                        if(paramCounter != func->parameters){
                                            return FUNCTION_CALL_ERROR;
                                        }
                                    }
                                        // Check for comma between parameters
                                    else if(activeToken.type != TYPE_COMMA){
                                        return SYNTAX_ERROR;
                                    }
                                }
                                else{ // Check fot right bracket after last argument
                                    if (activeToken.type == TYPE_RIGHT_BRACKET)
                                    {
                                        // Wrong parameters number
                                        if(paramCounter != func->parameters){
                                            return FUNCTION_CALL_ERROR;
                                        }
                                        return OK;
                                    }
                                        // Check for comma between parameters
                                    else if(activeToken.type != TYPE_COMMA){
                                        return SYNTAX_ERROR;
                                    }
                                }

                            }
                            else{ // Found named parameter
                                return SYNTAX_ERROR;
                            }
                        }
                        else{ // Parameters list should be empty
                            return SYNTAX_ERROR;
                        }
                    }
                    else if(activeToken.type == TYPE_COLON){
                        activeToken = dequeue(queue);
                        // Parameter was named
                        if (activeToken.type != TYPE_VARIABLE && activeToken.type != TYPE_INTEGER && activeToken.type != TYPE_STRING && activeToken.type != TYPE_DOUBLE)
                        {
                            return SYNTAX_ERROR;
                        }
                        paramCounter++;
                        // Check if variable was defined
                        if(activeToken.type == TYPE_VARIABLE){
                            if((var = BVSSearch(data->rootVar->rootPtr, activeToken)) == NULL)
                            {
                                return UNDEFINED_VARIABLE_ERROR;
                            }
                            // Wrong variable's data type
                            if(func->arguments->data_type != var->content->variable_type){
                                return FUNCTION_CALL_ERROR;
                            }
                        }
                        else{
                            // Wrong data type
                            if(func->arguments->data_type != activeToken.type){
                                return FUNCTION_CALL_ERROR;
                            }
                        }

                        // Define parameter in temporary frame
                        op1.str = PARAM;
                        op1.symbol = SYMBOL_TF;
                        setInstruction(paramCounter,defvar,DEFVAR,&op1,NULL,NULL);
                        symbolClear(&op1);

                        op1.str = PARAM;
                        op1.symbol = SYMBOL_TF;

                        op2.str = activeToken.content.str->str;
                        // Set variable's symbol
                        if(activeToken.type == TYPE_VARIABLE)
                        {
                            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
                        }
                        else{ // Set symbol
                            switch (activeToken.type)
                            {
                                case TYPE_STRING:
                                    op2.symbol = SYMBOL_STRING;
                                    break;
                                case TYPE_INTEGER:
                                    op2.symbol = SYMBOL_INTEGER;
                                    break;
                                case TYPE_DOUBLE:
                                    op2.symbol = SYMBOL_DOUBLE;
                                    break;

                                default:
                                    break;
                            }
                        }

                        // Move content to variable in temporary frame
                        setInstruction(paramCounter,move,MOVE,&op1,&op2,NULL);
                        symbolClear(&op1);
                        symbolClear(&op2);

                        activeToken = dequeue(queue);
                        // Get next argument from list
                        if(func->arguments->next != NULL){
                            func->arguments = func->arguments->next;
                        }
                        // End of parameter list
                        if (activeToken.type == TYPE_RIGHT_BRACKET)
                        {
                            // Wrong parameter number
                            if(paramCounter != func->parameters){
                                return FUNCTION_CALL_ERROR;
                            }
                            return OK;
                        }
                            // Check for comma between parameters
                        else if(activeToken.type != TYPE_COMMA){
                            return SYNTAX_ERROR;
                        }
                    }
                }
                else{
                    // Parameter list  and function's argument list is empty
                    if(func->parameters == 0 && activeToken.type == TYPE_RIGHT_BRACKET)
                    {
                        return OK;
                    }
                    // Missing parameter
                    return SYNTAX_ERROR;
                }
                // Continue while there is another parameter
            }while(activeToken.type == TYPE_COMMA);

        default:
            break;
    }
    return OK;
}

// Check for data type
int type(TokenQueue *queue,TNode *variable)
{
    // Check if there is colon or point before data type
    if (activeToken.type == TYPE_COLON || activeToken.type == TYPE_POINT)
    {
        activeToken = dequeue(queue);
        // Following token should be data type
        if ((result = type_spec(queue)) == SYNTAX_ERROR)
        {
            return result;
        }
        else{
            if(variable != NULL)
            {
                // update variable's type in the right BST
                updateBVS(variable,result, activeToken.qw);
            }
            return OK;
        }
    }
    else{
        return SYNTAX_ERROR;
    }

    return OK;
}

// <type_spec>
int type_spec(TokenQueue *queue)
{
    if (activeToken.type != KEYWORD_INT && activeToken.type != KEYWORD_INT_QW && activeToken.type != KEYWORD_DOUBLE && activeToken.type != KEYWORD_DOUBLE_QW && activeToken.type != KEYWORD_STRING && activeToken.type != KEYWORD_STRING_QW)
    {
        return SYNTAX_ERROR; // Data type not found
    }

    // Return found data type to update variable's type
    switch (activeToken.type)
    {
        case KEYWORD_INT:
            return TYPE_INTEGER;
        case KEYWORD_INT_QW:
            activeToken.qw = true;
            return TYPE_INTEGER;

        case KEYWORD_DOUBLE:
            activeToken.qw = false;
            return TYPE_DOUBLE;
        case KEYWORD_DOUBLE_QW:
            activeToken.qw = true;
            return TYPE_DOUBLE;

        case KEYWORD_STRING:
            activeToken.qw = false;
            return TYPE_STRING;
        case KEYWORD_STRING_QW:
            activeToken.qw = true;
            return TYPE_STRING;

        default:
            break;
    }
    return OK;

}

// Checks if token is variable's/function's ID
int ID(TokenQueue *queue)
{

    if (activeToken.type != TYPE_VARIABLE && activeToken.type != TYPE_FUNCTION_NAME)
    {
        return SYNTAX_ERROR; // Token is not an ID
    }
    return OK;
}

// Checks if token is left bracket
int Lbracket()
{

    if (activeToken.type != TYPE_LEFT_BRACKET)
    {
        return SYNTAX_ERROR; // Token is not a left bracket
    }

    return OK;
}

// Checks if token is right bracket
int Rbracket(TokenQueue *queue)
{
    if (activeToken.type != TYPE_RIGHT_BRACKET)
    {
        return SYNTAX_ERROR; // Token is  not a right bracket
    }

    return OK;
}

// Checks if token is left viniculum
int Lvinculum(TokenQueue *queue)
{

    if (activeToken.type != TYPE_LEFT_VINCULUM)
    {
        return SYNTAX_ERROR; // Token is not a left viniculum
    }

    return OK;
}

// Checks if token is right viniculum
int Rvinculum(TokenQueue *queue)
{

    if (activeToken.type != TYPE_RIGHT_VINCULUM)
    {
        return SYNTAX_ERROR; // Token is not a right viniculum
    }

    return OK;
}

// Built-in function WRITE
int writeFunc(TokenQueue *queue)
{
    symbolClear(&op1);
    do{
        activeToken = dequeue(queue); // Get first token after left bracket or comma
        switch(activeToken.type)
        {
            // Token is variable
            case TYPE_VARIABLE:
                // Check if variable was defined
                if(data->inFunction)
                {
                    if((BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                    {
                        return UNDEFINED_VARIABLE_ERROR;
                    }
                }
                else{
                    if((BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                    {
                        return UNDEFINED_VARIABLE_ERROR;
                    }
                }

                // Set Write instruction
                op1.str = activeToken.content.str->str;
                data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
                setInstruction(0,write,WRITE,&op1,NULL,NULL);
                symbolClear(&op1);

                break;

                // Token is a string
            case TYPE_STRING:
                // Set Write instruction
                op1.str = activeToken.content.str->str;
                op1.symbol = SYMBOL_STRING;
                setInstruction(0,write,WRITE,&op1,NULL,NULL);
                symbolClear(&op1);
                break;

                // Token is a integer
            case TYPE_INTEGER:
                // Set Write instruction
                op1.str = activeToken.content.str->str;
                op1.symbol = SYMBOL_INTEGER;
                setInstruction(0,write,WRITE,&op1,NULL,NULL);
                symbolClear(&op1);
                break;

                // Token is a double number
            case TYPE_DOUBLE:
                // Set Write instruction
                op1.str = activeToken.content.str->str;
                op1.symbol = SYMBOL_DOUBLE;
                setInstruction(0,write,WRITE,&op1,NULL,NULL);
                symbolClear(&op1);
                break;

            default:
                break;
        }

        activeToken = dequeue(queue);
        // After right bracket, end printing
        if ((result = Rbracket(queue)) == OK)
        {
            return result;
        }
        else if(activeToken.type != TYPE_COMMA){
            return SYNTAX_ERROR; // There is no comma between arguments
        }

        // If there is more arguments, continue
    }while(activeToken.type == TYPE_COMMA);
    return OK;
}

// Built-in function readX (X={Int,Double,String})
int readFunc(TokenQueue *queue, int type)
{
    // Check for left bracket after function's ID
    if((result = Lbracket()) != OK)
    {
        return result;
    }
    symbolClear(&op1);
    symbolClear(&op2);

    // Set variable to store result to
    op1.str = variable->content->str;
    data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

    switch (type)
    {
        case SYMBOL_INTEGER:
            // Reading integer number
            op2.str = "int";
            // Set variable's type
            updateBVS(variable,TYPE_INTEGER, false);
            break;

        case SYMBOL_STRING:
            // Reading string
            op2.str = "string";
            // Set variable's type
            updateBVS(variable,TYPE_STRING, false);
            break;

        case SYMBOL_DOUBLE:
            // Reading double number
            op2.str = "float";
            // Set variable's type
            updateBVS(variable,TYPE_DOUBLE, false);
        default:
            break;
    }

    // Set instruction to read
    setInstruction(0,read,READ,&op1,&op2,NULL);
    symbolClear(&op1);
    mystrClear(&op2);

    return OK;
}

// Built-in function Int2Double
int int2double(TokenQueue *queue)
{
    // Check for left bracket after function's ID
    if((result = Lbracket()) != OK)
    {
        return result;
    }

    symbolClear(&op1);
    symbolClear(&op2);

    activeToken = dequeue(queue);
    // Argument has to be variable or integer number
    if(activeToken.type == TYPE_VARIABLE)
    {
        // Check if variable was defined
        if(data->inFunction)
        {
            if((BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_VARIABLE_ERROR;
            }
        }
        else{
            if((BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_VARIABLE_ERROR;
            }
        }
        // Set variable's symbol
        data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
    }
    else if(activeToken.type == TYPE_INTEGER)
    {
        op2.symbol = SYMBOL_INTEGER; // Set argument's symbol after integer number was found
    }
    else{
        return FUNCTION_CALL_ERROR; // Wrong argument type
    }

    // Set variable to store result to
    op1.str = variable->content->str;
    data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

    op2.str = activeToken.content.str->str;

    setInstruction(0,int2float,INT2FLOAT,&op1,&op2,NULL);
    symbolClear(&op1);

    // Update variable's type in BST
    updateBVS(variable,TYPE_DOUBLE, false);
    return OK;

}

// Built-in function Double2Int
int double2int(TokenQueue *queue)
{
    // Check for left bracket after function's ID
    if((result = Lbracket()) != OK)
    {
        return result;
    }

    symbolClear(&op1);
    symbolClear(&op2);

    activeToken = dequeue(queue);
    // Argument has to be variable or double number
    if(activeToken.type == TYPE_VARIABLE)
    {
        // Check if variable was defined
        if(data->inFunction)
        {
            if((BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_VARIABLE_ERROR;
            }
        }
        else{
            if((BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_VARIABLE_ERROR;
            }
        }

        // Set variable's symbol
        data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
    }
    else if(activeToken.type == TYPE_DOUBLE)
    {
        op2.symbol = SYMBOL_DOUBLE; // Set argument's symbol after double number was found
    }
    else{
        return FUNCTION_CALL_ERROR; // Wrong argument's type
    }

    // Set variable to store result to
    op1.str = variable->content->str;
    data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

    op2.str = activeToken.content.str->str;

    setInstruction(0,float2int,FLOAT2INT,&op1,&op2,NULL);
    symbolClear(&op1);

    // Update variable's type in BST
    updateBVS(variable,TYPE_INTEGER, false);
    return OK;

}

// Built-in function chr
int chr(TokenQueue *queue)
{
    // Check for left bracket after function's ID
    if((result = Lbracket()) != OK)
    {
        return result;
    }

    symbolClear(&op1);
    symbolClear(&op2);

    activeToken = dequeue(queue);
    // Argument has to be variable or integer number
    if(activeToken.type == TYPE_VARIABLE || activeToken.type == TYPE_INTEGER)
    {
        // Set variable to store result to
        op1.str = variable->content->str;
        data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);

        if(activeToken.type == TYPE_VARIABLE)
        {
            // Check if variable was defined
            if(data->inFunction)
            {
                if((BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                {
                    return UNDEFINED_VARIABLE_ERROR;
                }
            }
            else{
                if((BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                {
                    return UNDEFINED_VARIABLE_ERROR;
                }
            }
            // Set variable's symbol
            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
        }
        else{
            op2.symbol = SYMBOL_INTEGER; // Set integer number's symbol
        }
        op2.str = activeToken.content.str->str;

        setInstruction(0,int2char,INT2CHAR,&op1,&op2,NULL);
        symbolClear(&op1);

        // Update variable's type in BST
        updateBVS(variable,TYPE_STRING, false);
        return OK;
    }
    else{
        return SYNTAX_ERROR; // Wrong argument type
    }
}

// Built-in function ord
int ord(TokenQueue *queue)
{
    // Check if there is left bracket after function's ID
    if((result = Lbracket()) != OK)
    {
        return result;
    }

    symbolClear(&op1);
    symbolClear(&op2);

    activeToken = dequeue(queue);
    // Argument has to be variable or string
    if(activeToken.type == TYPE_VARIABLE || activeToken.type == TYPE_STRING)
    {
        data->helper++; // Increment helper for unique label name
        if(activeToken.type == TYPE_VARIABLE)
        {
            // Check if variable was defined
            if(data->inFunction)
            {
                if((BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
                {
                    return UNDEFINED_VARIABLE_ERROR;
                }
            }
            else{
                if((BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
                {
                    return UNDEFINED_VARIABLE_ERROR;
                }
            }
            // Set variable's symbol
            data->inFunction ? (op2.symbol = SYMBOL_LF) : (op2.symbol = SYMBOL_GF);
        }
        else{
            op2.symbol = SYMBOL_STRING; // Set string symbol
        }

        op2.str = activeToken.content.str->str;

        op1.str = "charLabel";

        op3.str = "";
        op3.symbol = SYMBOL_STRING;
        // Return 0 if there is empty string
        setInstruction(data->helper,jumpifeq,JUMPIFEQ,&op1,&op2,&op3);

        op3.str = "0"; // Position
        op3.symbol = SYMBOL_INTEGER;
        op2.str = activeToken.content.str->str; // Function's argument
        op1.str = variable->content->str; // Variable to store result to
        data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
        setInstruction(0,str2int,STR2INT,&op1,&op2,&op3);
        symbolClear(&op1);
        symbolClear(&op2);

        // Jump to the end
        op1.str = "charLabel";
        setInstruction(data->helper+1,jump,JUMP,&op1,NULL,NULL);

        op1.str = "charLabel"; // Label if string was empty

        setInstruction(data->helper,label,LABEL,&op1,NULL,NULL);

        op3.str = "0";
        op3.symbol = SYMBOL_INTEGER;
        op1.str = variable->content->str;
        data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
        // Move 0 to variable after string was empty
        setInstruction(0,move,MOVE,&op1,&op3,NULL);
        symbolClear(&op1);
        symbolClear(&op3);

        data->helper++;
        op1.str = "charLabel"; // End label
        setInstruction(data->helper,label,LABEL,&op1,NULL,NULL);
        data->helper++;
        symbolClear(&op1);

        // Update variable's type in BST
        updateBVS(variable,TYPE_INTEGER, false);
        return OK;
    }
    else{
        return SYNTAX_ERROR; // Wrong argument type
    }
}

// Built-in function substring
int substring()
{
    // Body of substring function
    // Skip function's body
    op1.str = "substring_func_end";
    setInstruction(SKIP,jump,JUMP,&op1,NULL,NULL);
    // Label of a function
    op1.str = "substring_func";
    setInstruction(SKIP,label,LABEL,&op1,NULL,NULL);
    setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
    // Second argument -----
    // Check if second argument is < 0
    op1.str = "param2";
    op1.symbol = SYMBOL_LF;
    op2.str = "0";
    op2.symbol = SYMBOL_INTEGER;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,pushs,PUSHS,&op2,NULL,NULL);
    setInstruction(0,lts,LTS,NULL,NULL,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    // Check if second argument is <= length(first argument)
    op1.str = "param2";
    op1.symbol = SYMBOL_LF;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    symbolClear(&op1);
    op1.str = "EXP_i";
    op1.symbol = SYMBOL_LF;
    setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
    op2.str = "param1";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,strLen,STRLEN,&op1,&op2,NULL);
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);

    // Check if at least one of the previous checks was true, than return nil
    setInstruction(0,lts,LTS,NULL,NULL,NULL);
    setInstruction(0,nots,NOTS,NULL,NULL,NULL);
    setInstruction(0,ors,ORS,NULL,NULL,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    // If there was error, jump to substring_error
    op2.str = "EXP_b";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,defvar,DEFVAR,&op2,NULL,NULL);
    setInstruction(0,pops,POPS,&op2,NULL,NULL);
    op1.str = "substring_error";
    op3.str = BOOL_TRUE;
    setInstruction(SKIP,jumpifeq,JUMPIFEQ,&op1,&op2,&op3);

    // Third argument ---

    setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
    // Check if third argument is < 0
    op1.str = "param3";
    op1.symbol = SYMBOL_LF;
    op2.str = "0";
    op2.symbol = SYMBOL_INTEGER;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,pushs,PUSHS,&op2,NULL,NULL);
    setInstruction(0,lts,LTS,NULL,NULL,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    // Check is third argument is < length(first argument)
    op2.str = "param3";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,pushs,PUSHS,&op2,NULL,NULL);
    op1.str = "EXP_i";
    op1.symbol = SYMBOL_LF;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,gts,GTS,NULL,NULL,NULL);
    setInstruction(0,ors,ORS,NULL,NULL,NULL);
    symbolClear(&op1);

    // Check if at least one of the previous checks was true, than return nil
    op2.str = "EXP_b";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,pops,POPS,&op2,NULL,NULL);
    // If there is error, jump to substring_error
    op1.str = "substring_error";
    op3.str = BOOL_TRUE;
    setInstruction(SKIP,jumpifeq,JUMPIFEQ,&op1,&op2,&op3);

    // Check is second argument is > third argument
    setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
    op1.str = "param2";
    op1.symbol = SYMBOL_LF;
    op2.str = "param3";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,pushs,PUSHS,&op2,NULL,NULL);
    setInstruction(0,gts,GTS,NULL,NULL,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    op2.str = "EXP_b";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,pops,POPS,&op2,NULL,NULL);
    // If there is an error, jump to substring_error
    op1.str = "substring_error";
    op3.str = BOOL_TRUE;
    setInstruction(SKIP,jumpifeq,JUMPIFEQ,&op1,&op2,&op3);
    symbolClear(&op1);
    symbolClear(&op2);

    // GET SUBSTRING
    setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
    op1.str = "param3";
    op1.symbol = SYMBOL_LF;
    op2.str = "param2";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,pushs,PUSHS,&op2,NULL,NULL);
    setInstruction(0,subs,SUBS,NULL,NULL,NULL);
    // Counts the substring length
    op1.str = "substring_cnt";
    setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
    setInstruction(0,pops,POPS,&op1,NULL,NULL);

    symbolClear(&op1);
    symbolClear(&op2);
    // Substring's result
    op1.str = "substring_value";
    op1.symbol = SYMBOL_LF;
    setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
    // Move an empty string at the start
    op2.str = "";
    op2.symbol = SYMBOL_STRING;
    setInstruction(0,move,MOVE,&op1,&op2,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    // Single character of string at the current position
    op1.str = "substring_char";
    op1.symbol = SYMBOL_LF;
    setInstruction(0,defvar,DEFVAR,&op1,NULL,NULL);
    symbolClear(&op1);

    // Label to loop through string's characters
    op1.str = "substring_label";
    setInstruction(SKIP,label,LABEL,&op1,NULL,NULL);
    symbolClear(&op1);

    // IF substring_cnt counter is 0, end substring function
    op1.str = "substring_end";
    op2.str = "substring_cnt";
    op2.symbol = SYMBOL_LF;
    op3.str = "0";
    op3.symbol = SYMBOL_INTEGER;

    setInstruction(SKIP,jumpifeq,JUMPIFEQ,&op1,&op2,&op3);

    symbolClear(&op1);

    // Store character to substring_char of string at current position
    op1.str = "substring_char";
    op1.symbol = SYMBOL_LF;
    op2.str = "param1";
    op2.symbol = SYMBOL_LF;
    op3.str = "param2";
    op3.symbol = SYMBOL_LF;
    setInstruction(0,getchar,GETCHAR,&op1,&op2,&op3);
    setInstruction(0,pushs,PUSHS,&op3,NULL,NULL);
    symbolClear(&op1);

    // Increment position
    op1.str = "1";
    op1.symbol = SYMBOL_INTEGER;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,adds,ADDS,NULL,NULL,NULL);
    setInstruction(0,pops,POPS,&op3,NULL,NULL);
    symbolClear(&op3);
    symbolClear(&op2);
    symbolClear(&op1);

    // Append substring_char to the substring_value
    op1.str = "substring_value";
    op1.symbol = SYMBOL_LF;
    op2.str = "substring_char";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,concat,CONCAT,&op1,&op1,&op2);
    symbolClear(&op2);
    symbolClear(&op1);

    setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
    // Decrement substring_cnt after every GETCHAR
    op1.str = "substring_cnt";
    op1.symbol = SYMBOL_LF;
    op2.str = "1";
    op2.symbol = SYMBOL_INTEGER;
    setInstruction(0,pushs,PUSHS,&op1,NULL,NULL);
    setInstruction(0,pushs,PUSHS,&op2,NULL,NULL);
    setInstruction(0,subs,SUBS,NULL,NULL,NULL);
    op1.str = "substring_cnt";
    setInstruction(0,pops,POPS,&op1,NULL,NULL);
    symbolClear(&op1);

    // Jump to the substring_label to loop over string's characters
    op1.str = "substring_label";
    setInstruction(SKIP,jump,JUMP,&op1,NULL,NULL);

    // Label for handling errors
    op1.str = "substring_error";
    setInstruction(SKIP,label,LABEL,&op1,NULL,NULL);
    symbolClear(&op2);

    // Return nil
    op1.str = RETURN_VALUE;
    op1.symbol = SYMBOL_LF;
    op2.str = TYPE_NIL;
    setInstruction(0,move,MOVE,&op1,&op2,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    // Jump to the end of a function
    op1.str = "substring_return";
    setInstruction(SKIP,jump,JUMP,&op1,NULL,NULL);

    // Function ended withou errors
    op1.str = "substring_end";
    setInstruction(SKIP,label,LABEL,&op1,NULL,NULL);
    // Return substring_value
    op1.str = RETURN_VALUE;
    op1.symbol = SYMBOL_LF;
    op2.str = "substring_value";
    op2.symbol = SYMBOL_LF;
    setInstruction(0,move,MOVE,&op1,&op2,NULL);
    symbolClear(&op1);
    symbolClear(&op2);

    // Function's end
    op1.str = "substring_return";
    setInstruction(SKIP,label,LABEL,&op1,NULL,NULL);
    setInstruction(0,popframe,POPFRAME,NULL,NULL,NULL);
    setInstruction(0,return_ins,RETURN,NULL,NULL,NULL);

    // Label to skip function's body
    op1.str = "substring_func_end";
    setInstruction(SKIP,label,LABEL,&op1,NULL,NULL);
    return OK;
}

// Built-in function length
int strlen_f(TokenQueue *queue)
{
    symbolClear(&op1);
    symbolClear(&op2);

    //Checks if there is left bracket after function's ID
    if((result = Lbracket()) != OK)
    {
        return result;
    }
    activeToken = dequeue(queue);

    // Argument has to be variable or string
    if(activeToken.type == TYPE_VARIABLE)
    {
        // Check if variable was defined
        if(data->inFunction)
        {
            if((variable = BVSSearch(data->local_variable->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_VARIABLE_ERROR;
            }
            if(variable->content->variable_type != TYPE_STRING)
            {
                return FUNCTION_CALL_ERROR;
            }
            // Set variable for instruction
            op2.str = variable->content->str;
            op2.symbol = SYMBOL_LF;
        }
        else{
            if((variable = BVSSearch(data->rootVar->rootPtr,activeToken)) == NULL)
            {
                return UNDEFINED_VARIABLE_ERROR;
            }
            if(variable->content->variable_type != TYPE_STRING)
            {
                return FUNCTION_CALL_ERROR;
            }
            // Set variable for instruction
            op2.str = variable->content->str;
            op2.symbol = SYMBOL_GF;
        }
    }
        // Argument is type string
    else if(activeToken.type == TYPE_STRING)
    {
        // Set string for instruction
        op2.str = activeToken.content.str->str;
        op2.symbol = SYMBOL_STRING;
    }
    else{
        return FUNCTION_CALL_ERROR; // Wrong argument type
    }

    // Store strlen result into EXP_i
    op1.str = "EXP_i";
    data->inFunction ? (op1.symbol = SYMBOL_LF) : (op1.symbol = SYMBOL_GF);
    setInstruction(0,strLen,STRLEN,&op1,&op2,NULL);
    symbolClear(&op1);
    symbolClear(&op2);
    return OK;
}

// Function to clear operand's symbol
void symbolClear(mystring *op)
{
    op->symbol = 0;
    return;
}

// Function to parse expression
int Expression(token my_token, TokenQueue *queue, bool in_while_or_if,Data *data, TNode *variable){
    int res; // result of expression parsing
    token t = my_token;

    // Clear stack
    setInstruction(0,clears,CLEARS,NULL,NULL,NULL);
    // Perform expression parsing
    res = PrecedenceAnalysis(stack, t, queue, in_while_or_if, variable);

    pStack_free(stack); // Free pStack
    return res;
}

// Initialize helping structure's parameters
void InitData(Data *data)
{
    data->caller = 0;
    data->cnt = 0;
    data->helper = 0;
    data->ifCounter = 0;
    data->inFunction = false;
    data->inReturn = false;
    data->whileCounter = 0;
}