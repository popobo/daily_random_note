#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include <list>
#include <queue>

class foo {
public:
    foo(){
        std::cout << "foo" << std::endl;
    }

    ~foo(){
        std::cout << "~foo" << std::endl;
    }

    int i = 0;
};

class Base {
public:
    Base() {
        std::cout <<"Base constructor" << std::endl;
    }

    Base (const Base& newBase){
        std::cout << "Base copy constructor" << std::endl;      
    }
};

class Derived :public Base{
public:
    Derived() {
        std::cout <<"Derived constructor" << std::endl;
    }

    Derived(const Derived& newDerived){
        std::cout << "Derived copy constructor" << std::endl;    
    }
};

int main() {
    // std::shared_ptr<int> shptr = std::make_shared<int>(1);
    // std::shared_ptr<int> shptr2 = shptr;
    // if (shptr == shptr2) {
    //     std::cout << "====" << std::endl;
    // }

    // std::weak_ptr<int> weptr = shptr;
    // std::weak_ptr<int> weptr1 = shptr2;
    // if (weptr.lock() == weptr1.lock()) {
    //     std::cout << "=======" << std::endl;
    // }

    // int * i = new int{1};
    // std::shared_ptr<int> ish1;
    // {
    //     std::shared_ptr<int> ish2 = std::shared_ptr<int>(i);
    //     ish1 = ish2;
    //     std::cout << ish1.use_count() << std::endl;
    // }
    // std::cout << ish1.use_count() << std::endl;
    // ish1.reset();
    // std::cout << ish1.use_count() << std::endl;
    // *i = 2;
    // 会导致两次析构
    //foo * f = new foo;
    //std::shared_ptr<foo> ish1 = std::shared_ptr<foo>(new foo());
    //std::shared_ptr<foo> ish2 = std::shared_ptr<foo>(f);


    // std::vector<std::tuple<int, int>> vecTuples[3];
    // for (const auto&ele : vecTuples){
    //     std::cout << "test";
    // }

    // unsigned char *datas[8]{nullptr};
    // std::cout << sizeof(unsigned char *) << std::endl;
    // std::cout << sizeof(datas) << std::endl;

    // std::shared_ptr<foo> test = std::make_shared<foo>();
    // const std::shared_ptr<foo> &test2 = test; 
    // test2->i = 2;
    // std::cout << test->i;

    // test = std::make_shared<foo>();

    // foo* test1 = new foo();
    // test1->i = 3;
    // delete test1;
    // std::queue<std::shared_ptr<foo>> list;
    // {
    //     std::shared_ptr<foo> test = std::make_shared<foo>();
    //     list.push(test);
    //     list.pop();
    // }
    
    // std::cout << __LINE__ << std::endl;
    
    Derived dervive1;
    Base*base = new Derived(dervive1);

}