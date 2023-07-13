#include <iostream>
#include <memory>

class Test
{
public:
    Test()
    {
        std::cout << "Test" << std::endl;
    }
    
    ~Test()
    {
        std::cout << "~Test" << std::endl;
    }
};

int main()
{
    std::unique_ptr<Test[]> array(new Test[2]);

    std::unique_ptr<Test[], void(*)(Test*)> array2(new Test[2], [](Test* p){
        delete[] p;
    });

    std::unique_ptr<Test, void(*)(Test*)> array3(new Test[2], [](Test* p){
        delete[] p;
    });

    // std::unique_ptr<Test> array4(new Test[2]); wrong
}