#include "PrarameterCrop.hpp"
#include "EventHandler.hpp"
#include "Event.hpp"

struct Key
{  
    int value = 4;
};

void print1() {
    std::cout << "function print" << std::endl;
}

class Button {
    public:
        void print(int a) {
            std::cout << "member function print a: " << a << std::endl;
        }

    Event<int, int> Clicked;
};

void print2(int x, int y)
{
    std::cout << "鼠标的坐标是x：" << x << "y是：" << y << std::endl;
}

class Label
{

public:  
    void print(int a)
    {  
        std::cout << "类成员函数，参数是：" << a << std::endl;
    }
};

int main()
{
    // std::function<void(int,char,Key)> func = [](int a, char b, Key key)  
    // {  
    //     std::cout << "参数是" << a << " " << b << " " <<  key.value <<  std::endl;  
    // };  
    // std::tuple<int,int,char,int,float,Key> args ={1, 2, 'a', 4, 3.0f, Key()};  
    // TupleTake(func,args);
    // //正确打印结果 1,a,4

    // EventHandler<void, std::tuple<int>, int, char> func1{[](int a) {
    //     std::cout << "lambda function" << std::endl;
    // }};

    // func1(1, 'a');

    // EventHandler<void, std::tuple<>, int, char> func2{print1};
    // func2(1, 'a');
    
    // EventHandler<Button, std::tuple<char>, int, char> func3(new Button, &Button::print);
    // func3(1, 'a');

    // EventHandlerInterface<int, char> *handler = &func1;
    // (*handler)(1, 'a');
    // handler = &func2;
    // (*handler)(1, 'a');
    // handler = &func3;
    // (*handler)(1, 'a');

    Button *button = new Button{};
    Label *label = new Label{};
    
    button->Clicked.Attach(print2);
    button->Clicked.Attach(button, &Button::print);
    button->Clicked.Attach(label, [](int x, int y){
        std::cout << "lambda x: " << x << " y: " << y << std::endl;
    });

    button->Clicked(1, 2);  
    std::cout << "------------------------" << std::endl;  
    button->Clicked.Detach(print2);
    button->Clicked(1, 2);  
    std::cout << "------------------------" << std::endl;  
    button->Clicked.Detach(button, &Button::print);  
    button->Clicked(1, 2);  
    std::cout << "------------------------" << std::endl;  
    button->Clicked.Clear();  
    button->Clicked(1, 2); 

    delete button;
    delete label;

    return 0;
}