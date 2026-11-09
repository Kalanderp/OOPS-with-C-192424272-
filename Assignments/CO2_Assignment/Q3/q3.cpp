#include <iostream>
#include <iomanip>
using namespace std;

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};
double charge(int grams, bool express=false, bool insured=false) {
    double base=(grams/1000.0)*50.0;
    return base + (express?40.0:0.0) + (insured?15.0:0.0);
}
double charge(double weightKg, bool express=false, bool insured=false) {
    double base=weightKg*50.0;
    return base + (express?40.0:0.0) + (insured?15.0:0.0);
}
double charge(const Parcel& p) {
    return charge(p.weightKg,p.express,p.insured);
}
int main() {
    cout << fixed << setprecision(2);
    cout << "charge(2) [int grams]: " << charge(2) << '\n';
    cout << "charge(2.0f) [float promoted to double]: " << charge(2.0f) << '\n';
    cout << "charge(2.0) [double kg]: " << charge(2.0) << '\n';
    cout << "charge(1500, true) [grams, express]: " << charge(1500,true) << '\n';
    cout << "charge(1.5, false, true) [kg, insured]: " << charge(1.5,false,true) << '\n';
    Parcel p{2.0,true,true};
    cout << "charge(Parcel): " << charge(p) << '\n';
}
