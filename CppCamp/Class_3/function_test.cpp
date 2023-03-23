#include <functional>
#include <iostream>

void f() {
    std::cout << "f" << std::endl;
}

int main() {
    std::function<void()> g{f};
    g();
    std::function<void()> h{std::move(g)};
    h();
    if (!g){
        std::cout << "g is empty" << std::endl;
    }

    g = f;
    g();
}