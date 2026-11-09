#include <iostream>
using namespace std;

void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100;
}
void applyByReference(long long& fee, int percent) {
    fee -= fee * percent / 100;
}
void applyByPointer(long long* fee, int percent) {
    if (fee != nullptr) *fee -= (*fee) * percent / 100;
}
long long applyAndReturn(long long fee, int percent) {
    return fee - fee * percent / 100;
}
void change(int a, int& b, int* c) {
    a += 10; b += 10; *c += 10;
}
int& bigger(int& p, int& q) { return p > q ? p : q; }
void show(int x, int y = 5, int z = 10) { cout << x + y + z << ' '; }

int main() {
    cout << "PASSING METHODS\n";
    long long f1=45000, f2=45000, f3=45000;
    applyByValue(f1,10);
    applyByReference(f2,10);
    applyByPointer(&f3,10);
    cout << "By value: " << f1 << '\n';
    cout << "By reference: " << f2 << '\n';
    cout << "By pointer: " << f3 << '\n';
    cout << "Return new fee (15%): " << applyAndReturn(45000,15) << "\n\n";

    cout << "PREDICT THE OUTPUT PROGRAM\n";
    int a=1,b=2,c=3;
    change(a,b,&c);
    cout << a << ' ' << b << ' ' << c << '\n';
    bigger(a,c)=100;
    cout << a << ' ' << c << '\n';
    show(1); show(1,2); show(1,2,3); cout << '\n';

    cout << "\nCOMPLETE THE CODE\n";
    long long hostelFee=45000;
    applyByValue(hostelFee,10);
    cout << "after call by value : " << hostelFee << '\n';
    applyByReference(hostelFee,10);
    cout << "after call by reference: " << hostelFee << '\n';
}
