#include <iostream>
#include <chrono>
#include <cstdint>
using namespace std;

class Time {
    int h,m;
public:
    Time(int hh,int mm):h(hh),m(mm){}
    friend int minutesBetween(const Time& a,const Time& b);
    friend bool earlier(const Time& a,const Time& b);
};
inline int minutesBetween(const Time& a,const Time& b) {
    int x=a.h*60+a.m, y=b.h*60+b.m;
    return (x>y)?x-y:y-x;
}
inline bool earlier(const Time& a,const Time& b) {
    return a.h*60+a.m < b.h*60+b.m;
}
int main() {
    Time departure(9,45), arrival(13,10);
    cout << minutesBetween(departure,arrival) << ' '
         << boolalpha << earlier(arrival,departure) << '\n';
    Time beforeNoon(11,59), afterNoon(12,1), same(11,59);
    cout << "Across noon: " << minutesBetween(beforeNoon,afterNoon)
         << ", earlier=" << earlier(beforeNoon,afterNoon) << '\n';
    cout << "Same time: " << minutesBetween(beforeNoon,same)
         << ", earlier=" << earlier(beforeNoon,same) << '\n';

    volatile std::uint64_t checksum=0;
    constexpr int N=10000000;
    auto start=chrono::steady_clock::now();
    for(int i=0;i<N;++i) checksum += static_cast<std::uint64_t>(minutesBetween(departure,arrival));
    auto stop=chrono::steady_clock::now();
    auto us=chrono::duration_cast<chrono::microseconds>(stop-start).count();
    cout << "10M inline calls elapsed_us=" << us << " checksum=" << checksum << '\n';
}
