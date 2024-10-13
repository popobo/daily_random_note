#include <iostream>

class A {
    public:
        A() {std::cout << "1";}
        ~A() {std::cout << "2";}
};

class B {
    public:
        B() {std::cout << "3";}
        ~B() {std::cout << "4";}
        void doB() { std::cout << "B"; }
};

void* operator new(size_t n, B* buf) { 
    buf->doB();
    return buf;
}
void operator delete(void* p, B* buf) { buf->doB(); }

int main() {
    B *pb = new B{};
    A *pa = new (pb)A{};
    delete pa;
}