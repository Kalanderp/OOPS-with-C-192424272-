struct Book { explicit Book(int n) : copies(n) {} int copies; };
int main(){ Book shelf[3]; } // expected compile error: no default constructor