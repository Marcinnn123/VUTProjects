#ifndef MLIB_H
#define MLIB_H

#include <string>

typedef enum {SUCCESS, ERR_OVERFLOW, ERR_TOO_BIG_FACT_ARG, ERR_DIVIDE_BY_ZERO, ERR_NEG_ROOT_ARG, ERR_MALFORMED_EXPRESSION, ERR_ROOT_BY_ZERO, ERR_NEG_FACT_ARG} OPERATION_RESULT;

/**
*@param a prvni operand při sčitaní
*@param b druhy operand při sčitaní
*@param r vysledek sčitaní 
*@brief funkce pro sčitaní dvou operandu 
*/
OPERATION_RESULT mlib_add(double a, double b, double *r);
/**
*@param a prvni operand při odčitaní
*@param b druhy operand při odčitaní
*@param r vysledek odčitaní 
*@brief funkce pro odčitaní druheho operandu od prvního  
*/
OPERATION_RESULT mlib_sub(double a, double b, double *r);
/**
*@param a dělenec  
*@param b dělitel
*@param r vysledek dělení 
*@brief funkce pro dělení prvního operandu druhým   
*/
OPERATION_RESULT mlib_div(double a, double b, double *r);
/**
*@param a prvni operand při nasobení
*@param b druhy operand při nasobení
*@param r vysledek nasobení 
*@brief funkce pro nasobení prvního operandu druhým 
*/
OPERATION_RESULT mlib_mul(double a, double b, double *r);
/**
*@param a operand pod odmocninou 
*@param b exponent  
*@param r vysledek odmocňovaní   
*@brief funkce pro odmocňovaní
*/
OPERATION_RESULT mlib_root(double a, unsigned n, double *r);
/**
*@param a operand pod mocninou
*@param b mocnitel  
*@param r vysledek umocňovaní   
*@brief funkce pro umocňovaní
*/
OPERATION_RESULT mlib_power(double a, unsigned n, double *r);
/**
*@param a operand uvnitř absolutní hodnoty   
*@param r vysledek absolutní hodnoty  
*@brief funkce pro absolutní hodnotu
*/
OPERATION_RESULT mlib_abs(double a, double *r);
/**
*@param a číslo faktorialu  
*@param r vysledek faktorialu  
*@brief funkce pro faktorial
*/
OPERATION_RESULT mlib_fact(long a, unsigned long long *r);


/**
*@return OPERATION_RESULT SUCCESS on success, MALFORMED_EXPRESSION if expression is malformed, other error if some operation fails
*@param s input expression to evaluate
*@param output pointer where the result will be saved on success
*@brief takes in string expression, evaluates it, saves result and return success or error   
*/
OPERATION_RESULT eval(std::string &s, double *output);

#endif