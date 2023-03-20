#include <cinttypes>
#include <iostream>
#include <variant>

#define PRINT_FUNC_ADDR std::cout << __func__ << " at " << this << std::endl

class A {
    int m_a{1};
    int m_b{2};
    int m_c{3};

public:
    A(int a, int b, int c);
    A(const A& other);
    A();
    A& operator=(const A& other);
    ~A();

    void Dump();
    void DoSomething();
};

A::A(int a, int b, int c): m_a{a}, m_b{b}, m_c{c} {
    PRINT_FUNC_ADDR;
}

//A::A(const A &other): m_a{other.m_a}, m_b{other.m_b}, m_c{other.m_d}, m_d{other.m_d} {}

// 委託構造
A::A(const A &other): A(other.m_a, other.m_b, other.m_c) {
    PRINT_FUNC_ADDR;
} 

A::A() {
    PRINT_FUNC_ADDR;
}

A::~A() {
    PRINT_FUNC_ADDR;
}

A &A::operator=(const A &other) {
    m_a = other.m_a;
    m_b = other.m_b;
    m_c = other.m_c;
    PRINT_FUNC_ADDR;
    return *this;
}

void A::Dump()
{
    auto dumpItem = [this](auto name, auto &item) {
        std::cout << "this is " <<  this << "@" << (void *)&item << ", size is " << sizeof(item) << std::endl;
    };

    dumpItem("m_a", m_a);
    dumpItem("m_b", m_b);
    dumpItem("m_c", m_c);
    
    std::cout << "Object size: " << sizeof(*this) << std::endl;
}

void A::DoSomething() {
    std::cout << "do something" << std::endl;
}
class B: public A {
    int m_ba = 3;
public:
    void DoSomething();
};

void B::DoSomething() {
    std::cout << m_ba << std::endl;
}

void Func(A item) {
    item.DoSomething();
}

int main() {
    std::cout << __LINE__ << std::endl;

    A a{};
    B b{};
    a = b;
    
    // A *pa = new B{};
    // delete pa;

    std::cout << __LINE__ << std::endl;
    return 0;
}