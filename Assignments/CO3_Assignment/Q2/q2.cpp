#include <iostream>
#include <string>
#include <chrono>
using namespace std;

class Prescription {
    int n;
    string* medicines;
public:
    explicit Prescription(int count) : n(count), medicines(count > 0 ? new string[count] : nullptr) {}
    Prescription(const Prescription& other) : n(other.n), medicines(n > 0 ? new string[n] : nullptr) {
        for (int i=0; i<n; ++i) medicines[i]=other.medicines[i];
        cout << "[copy constructor] ";
    }
    Prescription& operator=(const Prescription& other) {
        if (this != &other) {
            string* fresh = other.n > 0 ? new string[other.n] : nullptr;
            for (int i=0; i<other.n; ++i) fresh[i]=other.medicines[i];
            delete[] medicines;
            medicines=fresh; n=other.n;
        }
        return *this;
    }
    ~Prescription() { delete[] medicines; }
    void set(int i,const string& m) { if(i>=0 && i<n) medicines[i]=m; }
    string get(int i) const { return (i>=0 && i<n)?medicines[i]:"<out-of-range>"; }
    int size() const { return n; }
};
void byValue(Prescription p) { cout << "by value received; "; }
void byConstReference(const Prescription& p) { cout << "by const reference received; "; }

int main() {
    Prescription p(2); p.set(0,"Paracetamol"); p.set(1,"ORS");
    Prescription q=p; q.set(1,"Cetirizine");
    Prescription r(1); r=p; p.set(0,"Ibuprofen");
    cout << p.get(0) << ' ' << p.get(1) << " | " << q.get(0) << ' ' << q.get(1)
         << " | " << r.get(0) << ' ' << r.get(1) << '\n';
    r=r; cout << "self-assignment safe: " << r.get(0) << '\n';
    byValue(p); byConstReference(p); cout << '\n';
}