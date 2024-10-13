#include <thread>
#include <iostream>
#include <mutex>

template<typename F>
class A {
    private:
        std::mutex m; // this is right
        std::thread t;
        // std::mutex m;
    public:
        A(F&& f):t([&](){
            // mutex may not be initialized here which causes a system error
            std::lock_guard<std::mutex> g(m);
            f();
        })
        {}

        ~A() {
            t.join();
        }
};



int main() {
    A a{[](){ std::cout << "hello" << std::endl; }};
    return 0;
}