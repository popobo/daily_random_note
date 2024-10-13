/*
is_subset_of 模板类用于检查一个元组是否为另一个元组的子集。它通过递归地比较两个元组的类型来实现这一点。

int_list 模板类表示一个整数列表。它包含一个静态成员函数 make_tuple，该函数接收一个元组并使用列表中的整数作为索引从目标元组中获取元素，然后创建一个新的元组。

list_prepend 模板类用于将一个整数添加到 int_list 的开头。

find_next_index 模板类递归地查找一个类型在另一个元组中的索引。当找到目标类型时，递归停止，返回找到的索引。

find_all_index 模板类递归地查找一个元组中所有类型在另一个元组中的索引，并将这些索引存储在一个 int_list 中。

TupleTake 是一个模板函数，它接收一个函数和一个元组。它首先检查所提供的函数的参数类型是否为目标元组的子集。如果满足条件，它会找到这些类型在目标元组中的索引，并使用这些索引从目标元组中提取相应的值，然后将这些值应用到所提供的函数上。
*/

#pragma once

#include <tuple>
#include <functional>
#include <iostream>

//参数裁剪相关
template<typename Subset, typename Superset>
struct is_subset_of;

//如果子集的元素和父集的元素不一样，那就递归找父集合的下一个元素
template<typename T, typename... Subset, typename U, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<U, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<T, Subset...>, std::tuple<Superset...>>::value;
};

//如果子集元素和父集元素的一样，那子集就递归寻找下一个元素
template<typename T, typename... Subset, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<T, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value;
};

//递归终止条件，如果子集都找完了，父集还没没空，说明满足子序列关系
template<typename... Superset>
struct is_subset_of<std::tuple<>, std::tuple<Superset...>> {
    constexpr static bool value = true;
};

template<typename... Subset>
struct is_subset_of<std::tuple<Subset...>, std::tuple<>> {
    constexpr static bool value = false;
};

template<>
struct is_subset_of<std::tuple<>, std::tuple<>> {
    constexpr static bool value = true;
};

template<int... Args>
struct int_list {
    using type = int_list<Args...>;
    
    template<typename Tuple>
    constexpr static auto make_tuple(const Tuple& target) {
        return std::make_tuple(std::get<Args>(target)...);
    }
};

template<int N, typename list>
struct list_prepend;

template<int N, int... Args>
struct list_prepend<N, int_list<Args...>> {
    using result = int_list<N, Args...>;
};

template<int N, typename Target, typename Tuple>
struct find_next_index;

template<int N, typename Target, typename Removed, typename... Args>
struct find_next_index<N, Target, std::tuple<Removed, Args...>> {
    constexpr static int value = find_next_index<N - 1, Target, std::tuple<Args...>>::value + 1;
};

template<typename Target, typename Removed, typename... Args>
struct find_next_index<-1, Target, std::tuple<Removed, Args...>> {
    constexpr static int value = find_next_index<-1, Target, std::tuple<Args...>>::value + 1;
};

template<typename Target, typename... Args>
struct find_next_index<-1, Target, std::tuple<Target, Args...>> {
    constexpr static int value = 0;
};

template<int N, typename Subset, typename Superset>
struct find_all_index;

// 递归寻找
template<int N, typename T, typename... Subset, typename... Superset>
struct find_all_index<N, std::tuple<T, Subset...>, std::tuple<Superset...>> {
    using value = typename list_prepend<
    
    find_next_index<N, T, std::tuple<Superset...>>::value,
    
    typename find_all_index<
        find_next_index<N, T, std::tuple<Superset...>>::value, 
        std::tuple<Subset...>, 
        std::tuple<Superset...>
        >::value
    
    >::result;
};

template<int N, typename... Superset>
struct find_all_index<N, std::tuple<>, std::tuple<Superset...>> {
    using value = int_list<>;
};

