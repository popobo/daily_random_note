#include <thread>
#include <iostream>
#include <chrono>

void thread_sleep(int32_t ms) {
    std::chrono::milliseconds s_time{ms};
    std::this_thread::sleep_for(s_time);
}

class background_task {
    public:
        void operator()() const {
            while(true){
                std::cout << "do something" << std::endl;
                thread_sleep(200);
            }
        }
};


struct func {
    int& x;
    func(int & _x): x(_x) {}
    void operator()() {
        for (unsigned j = 0; j < 100; ++j) {
            std::cout << "i:" << x << std::endl;
        }
    }
};

class thread_guard {
    private:
        std::thread& t;
    public:
        explicit thread_guard(std::thread& _t): t(_t) {}
        
        ~thread_guard() {
            if (t.joinable())
                t.join();
        }

        thread_guard(const std::thread& _t) = delete;
        thread_guard& operator=(const std::thread& _t) = delete;
        thread_guard(std::thread&& _t) = delete;
        thread_guard& operator=(std::thread&& _t) = delete;
};

void fu() {
    int32_t val = 100;
    func my_func{val};
    std::thread my_thread{my_func};
    thread_guard tg(my_thread);
    std::cout << "do something" << std::endl;
}

int main() {
    // std::thread my_thread(background_task()); this is a function declaration, std::thread (*)(background_task (*)())

    /* 2.1.1
    {
        std::thread my_thread{background_task()};
        thread_sleep(1000);// thread is terminated after printing 5 "do something"
        // thread destructor call std::terminated to terminated the whole program
    }

    std::cout << "main thread" << std::endl;
    */

    fu();
    std::cout << "do something" << std::endl;

    return 0;
}