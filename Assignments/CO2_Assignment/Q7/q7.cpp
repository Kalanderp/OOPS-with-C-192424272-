#include <iostream>
#include <array>
#include <chrono>
using namespace std;

class ShiftHistory {
    static constexpr int K=10;
    array<int,K> data{};
    int count=0;
public:
    void add(int value) {
        if(count<K) data[count++]=value;
        else {
            for(int i=1;i<K;++i) data[i-1]=data[i];
            data[K-1]=value;
        }
    }
    int size() const { return count; }
    int oldest() const { return count?data[0]:-1; }
};
class CircularHistory {
    static constexpr int K=10;
    array<int,K> data{};
    int count=0, next=0;
public:
    void add(int value) {
        data[next]=value;
        next=(next+1)%K;
        if(count<K) ++count;
    }
    int size() const { return count; }
    int oldest() const { return count==K?data[next]:(count?data[0]:-1); }
};
template<class History>
long long benchmark() {
    History h;
    constexpr int N=5000000;
    auto start=chrono::steady_clock::now();
    for(int i=0;i<N;++i) h.add(i);
    auto end=chrono::steady_clock::now();
    volatile int evidence=h.oldest()+h.size();
    (void)evidence;
    return chrono::duration_cast<chrono::microseconds>(end-start).count();
}
int main() {
    ShiftHistory original;
    CircularHistory redesigned;
    for(int i=1;i<=12;++i) { original.add(i); redesigned.add(i); }
    cout << "Same test: 12 insertions into capacity 10\n";
    cout << "Shift oldest=" << original.oldest() << ", count=" << original.size() << '\n';
    cout << "Circular oldest=" << redesigned.oldest() << ", count=" << redesigned.size() << '\n';
    auto oldUs=benchmark<ShiftHistory>();
    auto newUs=benchmark<CircularHistory>();
    cout << "5M insertion benchmark (microseconds)\n";
    cout << "Original shift: " << oldUs << '\n';
    cout << "Redesigned circular: " << newUs << '\n';
    cout << "Both retain the latest 10 values; circular insertion uses O(1) work.\n";
}
