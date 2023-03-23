#include <cstdio>
#include <functional>
#include <iostream>

template<typename Func>
class ScopeGuard final {
    public:
        ScopeGuard(Func func): m_Func{func} {}
        ~ScopeGuard() {
            m_Func();
        }
    private:
        Func m_Func;
};

int main() {
    FILE *fp = fopen("filename", "wb");
    ScopeGuard<std::function<void()>> guard{
        [&fp](){
            std::cout << "guard" << std::endl; 
            fclose(fp); 
        }
    };

    //...
}