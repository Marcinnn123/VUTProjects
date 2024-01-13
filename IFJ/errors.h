//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Marcin Sochacki (xsocha03)                   //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#define OK 0                            // SUCCES
#define SCANNER_ERROR 1                 // error in the program in lexical analysis
#define SYNTAX_ERROR 2                  // error in parsing
#define UNDEFINED_FUNCTION_ERROR 3      // undefined function, variable redefinition
#define FUNCTION_CALL_ERROR 4           // wrong number/type of parameters while calling the function or wrong type of return value
#define UNDEFINED_VARIABLE_ERROR 5      // undefined variable
#define RETURN_COUNT_ERROR 6            // missing/overrun expression in return statement from the function
#define ARITMETIC_EXPRESSION_ERROR 7    // type compatibility error in arithmetic, string and relational expressions 
#define NOT_SPECIFIED_ERROR 8           // variable or parameter type is not specified and cannot be inferred from the expression used
#define OTHER_SEMANTIC_ERROR 9          // other semantic errors
#define INTERNAL_ERROR 99               // error not affected by the input program