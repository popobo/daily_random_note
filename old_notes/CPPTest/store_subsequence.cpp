#include <tuple>

//储存元素
template<int ...Args>
struct int_list
{
    using type = int_list<Args...>;
    //根据储存的元素，从给定的tuple中进行裁剪
    template<typename Tuple>
    constexpr static auto maketuple(const Tuple &target)
    {   
        return std::make_tuple(std::get<Args>(target)...);
    }
};

//头插
template<int N, typename list>
struct list_prepend;

template<int N, int ...Args>
struct list_prepend<N, int_list<Args...>>
{
    using result = int_list<N, Args...>;
};

int main()
{
    // using myList = int_list<1, 2, 3>;  // 定义一个列表
    // using newList = list_prepend<0, myList>::result;  // 在列表头部插入整数 0
    // // newList 的类型为 int_list<0, 1, 2, 3>

    // std::tuple<int, int, int, int> myTuple(4, 5, 6, 7);
    // auto resultTuple = newList::maketuple(myTuple);
    // // resultTuple 的类型为 std::tuple<int, int, int, int, int>,
    // // 值为 std::make_tuple(0, 4, 5, 6, 7)

    using myList = int_list<3>;
    using newList = list_prepend<0, myList>::result;

    std::tuple<int, float, int, int> myTuple(4, 3.0, 6, 7);
    auto resultTuple = newList::maketuple(myTuple);
    
    return 0;
}