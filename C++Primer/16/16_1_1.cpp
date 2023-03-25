#include <vector>
#include <string>
#include <algorithm>
#include <list>

template<typename T_iterator, typename T_value>
inline T_iterator myfind(T_iterator begin, T_iterator end, const T_value& value) {
    
    for (auto iter = begin; iter != end; ++iter) { // iterator of list does not implement <
        if (*iter == value) {
            return iter;
        }
    }

    return end;
}

int main() {

    std::vector<int> vec1{1, 200, 3, 4}, vec2{3, 4, 5, 7};

    auto iter = myfind(vec1.begin(), vec1.end(), 200);
    
    std::list<std::string> list1{"123", "234"};
    auto iter1 = myfind(list1.begin(), list1.end(), "123");

    return 0;
}