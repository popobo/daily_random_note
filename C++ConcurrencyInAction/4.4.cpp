#include <list>
#include <algorithm>
#include <iostream>
#include <algorithm>
#include <future>
#include <cstdlib>
#include <ctime>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }

    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    const T& pivot = *result.begin();
    
    auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t){
        return t < pivot;
    });

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    auto new_lower{sequential_quick_sort(std::move(lower_part))};
    auto new_higher{sequential_quick_sort(std::move(input))};
    
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);
    return result;
};

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }

    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    const T& pivot = *result.begin();
    
    auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t){
        return t < pivot;
    });

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    std::future<std::list<T>> new_lower_future(std::async(&parallel_quick_sort<T>, std::move(lower_part)));
    auto new_higher{parallel_quick_sort(std::move(input))};
    
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower_future.get());
    return result;
};

std::list<int32_t> generate_random_list(int32_t size, int32_t min_value, int32_t max_value) {
    std::list<int32_t> result;
    srand(time(NULL));

    for(int32_t i = 0; i < size; ++i) {
        result.push_back(min_value + rand() % (max_value - min_value));
    }

    return result;
}

std::ostream& operator<<(std::ostream& ostr, const std::list<int32_t>& list) {
    for (auto &i: list) {
        ostr << ' ' << i;
    }
    return ostr;
}

int32_t main() {
    std::list<int32_t> input{generate_random_list(1000, 0, 100)};

    auto result1 = sequential_quick_sort(input);
    std::cout << result1 << std::endl;

    auto result2 = parallel_quick_sort(input);
    std::cout << result2 << std::endl;
    // std::cout << input << std::endl;
    

    return 0;
}