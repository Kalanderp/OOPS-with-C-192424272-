#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace std;

class Complex {
    double re, im;
public:
    Complex() : re(0), im(0) {}
    Complex(double real) : re(real), im(0) {}
    Complex(double real,double imag) : re(real), im(imag) {}
    Complex operator+(const Complex& z) const { return {re+z.re,im+z.im}; }
    Complex operator-(const Complex& z) const { return {re-z.re,im-z.im}; }
    Complex operator*(const Complex& z) const { return {re*z.re-im*z.im,re*z.im+im*z.re}; }
    Complex operator/(const Complex& z) const {
        double d=z.re*z.re+z.im*z.im;
        if(d==0.0) throw domain_error("division by zero impedance");
        return {(re*z.re+im*z.im)/d,(im*z.re-re*z.im)/d};
    }
    bool operator==(const Complex& z) const { return re==z.re && im==z.im; }
    double magnitude() const { return hypot(re,im); }
    friend ostream& operator<<(ostream& o,const Complex& z) {
        o << z.re; if(z.im>=0) o << " + " << z.im << "j"; else o << " - " << -z.im << "j";
        return o;
    }
};
int main(){
    const double w=100.0,L=0.1,C=0.001;
    Complex R(10,0), XL(0,w*L), XC(0,-1.0/(w*C));
    Complex series=R+XL+XC;
    Complex parallel=(R*XL)/(R+XL);
    cout << "R = " << R << '\n' << "L = " << XL << '\n' << "C = " << XC << '\n';
    cout << "Series impedance = " << series << ", |Z| = " << series.magnitude() << '\n';
    cout << "Parallel R-L impedance = " << parallel << ", |Z| = " << parallel.magnitude() << '\n';
    try { Complex bad=R/Complex(); cout << bad << '\n'; }
    catch(const domain_error& e){ cout << "Safe error: " << e.what() << '\n'; }
}