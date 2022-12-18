#include <cstdio>

void fun(int &x) {
//
}

int main() {
    // fun(10); 报错，因为10是右值，函数的参数是左值引用
    // int &x = 11;
    const int &x = 11; // const的引用能够绑定到右值
    int y = 10;
    fun(y);
    return 0;
}