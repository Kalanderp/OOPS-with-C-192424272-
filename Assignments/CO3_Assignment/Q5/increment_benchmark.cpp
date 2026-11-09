#include <chrono>
#include <iostream>
struct Counter {
    long long v=0;
    Counter& operator++(){ ++v; return *this; }
    Counter operator++(int){ Counter old=*this; ++v; return old; }
};
int main(){
    constexpr int N=10000000; volatile long long sink=0;
    Counter a; auto s=std::chrono::steady_clock::now();
    for(int i=0;i<N;++i) sink+=(++a).v;
    auto m=std::chrono::steady_clock::now();
    Counter b; for(int i=0;i<N;++i) sink+=(b++).v;
    auto e=std::chrono::steady_clock::now();
    std::cout<<"prefix_us="<<std::chrono::duration_cast<std::chrono::microseconds>(m-s).count()<<'\n';
    std::cout<<"postfix_us="<<std::chrono::duration_cast<std::chrono::microseconds>(e-m).count()<<'\n';
    std::cout<<"checksum="<<sink<<'\n';
}