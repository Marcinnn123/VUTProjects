#include "mlib_internal.h"
#include <math.h> 

OPERATION_RESULT mlib_add(double a, double b, double *r){

    double res = a + b;
    if(a > 0 && b > 0 && res < 0)
       return ERR_OVERFLOW;
    if(a < 0 && b < 0 && res > 0)
        return ERR_OVERFLOW;
    *r = res;
    return SUCCESS;
}

OPERATION_RESULT mlib_sub(double a, double b, double *r){
 
    *r = a - b;
    return SUCCESS;
}

OPERATION_RESULT mlib_div(double a, double b, double *r){

    if(b == 0) return ERR_DIVIDE_BY_ZERO;
    *r = a / b;
    return SUCCESS;
}

OPERATION_RESULT mlib_mul(double a, double b, double *r){
    *r = a * b;
    return SUCCESS;
}

OPERATION_RESULT mlib_root(double a, unsigned n, double *r){
    if(n == 0){
        *r = a;
        return SUCCESS;
    }

    if(a == 0){
        *r =  0;
        return SUCCESS;
    }
    if(a < 0){
        if(n%2 != 1) return ERR_NEG_ROOT_ARG;
        *r =  -std::pow(fabs(a), 1.0/n);
        return SUCCESS;
    } 
    *r =  std::pow(a, 1.0/n);

    return SUCCESS;
}

OPERATION_RESULT mlib_power(double a, unsigned n, double *r){

    *r =  pow(a, n);
    return SUCCESS;
}

OPERATION_RESULT mlib_abs(double a, double *r){
    *r = fabs(a);
    return SUCCESS;
}

OPERATION_RESULT mlib_fact(long a, unsigned long long *r){
    if(a < 0)
        return ERR_NEG_FACT_ARG;

    if(a > MAX_FACT_ARG)
        return ERR_TOO_BIG_FACT_ARG;
    if(a == 0) *r = 1;
    unsigned f = 1;
    for(unsigned i = 2; i <= a; ++i) 
            f *= i;

    *r = f;
    return SUCCESS;
}
