#include <iomanip>
#include <iostream>
#include <stdexcept>
using namespace std;

class Money {
    long long paise;
public:
    Money(long long rupees=0,int p=0) : paise(rupees*100+p) {}
    Money operator+(const Money& rhs) const { return Money(0,static_cast<int>(paise+rhs.paise)); }
    Money operator-(const Money& rhs) const { return Money(0,static_cast<int>(paise-rhs.paise)); }
    Money& operator+=(const Money& rhs) { paise+=rhs.paise; return *this; }
    bool operator==(const Money& rhs) const { return paise==rhs.paise; }
    bool operator<(const Money& rhs) const { return paise<rhs.paise; }
    friend ostream& operator<<(ostream& out,const Money& m) {
        long long v=m.paise; if(v<0) out << "-";
        v= v<0 ? -v : v;
        return out << "Rs " << v/100 << '.' << setw(2) << setfill('0') << v%100 << setfill(' ');
    }
    friend istream& operator>>(istream& in,Money& m) {
        long long r; int p; if(in>>r>>p) m.paise=r*100+p; return in;
    }
};
int main() {
    Money a(120,75),b(79,25),c=a+b;
    cout << c << '\n' << boolalpha << (c==Money(200)) << '\n';
    Money carry(0,75); carry+=Money(0,50); cout << "carry: " << carry << '\n';
    cout << "negative: " << (Money(1,0)-Money(2,25)) << '\n';
    cout << "less-than: " << (Money(0,75)<Money(1,0)) << '\n';
}