#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>

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
            std::cout << "x:" << x << std::endl;
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

class scoped_thread {
    private:
        std::thread m_t;
    public:
        explicit scoped_thread(std::thread t): m_t(std::move(t)) {
            if (!m_t.joinable()) // checks joinable here
                throw std::logic_error("No thread");
        }

        ~scoped_thread() {
            m_t.join();
        }
        
        scoped_thread(const scoped_thread& other) = delete;
        scoped_thread& operator=(const scoped_thread& other) = delete;
};

void fu() {
    int32_t val = 100;
    func my_func{val};
    std::thread my_thread{my_func};
    thread_guard tg(my_thread);
    std::cout << "do something" << std::endl;
}

void fu2() {
    int32_t val = 100;
    func my_func{val};
    scoped_thread sth{std::thread{my_func}};
    std::cout << "do something 2" << std::endl;
}


void do_work(int32_t x) {
    for (int32_t i = 0; i < x; ++i) {
        std::cout << "x: " << x << std::endl;
    }
}

void fu3() {
    std::vector<std::thread> threads;
    for (int32_t i = 0; i < 20; ++i) {
        // threads.push_back({do_work, i}); // unaccepted
        // threads.push_back(std::thread{do_work, i}); // accept
        // threads.emplace_back({do_work, i}); // unaccepted
        // threads.emplace_back(std::thread{do_work, i}); // accept
        threads.emplace_back(do_work, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    if (0 == length)
        return init;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<T> results{num_threads};
    std::vector<std::thread> threads{num_threads - 1};
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(
            accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i])
        );
        block_start = block_end;
    }

    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]
    );

    for (auto& entry: threads)
        entry.join();
    return std::accumulate(results.begin(), results.end(), init);
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

    /*
    fu();
    std::cout << "do something" << std::endl;

    fu2();
    std::cout << "do something 2" << std::endl;

    fu3();
    */
    std::vector<int32_t> vec{10000000};
    std::fill(vec.begin(), vec.end(), 1);
    int32_t sum = std::accumulate(vec.begin(), vec.end(), 0);

    return 0;
}