#include <iostream>
struct Example {
    int first;
    int second;
    Example() : second(2), first(second) {} // deliberately wrong list order
};
int main(){ Example x; std::cout << x.first << ' ' << x.second << '\n'; }