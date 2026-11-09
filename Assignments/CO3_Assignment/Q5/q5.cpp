#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class Time {
    int seconds;
public:
    explicit Time(int s=0) : seconds((s%86400+86400)%86400) {}
    Time& operator++() { seconds=(seconds+1)%86400; return *this; }
    Time operator++(int) { Time old=*this; ++(*this); return old; }
    bool operator==(const Time& x) const { return seconds==x.seconds; }
    bool operator<(const Time& x) const { return seconds<x.seconds; }
    friend ostream& operator<<(ostream& o,const Time& t) {
        int h=t.seconds/3600,m=(t.seconds/60)%60,s=t.seconds%60;
        return o << (h<10?"0":"") << h << ':' << (m<10?"0":"") << m << ':' << (s<10?"0":"") << s;
    }
};
class Playlist {
    vector<string> tracks;
public:
    Playlist(initializer_list<string> x):tracks(x){}
    string& operator[](int i) {
        if(i<0||i>=static_cast<int>(tracks.size())) throw out_of_range("playlist index out of range");
        return tracks[i];
    }
    const string& operator[](int i) const {
        if(i<0||i>=static_cast<int>(tracks.size())) throw out_of_range("playlist index out of range");
        return tracks[i];
    }
    int size() const { return static_cast<int>(tracks.size()); }
};
int main() {
    Time t(59*60+59); Time a=t++; Time b=++t;
    cout << a << ' ' << b << ' ' << t << '\n';
    Time x(3600); cout << boolalpha << (x==Time(3600)) << ' ' << (Time(3599)<x) << '\n';
    Playlist p{"Track A","Track B","Track C"};
    cout << p[0] << ' ' << p[2] << '\n';
    try { cout << p[-1] << '\n'; } catch(const out_of_range& e) { cout << "Error: " << e.what() << '\n'; }
    try { cout << p[p.size()] << '\n'; } catch(const out_of_range& e) { cout << "Error: " << e.what() << '\n'; }
}