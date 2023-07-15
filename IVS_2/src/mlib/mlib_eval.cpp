#include "mlib_internal.h"
#include <stack>
#include <math.h>
#include <unordered_map>

//TODO: switch to mlib_power in eval

int precedence(char c){
    if(c == '+' || c == '-') return 0;
    if(c == '*' || c == '/') return 1;
    if(c == '^') return 2;
    if(c == 'n') return 3;
    if(c == '!') return 4;
    if(c == '|') return 5;
    return -1;// c == (
}

int tokenize(std::string &s, std::vector<token_t> &output){

    char buffer[20];
    unsigned buffer_counter = 0;
    char* mess;
    token_t last_token;

    for(int i = 0; i < s.length(); i++){
        char c = s[i];

        if(c == ' ') {
            if(buffer_counter > 0){//end a number thats in buffer
                last_token = {.number = 0.0, .oper = '\0', .isNumber = true};
                buffer[buffer_counter] = '\0';
                last_token.number = strtod(buffer, &mess);
                if(*mess != '\0') return 0;//wrong number literal -> error
                output.push_back(last_token);
                buffer_counter = 0;
            }
            continue;
        }

        if((c >= '0' && c <= '9') || c == '.'){
            buffer[buffer_counter++] = c;
            if(buffer_counter >= 20)//to long number -> err
                return 0;
        }else{
            if(buffer_counter > 0){//end a number thats in buffer
                last_token = {.number = 0.0, .oper = '\0', .isNumber = true};
                buffer[buffer_counter] = '\0';
                last_token.number = strtod(buffer, &mess);
                if(*mess != '\0') return 0;//wrong number literal -> error
                output.push_back(last_token);
                buffer_counter = 0;
            }
            if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '!' || c == 'n' || c == '|' || c == '(' || c == ')'){
                if(!last_token.isNumber){
                    if(last_token.oper == '(' && c == ')') return 0;//empty brackets () -> error
                }
                last_token = {.number = 0, .oper = c, .isNumber = false};
                output.push_back(last_token);
            }else{//unknown operator -> error
                return 0;
            }
        }
    }

    if(buffer_counter > 0){//end a number thats in buffer
        token_t token{0.0,0,true};
        buffer[buffer_counter] = '\0';
        token.number = strtod(buffer, &mess);
        if(*mess != '\0') return 0;//wrong number literal -> error
        output.push_back(token);
    }

    return 1;//-> success
}

int getOutputQueqe(std::vector<token_t> &tokens, std::queue<token_t> &output_queue){
    std::stack<token_t> operator_stack;

    for(int i = 0; i < tokens.size(); i++){
        token_t token = tokens[i];

        if(token.isNumber){
            output_queue.push(token);
        }
        else if(token.oper == '('){
            operator_stack.push(token);
        }else if(token.oper == ')'){
            //pop from stack to queque until '('
            while(!operator_stack.empty() && operator_stack.top().oper != '('){
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.pop();
        }else{
            //pop higher order operators to que
            while(!operator_stack.empty() && precedence(operator_stack.top().oper) >= precedence(token.oper)){
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(token);
        }
    }
    // std::cout<<"\n";

    while(!operator_stack.empty()){
        output_queue.push(operator_stack.top());
        operator_stack.pop();
    }

    return 1;//success
}

double fact(double n){//remove later
    double f = 1;
    for(int i = 2; i <= n; ++i) 
            f *= i;
    return f;
}

int pre_process(std::string &s){
    int left = 0;
    int right = s.length() -1;
    int direction = 1;
    while(left < right){// 
            if(direction == 1){//going right
                if(s[left] == '|'){
                    s[left] = '(';
                    direction = -1;//switch direction
                }left++;
            }else{//going left
                if(s[right] == '|'){
                    s[right] = ')';
                    s.insert(right+1, "|");
                    direction = 1;//switch direction
                }right--;
            }
    }

    
    bool left_arg_exist = false;
    int left_parentheses_count = 0;
    int right_parentheses_count = 0;
    for(int i = 0; i < s.length(); i++){
        char c = s[i];
        if(c >= '0' && c <= '9'){
             left_arg_exist = true;
        }
        else if(c == ')'){
             left_arg_exist = true;
             right_parentheses_count++;
        }
        else if(c == '('){
             left_arg_exist = false;
             left_parentheses_count++;
        }
        else if(c == '-'){
            if(!left_arg_exist) s[i] = 'n';
            //left_arg_exist = false;//
        }else if(c == '*' || c == '/' || c == '+' || c == '^'){//
            left_arg_exist = false;//
        }//
    }

    if(left_parentheses_count != right_parentheses_count) return 0;//fail

    return 1;
}

OPERATION_RESULT eval(std::string &s, double *output){
    if(pre_process(s) == 0) return ERR_MALFORMED_EXPRESSION;

    std::vector<token_t> tokens;
    if(!tokenize(s, tokens)) return ERR_MALFORMED_EXPRESSION;
    if(tokens.size() == 0){//empty input
        *output = 0.0;
        return SUCCESS;
    }
    std::queue<token_t> output_queue;
    if(!getOutputQueqe(tokens, output_queue)) return ERR_MALFORMED_EXPRESSION;

    std::vector<token_t> vv;
    for (std::queue<token_t> dump = output_queue; !dump.empty(); dump.pop()){
        vv.push_back(dump.front());
    }
    
    for (std::vector<token_t>::iterator it = vv.begin(); it != vv.end(); ) {
        if (it->isNumber){ 
            ++it;
        }
        else{
            char oper = it->oper;
            double a,b;

            it = vv.erase(it);
            --it;
            if(oper != '!' && oper != 'n' && oper != '|'){//one argument operators
                if(!it->isNumber){
                    return ERR_MALFORMED_EXPRESSION;
                }
                b = it->number;
                it = vv.erase(it);
                --it;
            }
            if(!it->isNumber){
                return ERR_MALFORMED_EXPRESSION;
            }
            
            a = it->number;
            
            OPERATION_RESULT op_r = SUCCESS;

            switch (oper)
            {
                case '+': op_r = mlib_add(a, b, &(it->number)); break;
                case '-': op_r = mlib_sub(a, b, &(it->number)); break;
                case '*': op_r = mlib_mul(a, b, &(it->number)); break;
                case '/': op_r = mlib_div(a, b, &(it->number)); break;
                case '^': 
                    if(b < 1 && b > -1)
                        op_r = mlib_root(a, (int)1/b, &(it->number));
                    else if(b < 0) 
                        op_r = mlib_power(1.0/a, (int)-b, &(it->number));
                    else
                        op_r = mlib_power(a, (int)b, &(it->number));
                     
                    break;
                case '!': 
                    unsigned long long r; 
                    op_r = mlib_fact(a, &r); 
                    it->number = (double)r; 
                    break;
                case 'n': it->number = -a; break;
                case '|': op_r = mlib_abs(a, &(it->number)); break;
                default: ++it; break;
            }

            if(op_r != SUCCESS){//could not perform operation correctly -> error
                return op_r;
            }

            it->isNumber = true;

        }
    }

    if(vv.size() != 1){
        return ERR_MALFORMED_EXPRESSION;
    }

    if(output != NULL)
        *output = vv[0].number;

    return SUCCESS;
}
