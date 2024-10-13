#include <cstdio>
#include <memory>
#include <iostream>

void fun(int &x) {
//
}

void fun1(const int &x){
    printf("1 x:%d\n", x);
}

void fun1(int &x){
    printf("2 x:%d\n", x);
}

void fun1(int &&x){
    printf("3 x:%d\n", x);
}


class foo {
public:
    foo() {
       
    }

    int i = 0;
};

void fun2(const foo &f) {
    printf("clr foo i: %d\n", f.i);
}

void fun2(foo &f) {
    printf("lr foo i: %d\n", f.i);
}

void fun2(foo &&f){
    printf("rr foo i: %d\n", f.i);
}

int main() {
    // fun(10); 报错，因为10是右值，函数的参数是左值引用
    // int &x = 11;
    const int &x = 11; // const的引用能够绑定到右值
    int y = 10;
    fun(y);
    
    // 字面值会找右值引用
    fun1(100);
    fun1(y);
    fun1(std::move(y));

    fun2(foo());
    
    const char* str = u8R"bo2022(R"()")bo2022";
    printf("str: %s\n", str);

    
    const char16_t* str1 = uR"bo2022(R....)bo2022";
    // 为什么只有R打印出来
    printf("str1: %s\n", str1);

    std::cout << "str1:" << str1 << std::endl;
    return 0;
}