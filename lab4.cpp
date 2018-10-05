#include <string>
#include "arrstack.h"

bool bracketCheck(const std::string& s){
    bool check = true;
    Stack<std::string> stack;
    for(int i = 0; i < s.length(); i++){
        char c = s.at(i);
        std::string st(1, c);
        if(c == '(' || c == '[' || c == '{'){
            stack.push(st);
        }
        if(c == ')' || c == ']' || c == '}'){
            if(!stack.isEmpty()){
                std::string n = stack.top();
                if((n == "(" && c == ')') || (n == "[" && c == ']') || (n == "{" && c == '}')){
                    stack.pop();
                    check = true;
                }
                else{
                    check = false;
                }
            }
            else{
                check = false;
            }
        }
        if(!stack.isEmpty()){
            check = false;
        }
    }
    return check;
}
