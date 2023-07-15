#ifndef MLIB_INTERNAL_H
#define MLIB_INTERNAL_H

#include "mlib.h"

#include <queue>
#include <vector>

#define MAX_FACT_ARG 20

typedef struct{
    double number;/*!< number value, defined only when isNumber = true */
    char oper;/*!< operator value, defined only when isNumber = false*/
    bool isNumber;/*!< determines if the token is a number */
} token_t;

/**
*@return int precedence of operator
*@param c input operator 
*@brief takes in operator and returns it precedence in 
*/
int precedence(char c);

/**
*@return int 1 on success 0 on fail
*@param output vector the output is saved to
*@param s input string
*@brief takes in string and outputs tokens to the vector  
*/
int tokenize(std::string &s, std::vector<token_t> &output);

/**
*@return int 1 on success 0 on fail
*@param output_queue queue the output is saved to
*@param tokens input vector of tokens
*@brief takes in vector of tokens and returns output queue of operands and operators  
*/
int getOutputQueqe(std::vector<token_t> &tokens, std::queue<token_t> &output_queue);

/**
*@return int 1 on success 0 on fail
*@param s input string
*@brief preprocesses text for tokenizations, places 'n' operators and replaces '|x|' with '(x)|'
*/
int pre_process(std::string &s);


#endif