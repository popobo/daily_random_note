#pragma once

#include <tuple>
#include <functional>
#include <iostream>

template<typename Subset, typename Superset>
struct is_subset_of;

template<typename T, typename... Subset, typename U, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<U, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<T, Subset...>, std::tuple<Superset...>>::value;
};

template<typename T, typename... Subset, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<T, Superset...>> {
    constexpr static bool value = is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value;
};

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

//递归寻找
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
        using index = typename find_all_index<-1, std::tuple<Subset...>, std::tuple<Superset...>>::value;
        std::apply(func, index::make_tuple(target));
    }
}

