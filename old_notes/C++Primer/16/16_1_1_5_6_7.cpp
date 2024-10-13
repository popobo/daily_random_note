#include <cinttypes>
#include <iostream>
#include <iterator>

template<typename T, size_t size>
const T* my_begin(const T (&begin)[size]) {
    return begin;
}

template<typename T, size_t size>
const T* my_end(const T (&begin)[size]) {
    return begin + size;
}

template<typename T>
void print(const T* begin, const T* end) {
    for (auto itr = begin; itr != end; ++itr) {
        std::cout << *itr << std::endl;
    }
}

template<typename T, size_t size>
size_t myconstexpr(const T (&begin)[size]) {
    return size;
}

int main() {
    int j[] = {1, 2, 3, 1};
    
    print(std::begin(j), std::end(j));
    print(my_begin(j), my_end(j));

    int *x = j;
    int *&y = x;
    int (&z)[4] = j;
    std::cout << *y << std::endl;
    std::cout << *z << std::endl;

    std::cout << myconstexpr(j) << std::endl;

    return 0;
}