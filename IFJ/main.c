//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "scanner.h"
#include "str.h"
#include "errors.h"
#include "parse.h"
#include "symtable.h"
#include "tokenqueue.h"




int queue_parse(TokenQueue *queue, token *mytoken, TRootf *rootFun) {
    int result;
    int bracket_counter = 1;
    mystring function_name_save;

    argList *args = NULL;
    char *arg_name;
    char *arg_identifier;
    int arg_data_type;

    bool id_or_name = false;

    int function_parameters_count = 1;
    mystrInit(&function_name_save);

    do {
        // Scan the next token
        if ((result = scanNextToken(mytoken)) != OK) {
            return result;
        }
        // Check for end of file
        if (mytoken->type == TYPE_EOF) {
            return OK;
        }
        // Enqueue the token for further processing
        enqueue(queue, mytoken);

        // Check for the FUNC keyword
        if (mytoken->type == KEYWORD_FUNC) {
            if ((result = scanNextToken(mytoken)) != OK) {
                return result;
            }
            if (mytoken->type == TYPE_EOF) {
                return OK;
            }
            enqueue(queue, mytoken);

            if (mytoken->type == TYPE_FUNCTION_NAME) {
                // Check if the function with the same name already exists
                if (BVSSearch_function(rootFun->rootPtr, *mytoken) != NULL) {
                    // redefinition of function
                    return UNDEFINED_FUNCTION_ERROR;
                }

                // Copy the function name for later use
                mystrCopy(&function_name_save, mytoken->content.str);

                if ((result = scanNextToken(mytoken)) != OK) {
                    return result;
                }
                if (mytoken->type == TYPE_EOF) {
                    return OK;
                }
                enqueue(queue, mytoken);

                // Skip {
                if ((result = scanNextToken(mytoken)) != OK) {
                    return result;
                }
                if (mytoken->type == TYPE_EOF) {
                    return OK;
                }
                enqueue(queue, mytoken);

                // Check for the right bracket to handle function parameters
                if (mytoken->type == TYPE_RIGHT_BRACKET) {
                    bracket_counter--;
                    function_parameters_count = 0;
                }

                // Process function parameters
                while (bracket_counter != 0) {
                    // Check for left bracket to increment counter
                    if (mytoken->type == TYPE_LEFT_BRACKET) {
                        bracket_counter++;
                    }
                    // Check for right bracket to handle the end of parameters
                    else if (mytoken->type == TYPE_RIGHT_BRACKET) {
                        args = addNode(args, arg_data_type, arg_name, arg_identifier);
                        bracket_counter--;
                    }
                    // Check for variable or underscore to handle parameter name and identifier
                    else if (mytoken->type == TYPE_VARIABLE || mytoken->type == TYPE_UNDERSCORE) {
                        if (id_or_name == false) {
                            if (mytoken->type == TYPE_UNDERSCORE) {
                                arg_name = "_";
                            } else {
                                arg_name = mytoken->content.str->str;
                            }
                            id_or_name = true;
                        } else if (id_or_name == true) {
                            arg_identifier = mytoken->content.str->str;
                            id_or_name = false;
                        }
                    }
                    // Check for data types to handle parameter data type
                    else if (mytoken->type == KEYWORD_INT || mytoken->type == KEYWORD_STRING || mytoken->type == KEYWORD_DOUBLE) {
                        if (mytoken->type == KEYWORD_INT) {
                            arg_data_type = TYPE_INTEGER;
                        } else if (mytoken->type == KEYWORD_STRING) {
                            arg_data_type = TYPE_STRING;
                        } else if (mytoken->type == KEYWORD_DOUBLE) {
                            arg_data_type = TYPE_DOUBLE;
                        }
                    }
                    // Check for comma to handle multiple parameters
                    else if (mytoken->type == TYPE_COMMA) {
                        function_parameters_count++;
                        args = addNode(args, arg_data_type, arg_name, arg_identifier);
                    }

                    if ((result = scanNextToken(mytoken)) != OK) {
                        return result;
                    }
                    if (mytoken->type == TYPE_EOF) {
                        return OK;
                    }
                    enqueue(queue, mytoken);
                }

                // Check for the right bracket to complete parameter processing
                if (mytoken->type == TYPE_RIGHT_BRACKET) {
                    if ((result = scanNextToken(mytoken)) != OK) {
                        return result;
                    }
                    if (mytoken->type == TYPE_EOF) {
                        return OK;
                    }
                    enqueue(queue, mytoken);
                }

                if (mytoken->type == TYPE_LEFT_VINCULUM) {
                    // Insert void function 
                    rootFun->rootPtr = BVSInsert_function(rootFun->rootPtr, function_name_save, function_parameters_count, 0, args);
                } else {
                    if ((result = scanNextToken(mytoken)) != OK) {
                        return result;
                    }
                    if (mytoken->type == TYPE_EOF) {
                        return OK;
                    }
                    enqueue(queue, mytoken);

                    // Check for data types to handle the return type of the function
                    if (mytoken->type == KEYWORD_STRING || mytoken->type == KEYWORD_INT || mytoken->type == KEYWORD_DOUBLE) {
                        // Insert the function information into the function Binary Search Tree with the return type
                        rootFun->rootPtr = BVSInsert_function(rootFun->rootPtr, function_name_save, function_parameters_count, mytoken->type, args);
                    }
                }

                // Clear the function name for the next iteration
                mystrClear(&function_name_save);
                function_parameters_count = 1;
                bracket_counter = 1;
            }
        }

    } while (true);

    return OK;
}


int main(void) {
    setSourceFile(stdin);
    TokenQueue tokenQueue; // Initialize the queue
    initializeQueue(&tokenQueue);
    

    token initToken;
    mystring initString;
    mystring initIneger;
    mystring initDouble;
    token *activeLex;
    

    initString.str = NULL;
    initString.length = 0;
    initString.capacity = 0;
    initString.symbol = 0;
    initString.qw = false;
    initString.variable_type = 0;
    initToken.type = 199;
    initToken.content.str = &initString;
    initToken.content.integerNumber = &initIneger;
    initToken.content.doubleNumber = &initDouble;
    initToken.qw = false;
    
    activeLex = &initToken;
    TRootf FUNroot;
    TRootf *rootFun;

    rootFun = &FUNroot;

    BVSInit_function(rootFun);
 
    int result;
    if((result = queue_parse(&tokenQueue, activeLex, rootFun)) != OK){
        printf("EXIT int@%d\n",result);
        return result;
    }
    if(isEmpty(&tokenQueue)){
        printf(".IFJcode23");
        return OK;
    } 
    result = parse(rootFun,&tokenQueue);
    printf("EXIT int@%d\n",result);
    return result;
}

#endif
