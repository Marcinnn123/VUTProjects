#include <iostream>
#include "mlib/mlib.h"

int main(){
    while(true){
        std::cout<< "enter expression: ";
        std::string input;
        getline(std::cin, input);
        double output;
        OPERATION_RESULT op = eval(input, &output);
        if(op == SUCCESS){
            std::cout<<"result: "<<output<<'\n';
        }else if(op == ERR_MALFORMED_EXPRESSION){
            std::cout<<"ERROR: malformed expression"<<'\n';
        }else{
            std::cout<<"ERROR: some operation has failed"<<'\n';
        }
    }
    return 0;
}
