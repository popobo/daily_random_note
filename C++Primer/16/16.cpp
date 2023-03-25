#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <list>

struct sales_data
{
    std::string book_noP{""};
    unsigned units_sold{0};
    double revenue{0.0};
};


template <typename T>
int compare(const T& v1, const T& v2) {
    if (v1 < v2) return -1;
    if (v2 < v1) return 1; //只用<
    return 0;
}

template <unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M]) {
    return strcmp(p1, p2);
}

int main() {
    int ret = compare(1, 2);

    std::vector<int> vec1{1, 200, 3, 4}, vec2{3, 4, 5, 7};

    ret = compare(vec1, vec2);

    ret = compare("hi", "mom");

    // sales_data data1, data2;
    // ret = compare(data1, data2);
    
    return 0;
}