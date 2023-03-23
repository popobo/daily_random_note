#include <string>
#include <iostream>

int main() {
    std::string str0{"hello"};

    std::string&& str1 = std::move(str0);
    str0+="!";

    std::cout << str0;
    std::cout << str1;

    std::cout << "---------" << std::endl;
    std::string str2{str1};
    std::cout << str0 << std::endl;
    std::cout << str1 << std::endl;
    std::cout << str2 << std::endl;

    std::cout << "---------" << std::endl;
    std::string str3{std::move(str1)}; // str1是str0的（右值）引用，左值，即T&&跟int一样是一个types, 所以相当于把str0移动到str3
    std::cout << str0 << " is " << str0.empty() << std::endl;
    std::cout << str1 << " is " << str1.empty() << std::endl;
    std::cout << str2 << std::endl;
    std::cout << str3 << std::endl;
}