#include <iostream>
#include <type_traits>
#include <string>

int main() {
    int x;
    int& y = x;
    int&& z = std::move(x);
    auto&& f = []{};

    std::cout << std::is_reference<decltype(x)>::value << std::endl;
    std::cout << std::is_reference<decltype((x))>::value << std::endl;
    std::cout << std::is_reference<decltype(y)>::value << std::endl;
    std::cout << std::is_reference<decltype((y))>::value << std::endl;
    std::cout << std::is_reference<decltype(z)>::value << std::endl;
    std::cout << std::is_reference<decltype((z))>::value << std::endl;
    std::cout << std::is_reference<decltype(f)>::value << std::endl;
    std::cout << std::is_reference<decltype((f))>::value << std::endl;

    std::cout << "------------" << std::endl;

    std::cout << std::is_rvalue_reference<decltype(x)>::value << std::endl;
    std::cout << std::is_rvalue_reference<decltype((x))>::value << std::endl;
    std::cout << std::is_rvalue_reference<decltype(y)>::value << std::endl;
    std::cout << std::is_rvalue_reference<decltype((y))>::value << std::endl;
    std::cout << std::is_rvalue_reference<decltype(z)>::value << std::endl; //右值引用
    std::cout << std::is_rvalue_reference<decltype((z))>::value << std::endl;
    std::cout << std::is_rvalue_reference<decltype(f)>::value << std::endl; //右值引用
    std::cout << std::is_rvalue_reference<decltype((f))>::value << std::endl;
}