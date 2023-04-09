#include "PrarameterCrop.hpp"
#include "EventHandler.hpp"
#include "Event.hpp"
#include "Object.hpp"

struct Key
{  
    int value = 4;
};

void print1() {
    std::cout << "function print" << std::endl;
}

class Button: public Object {
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

class Label: public Object
{
    int* m_a = new int{111};

public:
    ~Label() {
        delete m_a;
        m_a = nullptr;
    }

    void print(int a)
    {
        std::cout << "类成员函数，参数是：" << a << " m_a: " <<* m_a << std::endl;
    }
};

int main()
{
    // Button *button1 = new Button();  
    // Label *label = new Label();  
    // label->Subscribe(button1, &Button::Clicked, &Label::print);  
    // button1->Clicked(1, 2);  
    // delete button1;  
    // delete label; 

    Button *button1 = new Button();  
    Label *label = new Label();  
    label->Subscribe(button1, &Button::Clicked, &Label::print);  
    button1->Clicked(1, 2);  
    delete label;  
    button1->Clicked(1, 2);  
    delete button1;  
    
    return 0;
}