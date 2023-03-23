#include <chrono>
#include <iostream>
#include <vector>
#include <string>

class RaiiTimer {
    public:
        RaiiTimer(): start{std::chrono::steady_clock::now()} {
            std::cout << "-------------" << std::endl;
        }

        ~RaiiTimer() {
            std::cout << static_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start).count() << std::endl;
        }

    private:
        std::chrono::time_point<std::chrono::steady_clock> start;
};

int main() {
    //RaiiTimer timer;
    std::vector<std::string> v1;
    for (int i = 0; i < 10000000; ++i) {
        v1.push_back("ABC123");
    }

    {
        std::vector<std::string> v2;
        RaiiTimer timer1;
        v2 = v1; // 拷贝赋值
    }

    {
        std::vector<std::string> v3;
        RaiiTimer timer1;
        v3 = std::move(v1); // 移动赋值
    }
}