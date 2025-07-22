#ifndef OTHER_HPP
#define OTHER_HPP

#include <string>

std::string makeFormatedFloat(const float& n){
    std::string s = std::to_string(n);
    while(s.back() != '.'){
        if(s.back() == '0'){
            s.pop_back();
        }
        else {
            break;
        }
    }
    if(s.back() == '.'){
        s.pop_back();
    }
    return s;
}

#endif