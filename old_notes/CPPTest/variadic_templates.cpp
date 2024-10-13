#include <iostream>

template <typename... Args>
void print(Args... args) {
  (std::cout << ... << args) << std::endl; 
}

int main()
{
    print(1, 2, "hello");
}