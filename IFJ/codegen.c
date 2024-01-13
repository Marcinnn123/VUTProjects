//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Adam Nieslanik (xniesl00)                    //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "str.h"
#include "errors.h"
#include "parse.h"
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

extern Data *data;

// Set instruction's opcode, operands
int setInstruction(int cnt,int opcode,char* ins,mystring *op1,mystring *op2,mystring *op3)
{   
    Instruction instruction = malloc(sizeof(Instruction)); // Allocate memory for instruction
    Params *params = (Params*)malloc(sizeof(Params)); // Allocate memory for instruction's parameters
    char* ifLabel = malloc(sizeof(char)); // Allocate memory for label string
    
    // Handle errors
    if(instruction == NULL || params == NULL || ifLabel == NULL)
    {
        exit(INTERNAL_ERROR);
    }

    // Set operands according to each instruction
    switch (opcode)
    {
    case jumpifeq:
    case jumpifneq:
        if(cnt != SKIP){
            sprintf(ifLabel,"%s_%d",op1->str,cnt); // Create unique label
            op1->str = ifLabel;
        }
        
        break;
    
    case label:
    case jump:
    case call:
        if(cnt != SKIP){
            if(strcmp(op1->str,FUN) == 0 || strcmp(op1->str,FUN_END) == 0)
            {
                sprintf(ifLabel,"%s_%s",op1->str,op2->str); // Create unique label
                op2 = NULL;
            }
            else
            {
                sprintf(ifLabel,"%s_%d",op1->str,cnt); // Create unique label
            }
            op1->str = ifLabel;
        }
        break;

    case move:
    case defvar:
        if(op1 != NULL)
        {
            if(!(strcmp(op1->str,PARAM)))
            {
                sprintf(ifLabel,"%s_%d",op1->str,cnt); // Create unique parameter
                op1->str = ifLabel;
            }
        }

        if(op2 != NULL)
        {
            if(!(strcmp(op2->str,PARAM)))
            {
                
                sprintf(ifLabel,"%s_%d",op2->str,cnt); // Create unique parameter
                op2->str = ifLabel;
            }
        }
        break;
    case write:
    case read:
    case createframe:
    case pushframe:
    case popframe:
    case return_ins:
    case pushs:
    case pops:
    case clears:

    case int2float:
    case int2floats:
    case float2int:
    case int2char:
    case str2int:
    case strLen:

    case adds:
    case subs:
    case muls:
    case divs:
    case idivs:
    case nots:
    case gts:
    case lts:
    case eqs:
    case eq:
    case header:
      
        break;

    default:
        break;
    }
    if(op1 != NULL){
        // Push nil to stack
        if(strcmp(op1->str, "nil") == 0){
            printf("PUSHS nil@nil\n");
            free(ifLabel);
            free(params);
            free(instruction);
            return OK;
        }
    }
    // Set created operands to instruction structure
    params->param1 = op1;
    params->param2 = op2;
    params->param3 = op3;

    instruction->opcode = ins;
    instruction->params = params;

    // Print created instruction
    printInstruction(instruction);

    // Free allocated memory
    free(ifLabel);
    free(params);
    free(instruction);
    return OK;
}
 
// Print created instruction
void printInstruction(Instruction instruction)
{ 
        double double_number; // Variable for printing float numbers
        char *endptr; // Used in strtod()
        
        // Print instruction's opcode
        printf("%s",instruction->opcode);

        // Print first operand
        if(instruction->params->param1 != NULL)
        {
                // Print operand according to symbol
                switch (instruction->params->param1->symbol)
                {
                case SYMBOL_GF:
                    printf(" %s@%s","GF",instruction->params->param1->str);
                   
                    break;
                case SYMBOL_LF:
                    printf(" %s@%s","LF",instruction->params->param1->str);
                 
                    break;
                case SYMBOL_TF:
                    printf(" %s@%s","TF",instruction->params->param1->str);
             
                    break;
                case SYMBOL_INTEGER:
                    printf(" %s@%s","int",instruction->params->param1->str);
               
                    break;
                case SYMBOL_STRING:
                    printf(" %s@%s","string",instruction->params->param1->str);
                
                    break;
                case SYMBOL_DOUBLE:
                        // Convert number to float               
                        double_number = strtod(instruction->params->param1->str, &endptr);
                        printf(" %s@%a","float",double_number);              
                        break;
                default:
                    printf(" %s",instruction->params->param1->str);
                    break;
                }     
        }
        
        // Print second operand
        if(instruction->params->param2 != NULL)
        {     
                // Print operand according to symbol
                switch (instruction->params->param2->symbol)
                {
                case SYMBOL_GF:
                    printf(" %s@%s","GF",instruction->params->param2->str);
                 
                    break;
                case SYMBOL_LF:
                    printf(" %s@%s","LF",instruction->params->param2->str);
                
                    break;
                case SYMBOL_TF:
                    printf(" %s@%s","TF",instruction->params->param2->str);
             
                    break;
                case SYMBOL_INTEGER:
                    printf(" %s@%s","int",instruction->params->param2->str);
               
                    break;
                case SYMBOL_STRING:
                    printf(" %s@%s","string",instruction->params->param2->str);
                
                    break;
                case SYMBOL_DOUBLE:  
                        // Convert number to float           
                        double_number = strtod(instruction->params->param2->str, &endptr);                      
                        printf(" %s@%a","float",double_number); 
                        break;
                default:
                    printf(" %s",instruction->params->param2->str);
                    break;
                }      
        }
        
        // Print third operand
        if(instruction->params->param3 != NULL)
        {
                // Print operand according to symbol
                switch (instruction->params->param3->symbol)
                {
                case SYMBOL_GF:
                    printf(" %s@%s","GF",instruction->params->param3->str);
                    
                    break;
                case SYMBOL_LF:
                    printf(" %s@%s","LF",instruction->params->param3->str);
                    
                    break;
                case SYMBOL_TF:
                    printf(" %s@%s","TF",instruction->params->param3->str);
                    
                    break;
                case SYMBOL_INTEGER:
                    printf(" %s@%s","int",instruction->params->param3->str);
                    
                    break;
                case SYMBOL_STRING:
                    printf(" %s@%s","string",instruction->params->param3->str);
                    
                    break;
                case SYMBOL_DOUBLE: 
                    // Convert number to float             
                    double_number = strtod(instruction->params->param3->str, &endptr);
                    printf(" %s@%a","float",double_number);        
                    break;
                default:
                    printf(" %s",instruction->params->param3->str);
                    break;
                } 
        }
        printf("\n");
}
