#include <chrono>
#include <iostream>
#include <iomanip>
int main(){
    constexpr int N=1000000;
    volatile double d=0.0; auto a=std::chrono::steady_clock::now();
    for(int i=0;i<N;++i) d+=0.01;
    auto b=std::chrono::steady_clock::now();
    volatile long long p=0; auto c=std::chrono::steady_clock::now();
    for(int i=0;i<N;++i) p+=1;
    auto e=std::chrono::steady_clock::now();
    std::cout<<std::setprecision(15)<<"double result="<<d<<" time_us="
      <<std::chrono::duration_cast<std::chrono::microseconds>(b-a).count()<<'\n';
    std::cout<<"integer paise="<<p<<" time_us="
      <<std::chrono::duration_cast<std::chrono::microseconds>(e-c).count()<<'\n';
}