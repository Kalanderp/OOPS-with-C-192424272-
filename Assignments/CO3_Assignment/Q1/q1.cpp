#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Book {
    string title;
    string author;
    int copies;
public:
    Book() : Book("Untitled", "Unknown", 0) {}
    explicit Book(const string& t) : Book(t, "Unknown", 1) {}
    Book(const string& t, const string& a) : Book(t, a, 1) {}
    Book(const string& t, const string& a, int c)
        : title(t), author(a), copies(c < 0 ? throw invalid_argument("copies cannot be negative") : c) {}
    void print() const {
        cout << title << " | " << author << " | copies=" << copies << '\n';
    }
};

int main() {
    Book empty;
    Book donated("The Alchemist");
    Book named("Wings of Fire", "A.P.J. Abdul Kalam");
    Book full("C++ Primer", "Lippman", 5);
    empty.print(); donated.print(); named.print(); full.print();
    try { Book invalid("Invalid", "Test", -2); }
    catch (const invalid_argument& e) { cout << "Rejected: " << e.what() << '\n'; }
}