#include <iostream>
#include <string>
using namespace std;

class BusTicket {
    static int issued;
    static const int CAPACITY=3;
    int seat;
    string name;
public:
    explicit BusTicket(const string& n):seat(0),name(n) {
        if (issued<CAPACITY) seat=++issued;
    }
    static int count() { return issued; }
    void print() const {
        cout << name << ": " << (seat ? "Seat "+to_string(seat) : string("Waiting")) << '\n';
    }
};
int BusTicket::issued=0;

class Token {
    static int next;
    int id;
public:
    Token():id(++next){}
    int get() const { return id; }
    static int issued() { return next-100; }
};
int Token::next=100;

void createBatch(const string& label) {
    cout << label << '\n';
    BusTicket local[2]={BusTicket("Visitor-1"),BusTicket("Visitor-2")};
    for (const auto& t:local) t.print();
    cout << "Cumulative seats issued: " << BusTicket::count() << '\n';
}
int main() {
    Token counter[3];
    Token extra;
    cout << "Token prediction: " << counter[0].get() << ' ' << counter[2].get()
         << ' ' << extra.get() << ' ' << Token::issued() << '\n';

    BusTicket trip[4]={BusTicket("Asha"),BusTicket("Bala"),BusTicket("Chitra"),BusTicket("Dev")};
    for (const auto& t:trip) t.print();
    cout << "Issued: " << BusTicket::count() << '\n';
    createBatch("Function call 1:");
    createBatch("Function call 2:");
}
