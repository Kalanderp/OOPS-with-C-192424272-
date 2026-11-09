#include <iostream>
#include <cstring>
using namespace std;

class Text {
    char* data;
public:
    Text(const char* s="") {
        size_t n=strlen(s); data=new char[n+1]; memcpy(data,s,n+1);
    }
    Text(const Text& other) {
        size_t n=strlen(other.data); data=new char[n+1]; memcpy(data,other.data,n+1);
        cout << "[copy] ";
    }
    Text& operator=(const Text& other) {
        if(this!=&other) {
            size_t n=strlen(other.data); char* fresh=new char[n+1]; memcpy(fresh,other.data,n+1);
            delete[] data; data=fresh;
        }
        return *this;
    }
    ~Text(){ delete[] data; }
    Text& operator+=(const Text& rhs) {
        size_t a=strlen(data),b=strlen(rhs.data); char* fresh=new char[a+b+1];
        memcpy(fresh,data,a); memcpy(fresh+a,rhs.data,b+1); delete[] data; data=fresh; return *this;
    }
    friend Text operator+(const Text& a,const Text& b) { Text result(a); result+=b; return result; }
    friend bool operator==(const Text& a,const Text& b){ return strcmp(a.data,b.data)==0; }
    friend bool operator!=(const Text& a,const Text& b){ return !(a==b); }
    friend ostream& operator<<(ostream& o,const Text& t){ return o<<t.data; }
};
int main(){
    Text a("Hello"),b(" "),c("World");
    Text message=a+b+c;
    cout << "\nmessage: " << message << '\n';
    cout << boolalpha << (message!=a) << '\n';
    Text empty; empty=empty; cout << "empty: [" << empty << "]\n";
    Text longText("A long SMS message can be stored safely with dynamic memory.");
    cout << longText << '\n';
}