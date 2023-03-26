#include <iostream>
#include <vector>

class A {
    public:
        A() = default;
        A(const A& rhs) {
            std::cout << "copy constructor" << std::endl;
        }
        
        A(const A&& rhs) {
            std::cout << "move constructor" << std::endl;
        }
};

template <typename T>
void testForward(T&& v) {}

template <typename T>
void print(T& t) {
    std::cout << "Lvalue ref" << std::endl;
}

template <typename T>
void print(T&& t) {
    std::cout << "Rvalue ref" << std::endl;
}

template <typename T>
void testForward1(T&& v) {
    std::cout << "==============" << std::endl;
    print(v);
    print(std::forward<T>(v));
    print(std::move(v));
    std::cout << "--------------" << std::endl;
}


int main() {
    std::vector<A> vec;
    vec.reserve(20);
    A temp;
                                 // 调用的构造函数，调用的vector的方法
    vec.push_back(temp); // copy , push_back(const value_type& __x)
    vec.push_back(std::move(temp)); // move , push_back(value_type&& __x)
    vec.push_back(A()); //move , push_back(value_type&& __x)
    vec.emplace_back(temp); // copy , emplace_back(_Args&&... __args);
    vec.emplace_back(std::move(temp)); //move , emplace_back(_Args&&... __args);
    vec.emplace_back(A()); // move , emplace_back(_Args&&... __args);

    // testForward(1);
    // int x = 1;
    // testForward(x);
    // const int& rx = x;
    // testForward(rx);

    int x = 1;
    testForward1(x);
    testForward1(std::move(x));    
}