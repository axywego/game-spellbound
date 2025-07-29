#ifndef OTHER_HPP
#define OTHER_HPP

#include <iostream>
#include <string>
#include <chrono>

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


// including "from" and "to"
std::uint64_t generate8Bytes(const uint64_t& from, const uint64_t& to) {

    auto now = std::chrono::high_resolution_clock::now();
    std::uint64_t nanos = std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();
    
    nanos = (nanos >> 32) ^ (nanos & 0xFFFFFFFF);
    nanos = nanos * 0x45d9f3b;
    auto newRand = std::hash<std::uint64_t>{}(nanos);
    nanos = nanos ^ (nanos >> 16);
    
    return from + (nanos % (to - from + 1));
}

#endif