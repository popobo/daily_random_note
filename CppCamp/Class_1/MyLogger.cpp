#include <cinttypes>
#include <iostream>
#include <variant>

#define PRINT_FUNC_ADDR std::cout << __func__ << " at " << this << std::endl

class Widget {
    int m_a{1};
    int m_b{2};
    int m_c{3};
    uint8_t m_d{4};

public:
    Widget(int a, int b, int c, uint8_t d);
    Widget(const Widget& other);
    Widget();
    Widget& operator=(const Widget& other);
    // ~Widget();

    void Dump();
    int Sum();
};

Widget::Widget(int a, int b, int c, uint8_t d): m_a{a}, m_b{b}, m_c{c}, m_d{d} {
    PRINT_FUNC_ADDR;
}

//Widget::Widget(const Widget &other): m_a{other.m_a}, m_b{other.m_b}, m_c{other.m_d}, m_d{other.m_d} {}

// 委託構造
Widget::Widget(const Widget &other): Widget(other.m_a, other.m_b, other.m_c, other.m_d) {
    PRINT_FUNC_ADDR;
} 

Widget::Widget() {
    PRINT_FUNC_ADDR;
}

Widget &Widget::operator=(const Widget &other) {
    m_a = other.m_a;
    m_b = other.m_b;
    m_c = other.m_c;
    m_d = other.m_d;
    PRINT_FUNC_ADDR;
    return *this;
}

// Widget::~Widget() {
//     PRINT_FUNC_ADDR;
// }

void Widget::Dump()
{
    auto dumpItem = [this](auto name, auto &item) {
        std::cout << "@" << (void *)&item << ", size is " << sizeof(item) << std::endl;
    };

    dumpItem("m_a", m_a);
    dumpItem("m_b", m_b);
    dumpItem("m_c", m_c);
    dumpItem("m_d", m_d);
    
    std::cout << "Object size: " << sizeof(*this) << std::endl;
}

int Widget::Sum() {
    return m_a + m_b + m_c + m_d;       
}

int CallSum(Widget a) {
    return a.Sum();
}

Widget factory(int a, int b, int c, uint8_t d) {
    return Widget(a, b, c, d);
}

enum struct Tag: uint16_t {
    W_OBJ,
    INTEGER,
};

struct TaggedPoorVariant {
    Tag t;
    union {
        Widget m_w;
        int m_int;
    };
};

static Widget c{};

int main() {
    std::cout << __LINE__ << std::endl;

    Widget a{};
    Widget b{};
    b = a;
    Widget d{a};
    a.Dump();
    b.Dump();
    d.Dump();

    CallSum(a);
    
    Widget e{factory(2, 3, 4, 5)};

    TaggedPoorVariant v1{Tag::W_OBJ, Widget{1, 2, 3, 4}}; // This requires that destructor of Widget must be trivial, or report error: union member ‘TaggedPoorVariant::<unnamed union>::m_w’ with non-trivial ‘Widget::~Widget()’
    v1.t = Tag::INTEGER;
    v1.m_int = 100;
    TaggedPoorVariant v2{.t = Tag::INTEGER, .m_int = 42u};
    std::cout << "v1 has a size of " << sizeof(v1) << std::endl;
    std::cout << "v2 has a size of " << sizeof(v2) << std::endl;
    
    std::variant<int, std::string, Widget> v3;
    std::cout << "v2 has a size of " << sizeof(v3) << std::endl;
    v3 = "hello";
    std::cout << v3.index() << std::endl;
    v3 = 42;
    std::cout << v3.index() << std::endl;
    v3 = Widget{1, 1, 1, 1};
    std::cout << v3.index() << std::endl;

    std::cout << __LINE__ << std::endl;
    return 0;
}