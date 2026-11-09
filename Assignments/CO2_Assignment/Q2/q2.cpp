#include <iostream>
#include <cstddef>
using namespace std;

int& highestMark(int marks[], size_t n) {
    size_t best=0;
    for (size_t i=1;i<n;++i) if (marks[i]>marks[best]) best=i;
    return marks[best];
}
const int& highestMarkConst(const int marks[], size_t n) {
    size_t best=0;
    for (size_t i=1;i<n;++i) if (marks[i]>marks[best]) best=i;
    return marks[best];
}
int highestMarkCopy(const int marks[], size_t n) {
    int best=marks[0];
    for (size_t i=1;i<n;++i) if (marks[i]>best) best=marks[i];
    return best;
}
int* highestMarkPointer(int marks[], size_t n) {
    size_t best=0;
    for (size_t i=1;i<n;++i) if (marks[i]>marks[best]) best=i;
    return &marks[best];
}
int highestIndex(const int marks[], size_t n) {
    size_t best=0;
    for (size_t i=1;i<n;++i) if (marks[i]>marks[best]) best=i;
    return static_cast<int>(best);
}
// Deliberately unsafe example is explained in the report and is NOT called.
// int& danglingReference() { int local=10; return local; }

int main() {
    int marks[]={72,91,84,88,91};
    const size_t n=sizeof(marks)/sizeof(marks[0]);
    cout << "Highest before moderation: " << highestMark(marks,n) << '\n';
    highestMark(marks,n) += 2;
    cout << "Array after +2 moderation: ";
    for (int x:marks) cout << x << ' ';
    cout << '\n';
    cout << "Highest as const reference: " << highestMarkConst(marks,n) << '\n';
    cout << "Highest as copy: " << highestMarkCopy(marks,n) << '\n';
    int idx=highestIndex(marks,n);
    marks[idx] += 1;
    cout << "Index method: index=" << idx << ", updated=" << marks[idx] << '\n';
    int* ptr=highestMarkPointer(marks,n);
    cout << "Pointer method: " << *ptr << '\n';
}
