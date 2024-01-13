//////////////////////////////////////////////////////////////////
//         @@ Implementace překladače jazyka IFJ23 @@           //
//                                                              //
//         author: Adam Nieslanik (xniesl00)                    //
//                                                              //
//                                                              //
//                                                              //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef CODEGEN
#define CODEGEN

#include "str.h"


#define SKIP 999 // Skips unique counter

// OPCODE NUMBERS
#define jump 500
#define jumpifeq 501
#define jumpifneq 502
#define label 503

#define createframe 504
#define pushframe 505
#define popframe 506

#define defvar 507
#define move 508

#define return_ins 509
#define call 510

#define adds 511
#define subs 512
#define muls 513
#define divs 514
#define idivs 536
#define lts 515
#define gts 516
#define eqs 517

#define pops 518
#define pushs 519

#define write 520
#define read 521

#define int2float 522
#define float2int 523
#define int2char 524
#define str2int 525
#define eq 526
#define nots 527
#define concat 528
#define clears 529
#define header 530
#define int2floats 531
#define strLen 532
#define getchar 533

#define add 534
#define ors 535
// OPCODE NUMBERS

// OPCODE NAMES
#define HEADER ".IFJcode23"
#define JUMP "JUMP"
#define JUMPIFEQ "JUMPIFEQ"
#define JUMPIFNEQ "JUMPIFNEQ"
#define LABEL "LABEL"
#define MOVE "MOVE"
#define DEFVAR "DEFVAR"

#define CREATEFRAME "CREATEFRAME"
#define PUSHFRAME "PUSHFRAME"
#define POPFRAME "POPFRAME"
#define RETURN "RETURN"
#define CALL "CALL"

#define BOOL_TRUE "bool@true"
#define BOOL_FALSE "bool@false"
#define TYPE_NIL "nil@nil"
#define IF "IF_end"
#define ELSE "ELSE_end"
#define WHILE "WHILE"
#define WHILE_END "WHILE_end"
#define FUN "FUN"
#define FUN_END "FUN_end"
#define PARAM "param"
#define RETURN_VALUE "return_value"

#define PUSHS "PUSHS"
#define POPS "POPS"
#define CLEARS "CLEARS"

#define ADDS "ADDS"
#define SUBS "SUBS"
#define MULS "MULS"
#define DIVS "DIVS"
#define IDIVS "IDIVS"
#define LTS "LTS"
#define GTS "GTS"
#define EQS "EQS"
#define NOTS "NOTS"
#define CONCAT "CONCAT"
#define ORS "ORS"

#define EQ "EQ"
#define ADD "ADD"

#define WRITE "WRITE"
#define READ "READ"

#define INT2FLOAT "INT2FLOAT"
#define FLOAT2INT "FLOAT2INT"
#define INT2CHAR "INT2CHAR"
#define STR2INT "STRI2INT"
#define INT2FLOATS "INT2FLOATS"
#define GETCHAR "GETCHAR"
#define STRLEN "STRLEN"
// OPCODE NAMES

/*
* Structure for instruction's operands
*/
typedef struct params{
    mystring *param1;
    mystring *param2;
    mystring *param3;
}Params;

/*
* Instruction with operands and opcode
*/
typedef struct instruction{
    Params *params;
    char* opcode;
    struct instruction *next;
}*Instruction;


/**
* @brief Set instruction's opcode, operands
* @param cnt Counter for creating unique names/labels
* @param opcode Instruction's opcode number
* @param ins Instruction's opcode name
* @param op1 First operand
* @param op1 Second operand
* @param op1 Third operand
* @return Error code
*/
int setInstruction(int cnt,int opcode,char* ins,mystring *op1,mystring *op2,mystring *op3);

/**
 * @brief Print instruction
 * @param instruction Instruction to be printed
*/
void printInstruction(Instruction instruction);

#endif