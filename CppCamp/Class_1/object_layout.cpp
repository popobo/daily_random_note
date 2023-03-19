#include <iostream>

#define PRINT_FUNC_ADDR std::cout << __func__ << std::endl

class Point {
public:
    int x;
    int y;
    static int a;
};

int main() {
    Point p1{1, 2};
    std::cout << &p1.x << std::endl; // low address
    std::cout << &p1.y << std::endl; // high address
    std::cout << sizeof(p1) << std::endl;

    Point *p2 = new Point{3, 4};
    std::cout << &p2->x << std::endl; // low address
    std::cout << &p2->y << std::endl; // high address  
    std::cout << sizeof(p1) << std::endl;
    delete p2;
    PRINT_FUNC_ADDR;
}