template<typename... Subset, typename... Superset>
constexpr auto TupleTake(const std::function<void(Subset...)>& func, const std::tuple<Superset...>& target) {
    static_assert(is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value, 
    "slot function parameters and signal parameters do not match");
    if constexpr (is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value) {
        using index = typename find_all_index
        <       
        -1, std::tuple<Subset...>, 
        std::tuple<Superset...>
        >::value;
        
        std::apply(func, index::make_tuple(target));
    }
}

/*
int main() {
    std::tuple<int, double, int> my_tuple(1, 2.0, 3);
    std::function<void(int, int)> my_function = print_sum;

    TupleTake(my_function, my_tuple);

    return 0;
}

For TupleTake template function
Subset is {int, int}, Superset is {int, double, int}

first is_subset_of is 
template<typename T, typename... Subset, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<T, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value;
};
T is int, Subset is {int}, Superset is {double, int}

second is_subset_of is 
template<typename T, typename... Subset, typename U, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<U, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<T, Subset...>, std::tuple<Superset...>>::value;
};
T is int, Subset is {}, U is double, Superset is {int}

third is_subset_of is 
template<typename T, typename... Subset, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<T, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value;
};
T is int, Subset is {}, Superset is {}

forth is_subset_of is
template<>
struct is_subset_of<std::tuple<>, std::tuple<>> {
    constexpr static bool value = true;
};

Finally the first is_subset_of<>::value is true

{
First find_all_index is
template<int N, typename T, typename... Subset, typename... Superset>
struct find_all_index<N, std::tuple<T, Subset...>, std::tuple<Superset...>> {
    using value = typename list_prepend<
    
    find_next_index<N, T, std::tuple<Superset...>>::value,
    
    typename find_all_index<
        find_next_index<N, T, std::tuple<Superset...>>::value, 
        std::tuple<Subset...>, 
        std::tuple<Superset...>
        >::value
    
    >::result;
};
N = -1，T int, Subset is {int}, Superset is {int, double, int}

    first find_next_index is
    template<typename Target, typename... Args>
    struct find_next_index<-1, Target, std::tuple<Target, Args...>> {
        constexpr static int value = 0;
    };
    Target is int, Args is {double, int}
    so first_next_index<>::value is 0
}

{
Second find_all_index is
template<int N, typename T, typename... Subset, typename... Superset>
struct find_all_index<N, std::tuple<T, Subset...>, std::tuple<Superset...>> {
    using value = typename list_prepend<
    
    find_next_index<N, T, std::tuple<Superset...>>::value,
    
    typename find_all_index<
        find_next_index<N, T, std::tuple<Superset...>>::value, 
        std::tuple<Subset...>, 
        std::tuple<Superset...>
        >::value
    
    >::result;
};
N is 0, T is int, Subset is {int}, Superset{int, double int}

    First find_next_index is
    template<int N, typename Target, typename Removed, typename... Args>
    struct find_next_index<N, Target, std::tuple<Removed, Args...>> {
        constexpr static int value = find_next_index<N - 1, Target, std::tuple<Args...>>::value + 1;
    };
    N is 0, Target is int, Removed is int, Args is {double, int}

    Second find_next_index is 
    template<typename Target, typename Removed, typename... Args>
    struct find_next_index<-1, Target, std::tuple<Removed, Args...>> {
        constexpr static int value = find_next_index<-1, Target, std::tuple<Args...>>::value + 1;
    };
    Target is int, Removed is double Args is {int}

    Third find_next_Index is 
    template<typename Target, typename... Args>
        struct find_next_index<-1, Target, std::tuple<Target, Args...>> {
    constexpr static int value = 0;
    };
    Target is int, Args is {}
    so first find_next_index<>::value = 2
}

...

Finally index 
template<int... Args>
struct int_list {
    using type = int_list<Args...>;
    
    template<typename Tuple>
    constexpr static auto make_tuple(const Tuple& target) {
        return std::make_tuple(std::get<Args>(target)...);
    }
};
Args is {0, 2}

std::make_tuple(std::get<0, 2>(std::tuple(int, double, int));

*/