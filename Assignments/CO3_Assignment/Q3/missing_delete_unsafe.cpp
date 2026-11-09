#include <iostream>
int main(){
    int rows=10,seats=20;
    char** map=new char*[rows];
    for(int r=0;r<rows;++r) map[r]=new char[seats];
    // Intentionally omitted delete[] operations: memory leak demonstration.
    std::cout << "Allocated " << rows*seats << " seat cells without releasing them\\n";
}