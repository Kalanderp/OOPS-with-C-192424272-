#include <string>
struct Book { explicit Book(const std::string&) {} };
int main(){ Book a("Title"); /* Book b = "Title"; // rejected when explicit */ }