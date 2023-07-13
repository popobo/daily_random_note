#include <iostream>
#include <thread>
#include <future>

int calculate_result() {
    // 模拟计算结果的过程
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}

int main() {
    // 创建一个 std::promise 对象和一个 std::future 对象
    std::promise<int> my_promise;
    std::future<int> my_future = my_promise.get_future();

    // 在另一个线程中计算结果并将结果传递给 std::promise 对象
    std::thread my_thread([&my_promise]() {
        int result = calculate_result();
        my_promise.set_value(result);
    });

    // 在当前线程中等待计算结果并处理
    std::cout << "Waiting for result..." << std::endl;
    try {
        int result = my_future.get(); // 从 std::future 对象中获取结果
        std::cout << "Result is: " << result << std::endl;
    } catch(const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    // 等待另一个线程结束
    my_thread.join();

    return 0;
}