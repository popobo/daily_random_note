#include <memory>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <thread>

template<typename T>
class thread_safe_queue {
    private:
        mutable std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable data_cond;
    public:
        thread_safe_queue() = default;
        thread_safe_queue(const thread_safe_queue&);
        thread_safe_queue& operator=(const thread_safe_queue&) = delete;
        thread_safe_queue(thread_safe_queue&&) = delete;
        thread_safe_queue& operator=(thread_safe_queue&&) = delete;
        void push(T new_value);
        bool try_pop(T& value);
        std::shared_ptr<T> try_pop();
        void wait_and_pop(T& value);
        std::shared_ptr<T> wait_and_pop();
        bool empty() const;      
};

template <typename T>
thread_safe_queue<T>::thread_safe_queue(const thread_safe_queue &other)
{
    std::lock_guard lk{other.mut};
}

template <typename T>
void thread_safe_queue<T>::push(T new_value)
{
    std::lock_guard lk{mut};
    data_queue.push(new_value);
    data_cond.notify_one();
}

template <typename T>
bool thread_safe_queue<T>::try_pop(T &value)
{
    std::lock_guard lk{mut};
    if (data_queue.empty()) {
        return false;
    }
    value = data_queue.front();
    data_queue.pop();
    return true;
}

template <typename T>
std::shared_ptr<T> thread_safe_queue<T>::try_pop()
{
    std::lock_guard lk{mut};
    if (data_queue.empty()) {
        return std::shared_ptr<T>();
    }
    std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
    data_queue.pop();
    return true;
}

template <typename T>
void thread_safe_queue<T>::wait_and_pop(T &value)
{
    std::unique_lock lk{mut};
    data_cond.wait(lk, [this]{ return !data_queue.empty(); });
    value = data_queue.front();
    data_queue.pop();
}

template <typename T>
std::shared_ptr<T> thread_safe_queue<T>::wait_and_pop()
{
    std::unique_lock lk{mut};
    data_cond.wait(lk, [this]{ return !this->data_queue.empty(); }); // Add this or not 
    std::shared_ptr<T> res{std::make_shared<T>(data_queue.front())};
    data_queue.pop();
    return res;
}

template <typename T>
bool thread_safe_queue<T>::empty() const
{
    std::unique_lock lk{mut};
    return data_queue.empty();
}


thread_safe_queue<int32_t> queue; 

void input(int32_t ele) {
    for (int32_t i  = 0; i < 20; i++) {
        queue.push(ele);
    }
}

void output() {
    for (int32_t i = 0; i < 20; ++i) {
        auto ele = queue.wait_and_pop();
        std::cout << i << "th " << *ele << std::endl;
    }
}

int main() {
    std::thread output_thread_1{output};
    std::thread output_thread_2{output};
    std::thread input_thread_1{input, 666};
    std::thread input_thread_2{input, 666};

    input_thread_1.join();
    input_thread_2.join();
    output_thread_1.join();
    output_thread_2.join();
}