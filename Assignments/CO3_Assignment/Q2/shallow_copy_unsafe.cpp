#include <string>
struct Prescription {
    int n; std::string* medicines;
    explicit Prescription(int x):n(x),medicines(new std::string[x]){}
    ~Prescription(){ delete[] medicines; }
    // No copy constructor: compiler copies the pointer (shallow copy).
};
int main(){ Prescription p(2); Prescription q=p; } // intentionally unsafe: double delete