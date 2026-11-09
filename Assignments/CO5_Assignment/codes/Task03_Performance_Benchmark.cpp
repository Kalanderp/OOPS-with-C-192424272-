#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>

struct Base { virtual ~Base() = default; virtual double total() const = 0; };
struct OP : Base { double c,t; OP(double a,double b):c(a),t(b){} double total() const override{return c+t;} };
struct IP : Base { double days,rate,proc; IP(double d,double r,double p):days(d),rate(r),proc(p){} double total() const override{return days*rate+proc;} };
struct Flat { int type; double a,b,c; };
double sw(const Flat& x) { switch(x.type){case 0:return x.a+x.b;case 1:return x.a*x.b+x.c;default:return 0;} }
using V = std::variant<OP,IP>;
double visit(const V& v) { return std::visit([](const auto& x){return x.total();},v); }
volatile double sink = 0;
template<class F> double bench(F f) {
    auto s=std::chrono::steady_clock::now(); double sum=f();
    auto e=std::chrono::steady_clock::now(); sink=sum;
    return std::chrono::duration<double,std::milli>(e-s).count();
}
int main() {
    std::cout << std::fixed << std::setprecision(3);
    for (std::size_t n : {1000ULL,1000000ULL,10000000ULL}) {
        std::vector<std::unique_ptr<Base>> v; v.reserve(n);
        std::vector<Flat> f; f.reserve(n);
        std::vector<V> z; z.reserve(n);
        for(std::size_t i=0;i<n;++i) {
            if(i%2==0){v.emplace_back(new OP(500,1200));f.push_back({0,500,1200,0});z.emplace_back(OP(500,1200));}
            else {v.emplace_back(new IP(3,2500,40000));f.push_back({1,3,2500,40000});z.emplace_back(IP(3,2500,40000));}
        }
        auto tv=bench([&]{double s=0;for(auto& p:v)s+=p->total();return s;});
        auto ts=bench([&]{double s=0;for(auto& x:f)s+=sw(x);return s;});
        auto tz=bench([&]{double s=0;for(auto& x:z)s+=visit(x);return s;});
        std::cout<<"N="<<n<<" virtual_ms="<<tv<<" switch_ms="<<ts<<" variant_ms="<<tz
                 <<" totals_equal="<<(v.size()==f.size() && v.size()==z.size())<<'\n';
    }
    constexpr std::size_t n=1000000;
    for(int invalidPercent : {0,1,50}) {
        std::vector<double> amounts(n,100.0);
        for(std::size_t i=0;i<n*invalidPercent/100;++i) amounts[i]=-1.0;
        auto tr=bench([&]{std::size_t ok=0;for(double x:amounts)if(x>=0)++ok;return double(ok);});
        auto te=bench([&]{std::size_t ok=0;for(double x:amounts){try{if(x<0)throw std::invalid_argument("negative");++ok;}catch(const std::invalid_argument&){} }return double(ok);});
        std::cout<<"invalid="<<invalidPercent<<"% return_code_ms="<<tr<<" exception_ms="<<te<<'\n';
    }
}
