#include <iostream>
#include "mlib/mlib.h"
#include <math.h>

using namespace std;

int main()
{
int x = 0; // counter of stdin numbers [int]
double N = 0.0; // counter of stdin numbers [double]
double number = 0.0; //stdin number
double sum = 0.0; // summary of numbers with one extra number  
double sum2 = 0.0; // summary of numbers with one extra number ^2
double fixed_sum = 0.0; // real summary / N
double fixed_sum2 = 0.0; // real summary ^2
double powered_number = 0.0; // number^2
double fixed_sum_tmp = 0.0; //sum -1 
double powered_fixed_sum = 0.0; // fixed sum ^2
double powered_fixed_sum_N = 0.0; // (fixed sum ^2) * N
double divide = 0.0; // N - 1
double element = 0.0; // fixed_sum2 - powered_fixed_sum_N 
double root_element = 0.0; // element/divide
double s = 0.0; // result



while(!feof(stdin))
{
cin >> number;
x++;
////////////////////////
// sum += number
if(mlib_add(sum, number, &sum) != SUCCESS)
{
    return 1;
}
////////////////////////
// powered number
if(mlib_power(number, 2, &powered_number) != SUCCESS)
{
    return 1;
}
/////////////////////////
// sum2 += powered_number 
if(mlib_add(sum2, powered_number, &sum2) != SUCCESS)
{
    return 1;
}
} // end of while loop
if(mlib_sub(x, 1, &N) != SUCCESS)
{
    return 1;
}
// double fixed_sum = (sum - number)/N
if(mlib_sub(sum, number, &fixed_sum_tmp) != SUCCESS)
{
    return 1;
}
if(mlib_div(fixed_sum_tmp, N, &fixed_sum) != SUCCESS)
{
    return 1;
}
//////////////////////
//double fixed_sum2 = (sum2 - (number*number));
if(mlib_sub(sum2, powered_number, &fixed_sum2) != SUCCESS)
{
    return 1;
}
//////////////////////
// double element = fixed_sum2 - (fixed_sum*fixed_sum)*N;
if(mlib_power(fixed_sum, 2, &powered_fixed_sum) != SUCCESS)
{
    return 1;
}
if(mlib_mul(powered_fixed_sum, N, &powered_fixed_sum_N) != SUCCESS)
{
    return 1;
}

if(mlib_sub(fixed_sum2, powered_fixed_sum_N, &element) != SUCCESS)
{
    return 1;
}
////////////////////////
// double divide = N -1
// double root_element = element/divide
// double s = sqrt(root_element);
if(mlib_sub(N, 1, &divide) != SUCCESS)
{
    return 1;
}
if(mlib_div(element, divide, &root_element) != SUCCESS)
{
    return 1;
}
if(mlib_root(root_element, 2, &s) != SUCCESS)
{
    return 1;
}
////////////////////////

cout << s << endl;
    return 0;
}

