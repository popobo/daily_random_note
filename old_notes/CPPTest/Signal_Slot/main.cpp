#include "PrarameterCrop.hpp"
#include "EventHandler.hpp"
#include "Event.hpp"
#include "Object.hpp"

struct Key
{
    int id;

    Key(int i)
    {
        id = i;
        std::cout << "构造函数" << std::endl;
    }

};

class Button : public Object
{
    int id;
public:
    Event<int, const char *, std::string, Key> Clicked;

    Button(int i) : id(i)
    {}

    void print(int a)
    {
        std::cout << "对象ID是：" << id << std::endl;
    }

    void SetId(int num)
    {
        id = num;
    }
};

class Label : public Object
{
    int id;
public:
    Label(int v) : id(v)
    {}

    void print(int a)
    {
        std::cout << "我是label的打印函数，参数是：" << a << "  id是: " << id << std::endl;
    }
};

int main() 
{
    Button *button1 = new Button(1);
    Label *label1
     = new Label(1);
    Label *label2 = new Label(2);
    EventLoop * event_loop_ptr = nullptr;
    std::thread th1{[&]() {
        event_loop_ptr = new EventLoop{};
        label1->MoveToThread(std::this_thread::get_id());
        event_loop_ptr->Run();
    }};

    // connect(button1, &Button::Clicked, label1, &Label::print);
    // connect(button1, &Button::Clicked, label2, &Label::print);
    connect(button1, &Button::Clicked, label1, [=](const char *text)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "成功绑定lambda函数啦,这里是1号,打印的信息是：" << text << std::endl;
    });

    // connect(button1, &Button::Clicked, label2, [&](Key key)
    // {
    //     std::cout << "成功绑定lambda函数啦,这里是2号，打印的信息是：" << key.id << std::endl;
    // }, ConnectionType::AutoConnection);
    
    Key key(1);
    button1->Clicked.emit(1, "按钮被点击啦", "这个是std::string哦", key);
    std::cout << "----------------" << std::endl;
    disconnect(button1, &Button::Clicked, label1, &Label::print);
    button1->Clicked.emit(1, "按钮被点击啦", "这个是std::string哦", key);
    delete label1;
    std::cout << "----------------" << std::endl;
    button1->Clicked.emit(1, "按钮被点击啦", "这个是std::string哦", key);
    std::cout << "----------------" << std::endl;
    delete label2;
    button1->Clicked.emit(1, "按钮被点击啦", "这个是std::string哦", key);
    delete button1;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    event_loop_ptr->Quit();
    th1.join();
    delete event_loop_ptr;

    return 0;
}