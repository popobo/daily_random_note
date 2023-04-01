#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <iostream>
#include <thread>

struct empty_stack: std::exception {
    const char* what() const throw();
};

template<typename T>
class thread_safe_stack {
    private:
        std::stack<T> data;
        mutable std::mutex m;

    public:
        thread_safe_stack();
        thread_safe_stack(const thread_safe_stack&);
        thread_safe_stack& operator=(const thread_safe_stack&) = delete;
        void push(T new_value);
        std::shared_ptr<T> pop();
        void pop(T& value);
        bool empty() const;
};

template <typename T>
thread_safe_stack<T>::thread_safe_stack() {}

template <typename T>
thread_safe_stack<T>::thread_safe_stack(const thread_safe_stack & other)
{
    std::lock_guard lock{other.m};
    data = other.m;
}

template <typename T>
void thread_safe_stack<T>::push(T new_value)
{
    std::lock_guard lock{m};
    data.push(std::move(new_value));
}

template <typename T>
std::shared_ptr<T> thread_safe_stack<T>::pop()
{
    std::lock_guard lock{m};
    if (data.empty()) throw empty_stack();
    std::shared_ptr<T> const res{std::make_shared<T>(data.top())};
    data.pop();
    return res;    
}

template <typename T>
void thread_safe_stack<T>::pop(T &value)
{
    std::lock_guard lock(m);
    if(data.empty()) throw empty_stack();
    value = data.top();
    data.pop();
}

template <typename T>
bool thread_safe_stack<T>::empty() const
{
    std::lock_guard lock{m};
    return data.empty();
}

const char *empty_stack::what() const throw()
{
    return "empty_stack";
}

thread_safe_stack<int32_t> m_thread_safe_stack; 

void input(int32_t ele) {
    for (int32_t i  = 0; i < 10; i++) {
        m_thread_safe_stack.push(ele);
    }
}

void output() {
    for (int32_t i = 0; i < 20; ++i) {
        try {
            auto ele = m_thread_safe_stack.pop();
            std::cout << *ele << std::endl;
        }
        catch(const empty_stack& stack) {
            std::cout << stack.what() << std::endl;
            --i;
        }
    }
}

int main() {
    std::thread output_thread_1{output};
    std::thread input_thread_1{input, 666};
    std::thread input_thread_2{input, 666};

    input_thread_1.join();
    input_thread_2.join();
    output_thread_1.join();

    return 0;
}