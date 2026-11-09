#include <iostream>
#include <string>
using namespace std;

class Event {
    int rows, seats, bookedCount;
    char** map;
    string name;
public:
    Event(string n,int r,int s) : rows(r>0?r:0), seats(s>0?s:0), bookedCount(0), map(nullptr), name(n) {
        if(rows>0 && seats>0) {
            map=new char*[rows];
            int made=0;
            try {
                for(;made<rows;++made) map[made]=new char[seats]();
            } catch(...) {
                while(made>0) delete[] map[--made];
                delete[] map; map=nullptr; throw;
            }
        }
        cout << "Construct " << name << " (" << rows << "x" << seats << ")\n";
    }
    bool book(int r,int s) {
        if(r<0||r>=rows||s<0||s>=seats||map[r][s]=='B') return false;
        map[r][s]='B'; ++bookedCount; return true;
    }
    bool cancel(int r,int s) {
        if(r<0||r>=rows||s<0||s>=seats||map[r][s]!='B') return false;
        map[r][s]=0; --bookedCount; return true;
    }
    ~Event() {
        if(map) { for(int r=0;r<rows;++r) delete[] map[r]; }
        delete[] map;
        cout << "Close " << name << "; booked=" << bookedCount << '\n';
    }
};
int main() {
    Event stackEvent("Stack",2,3);
    stackEvent.book(0,1); stackEvent.book(1,2); stackEvent.cancel(0,1);
    { Event blockEvent("Block",1,2); blockEvent.book(0,0); }
    Event* heapEvent=new Event("Heap",2,2);
    heapEvent->book(1,1); delete heapEvent;
    Event zeroEvent("ZeroRows",0,5);
}