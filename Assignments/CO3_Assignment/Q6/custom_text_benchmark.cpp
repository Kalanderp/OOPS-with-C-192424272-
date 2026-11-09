#include <chrono>
#include <cstring>
#include <iostream>
class Text {
    char* data;
public:
    Text():data(new char[1]{'\0'}){}
    Text& operator+=(const char* rhs){
        size_t a=std::strlen(data), b=std::strlen(rhs);
        char* fresh=new char[a+b+1];
        std::memcpy(fresh,data,a); std::memcpy(fresh+a,rhs,b+1);
        delete[] data; data=fresh; return *this;
    }
    ~Text(){delete[] data;}
    size_t size() const{return std::strlen(data);}
};
int main(){
    Text t; auto a=std::chrono::steady_clock::now();
    for(int i=0;i<100000;++i)t+="x";
    auto b=std::chrono::steady_clock::now();
    std::cout<<"custom Text length="<<t.size()<<" time_us="
      <<std::chrono::duration_cast<std::chrono::microseconds>(b-a).count()<<'\n';
}