#include <chrono>
#include <iostream>
#include <string>
#include <cstring>
int main(){
    const int N=100000;
    auto a=std::chrono::steady_clock::now();
    std::string s; s.reserve(N);
    for(int i=0;i<N;++i) s+='x';
    auto b=std::chrono::steady_clock::now();
    std::cout<<"std::string length="<<s.size()<<" time_us="
      <<std::chrono::duration_cast<std::chrono::microseconds>(b-a).count()<<'\n';
    // Text::operator+= in q6.cpp reallocates each time; O(N^2) character copying.
    std::cout<<"Custom Text repeated += is O(N^2) without capacity management.\n";
}