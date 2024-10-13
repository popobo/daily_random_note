#include <future>
#include <iostream>

int32_t asynchronous_task() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 666;   
}

void do_other_stuff() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "do other stuff" << std::endl;
}

void compute_result(std::promise<int>&& result_promise) {
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        result_promise.set_value(777);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "after 777" << std::endl;
    } catch(...) {
        result_promise.set_exception(std::current_exception());
    }
}

int main() {
    // std::future<int32_t> answer = std::async(std::launch::deferred, asynchronous_task); // 4s
    // std::future<int32_t> answer = std::async(std::launch::async, asynchronous_task); // 2s
    // do_other_stuff();
    // std::cout << answer.get() << std::endl;

    std::promise<int> result_promise;
    auto result_future = result_promise.get_future();

    std::thread t(compute_result, std::move(result_promise));
    
    std::cout << "The result is " << result_future.get() << std::endl;

    t.join();
    return 0;
}