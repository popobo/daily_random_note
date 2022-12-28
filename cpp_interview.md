# 表达式，左值，右值和引用
- 表达式时可求值的，对表达式求值可得到一个结果，结果有两个属性
    - 类型（int, string或自定义类型）
    - 值类别
        - 五个类别分别基于表示的两个特征
            - 具名（identity）：可以确定表达式是否与另一表达式指代同一实体，可以在等号左边，能够取地址
            - 可被移动：移动构造函数、移动复制运算符或实现了移动语义的其他函数重载能否绑定于这个表表达式
        - 五种类型
            - 左值（lvalue，left value：具名且不可被移动
                - 特征：
                    - 可通过取地址运算符获取其地址（可以取地址）
                    - 可修改的左值可用作内建赋值和内建符合赋值运算符的左操作数（可以放在等号左边）
                    - 可以用来初始化左值引用
                - 例子
                    - 变量名、函数名以及数据成员名
                    - 返回左值引用的函数调用
                    - 由赋值运算符或复合赋值运算符连接的表达式，如(a=b, a-=b等)
                    - 解引用表达式 *ptr
                    - 下标运算符的结果([])
                    - 
                        ```c++
                            int a = 1; // a是左值
                            T& f();
                            f();//左值
                            ++a;//左值
                            --a;//左值
                            int b = a;//a和b都是左值
                            struct S* ptr = &obj; // ptr为左值
                            arr[1] = 2; // 左值
                            int *p = &a; // p为左值
                            *p = 10; // *p为左值
                            class MyClass{};
                            MyClass c; // c为左值
                        ```
            - 将亡值（xvalue，expiring value)：具名且可被移动
                - 顾名思义即将消亡的值，是C++11新增的跟右值引用相关的表达式，通常是将要被移动的对象（移为他用）
                - 例子
                    - 返回右值引用T&&的函数的返回值
                    - std::move的返回值
                    - 转换为T&&的类型转换函数的返回值
                - 只能用两种方式获得
                    - 返回右值引用的函数的调用表达式,如 static_cast<T&&>(t); 该表达式得到一个 xvalue
                    - 转换为右值引用的转换函数的调用表达式，如：std::move(t)
                    - 
                        ```c++
                        std::string fun() {
                        std::string str;
                        // ...
                        return str;
                        }

                        std::string s = fun();
                        ```
                        - C++11前，s = fun();会调用拷贝构造函数
                        - C++11后，s = fun();会被优化为move操作，
                        str会被进行隐式右值转换，等价于static_cast<std::string&&>(str)，
                        即触发s相对应的移动构造，或移动赋值运算符，
                        没有移动构造或移动赋值运算符则调用拷贝构造或拷贝赋值运算符，都没有则报错
                        - 然后str都会被销毁，所以是将亡值
                        - 实际上编译器可能会帮你优化成只调用一次构造函数，
                        添加编译选项"-fno-elide-contruction"可以取消这个优化
            - 纯右值（pvalue，pure value）：不具名且可被移动
                - 特征： 只能在等号右边，不能取地址，不具名
                - 例子
                    - 字面值(字符串字面值除外)，例如1，'a', true等
                    - 返回值为非引用的函数调用或操作符重载，
                    例如：str.substr(1, 2), str1 + str2, or it++
                    - 后置自增和自减表达式(a++, a--)
                    - 算术表达式
                    - 逻辑表达式
                    - 比较表达式
                    - 取地址表达式
                    - lambda表达式
                    - 
                        ```c++
                        // 字面值
                        nullptr;
                        true;
                        1;
                        // 返回值为非引用的函数调用
                        int func();
                        func();

                        //后置自增/自减表达式
                        a++;
                        a--;
                        //逻辑表达式
                        a && b;
                        //比较表达式
                        a > b;
                        ```
            - 混合类别
                - 泛左值（glvalue，generalized lvalue）：具名，lvalue和xvalue都属于glvalue
                - 右值（rvalue，right value）：可被移动的表达式，prvalue和xvalue都属于rvalue
    - 引用
        - 特点：
            - 变量别名
            - 声明时必须初始化
            - 通过引用可以修改变量
        - C++11中有三种引用
            - 左值引用
            - 常量左值引用（不希望被修改）
                ```c++
                std::string str = "abc";
                std::string &s = str;
                const int &a = 10;
                int &b = 10; //错，右值不能绑定左值引用
                ```
            - 右值引用
                - 右值引用可以延长右值（主要是将亡值）的生命周期，右值引用的作用就是为了绑定右值
                - 
                    ```c++
                    int a = 1;
                    int &rb = a; // b为左值引用
                    int &&rrb = a; // 错误，a是左值，右值引用不能绑定左值
                    int &&rbb1 = 10; // 正确，10为右值
                    int &rb1 = i * 2; // 错误，i * 2 为右值
                    int &&rb2 = i * 2; // 正确
                    const int &c = 1; // 正确
                    const int &c1 = i * 2; // 正确
                    ```
                - 作用
                    - 实现移动语义
                        - 对象赋值时，避免资源的重新分配
                        - 移动构造函数以及移动赋值函数
                        - 在stl中有着广泛的应用
                        - std::unique_ptr
                        - std::function
                    - 实现完美转发
                        - 函数模板可以将自己的参数完美地转发给内部调用的其他函数
                        - 完美是指不仅能准确转发参数的值，还能保证被转发的参数的左右值属性不变
                        - 
                            ```c++
                                template<typename T>
                                void revoke(T &&t){
                                    // t是左值
                                    func(std::forward<T>(t));
                                }
                                revoke(10); //右值或右值引用，int &&&& -> int &&
                                int i = 10;
                                revoke(i); // 传入左值，int && -> int & ?
                                int &j = i;
                                revoke(j); // 传入左值引用，int &&& -> int &
                            ```
                        - 借用万能引用（T &&t)，通过引用的方式接收左右属性的值
                        - 引用折叠规则
                            - 参数为左值或左值引用，T&&将转化为int&
                            - 参数为右值或右值引用，T&&将转化为int&&
                            - 引用一般不能指向引用，除了这种情况 
                        - std::forward<T>(v)
                            - T为左值引用，v将转化为T类型的左值
                            - T为右值引用，v将转化为T类型的右值

# C++ 前置声明
- 前提：
    在一个既定平台上，各种指针主要是提供一个高层的抽象语义，
    即该指针到底指向哪个对象，让编译器对用户操作（如调用成员函数）进行决议和检查
    但其实在运行期，每种指针都是32/64位的长整型
- 优点
    - 前置声明能够隐藏依赖
        - 节省编译时间
        - 解决循环引用的问题
        A包含一个指向B的指针，B包含A的一个实例，这种情况下，使用前置声明
    - pimple模式
        - Qt中有使用，主要是为了避免UI界面的改动导致大量源文件重新编译
        - pimple模式又被称作“编译期防火墙”，只有class A的实现部分依赖class B 
- 缺点
    - 场景：类的实现者需要修改类名或更换命名空间，
    出于兼容性他原本可以在原命名空间里/用原名通过using来起一个别名指向新类。
    然而别名不能被前向声明。所以需要修改所有相关的头文件。
    - 若把#include换成前置声明，由于声明时不知道D是B的子类，test()中f(x)就会导致f(void*)被调用，而不是f(B*)。
    - 
        ```c++
        // b.h:
        struct B {};
        struct D : B {};

        // good_user.cc:
        #include "b.h"
        void f(B*);
        void f(void*);
        void test(D* x) { f(x); }  // calls f(B*)
        ```
    - C++标准5.3.5/5中规定，delete一个不完整类型的指针时，
    如果这个类型有non-trivial的析构函数（不是编译器默认生成的析构函数），
    那么这种行为是未定义的。把前置声明换成#include则能保证消除这种风险。
- 处理方式
    - 当且仅当出现循环引用时使用前置声明（一般设计时就应该避免循环引用）
    - 由一个专门管理头文件的团队来管理前置声明

# Qt
## Qt元对象系统
- Qt的元对象系统（Meta-Object System）
提供对象之前通信的信号与槽机制、
运行时类型信息、动态属性系统
    - QObject类时所有使用元对象系统的类的基类
    - 在一个类的private部分声明Q_OBJECT宏，使得类可以使用元对象特性，
    如动态属性、信号与槽
    - MOC（元对象编译器）为每个QObject的子类提供必要的代码来实现元对象系统的特性。
    - 除了信号与槽机制外，元对象还提供其他功能
        - Qbjet::metaOject()函数返回类关联的元对象，可以访问元对象的接口。
        - 
            ```c++
            QObject *obj = new QPushButton;
            obj->metaObject ()->classNane();  //返回"QPushButton
            ```
        - Q0bjct:inherits(const char *className)函数判断一个对象实例是否是名称为 className 的类或 QObject 的子类的实例。
        - 
            ```c++
            QTimer *timer = new QTimer;  // OTimer是oobject的子类
            timer->inherits ("QTimer");  //返回true
            timer->inherits ("QObject");  //返回true
            timer->inherits ("QAbstractButton");//返回false. 不是QAbatractButton的子类
            ```
        - QObject::tr() 和 Qbjet::trUtf8() 函数可翻译字符串，用于多语言界面设计。
        - QObjct:setProperty() 和 Q0bjct:property() 函数用于通过属性名称动态设置和获取属性值。
        - 对于 QObject 及其子类，使用 qobject_cast() 函数进行动态投射(dynamic cast)。
        - 
            ```c++
            // QMyWidget 是 QWidget 的子类并且在类定义中声明了Q_OBJECT宏。
            Q0bject *obj = new QMyWidget;
            Qwidget *widget = qobject_cast<Qwidget *>(obj); // 成功
            QMyWidget *myWidget = qobject_cast<QMyWidget *>(obj); // 成功，qoiect_cast() 并不区分 Qt 内建的类型和用户自定义类型
            QLabe1 *labol = qobject_caot<QLabe1 *>(obj); // 失败
            ```
## 属性系统
- 属性定义：Qt提供一个Q_PROPERTY宏可以定义属性，也是基于元对象系统实现的。
    - 在QObject的子类中，可以用宏Q_PROPERTY定义属性
    - 
        ```c++
        Q_PROPERTY(type name
             (READ getFunction [WRITE setFunction] |
              MEMBER memberName [(READ getFunction | WRITE setFunction)])
             [RESET resetFunction]
             [NOTIFY notifySignal]
             [REVISION int]
             [DESIGNABLE bool]
             [SCRIPTABLE bool]
             [STORED bool]
             [USER bool]
             [CONSTANT]
             [FINAL])
        ```
        - READ 指定一个读取属性值的函数，没有 MEMBER 关键字时必须设置READ。
        - WRITE指定一个设定属性值的函数， 只读属性没有WRITE设置。
        - MEMBER指定一个成员变量与属性关联，成为可读可写的属性，无需再设置READ和WRITE。
        - RESET是可选的，用于指定一个设置属性缺省值的函数。
        - NOTIFY是可选的，用于设置一个信号， 当属性值变化时发射此信号。
        - CONSTANT表示属性值是常数，对于一个对象实例，READ指定的函数返回值是常数，
        但每个实例的返回值可以不一样。具有CONSTANT关键字的属性不能有WRITE和NOTIFY关键字。
        - FINAL表示所定义的属性不能被子类重载。
        - QWidget定义属性的例子：
            - 
                ```c++
                Q_PROPERTY (bool  focus READ hasFocus)
                Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
                Q_PROPERTY (QCursor cursor READ cursor WRITE setCursor RESET unsetCursor)
                ```
    - 属性的使用
        - 不管是否用READ和WRITE定义了接口函数。
        只要知道属性名称就可以通过QObjct:property()读取属性值，并通过QObject:setProperty0设置属性值
        ```c++
        QPushButton *button = new QPushButton;
        QObject *object = button;
        object->setProperty("flat", true);
        bool isFlat- object->property("flat")
        ```
- 动态属性
    - QObject:setPropert()函数可以在运行时为类定义一个新的属性，称之为动态属性。
    动态属性是针对类的实例定义的。
    动态属性可以使用Qbjct:property()查询，就如在类定义里用 Q_PROPERTY 宏定义的属性一样。
    - 例如，在数据表编辑界面上，一些字段是必填字段，
    就可以在初始化界面时为这些字段的关联显示组件定义一个新的required属性，
    并设置值为"true"，如:
    - 
        ```c++
        editName->setProperty("required"，"true");
        comboSex-> setProperty("required". "true"); 
        checkAgree-> setProperty("required", "true");
        ```
    - 然后，可以应用下面的样式定义将这种必填字段的背景颜色设置为亮绿色
    - 
        ```
        *[required="true"] (background-color: lime)
        ```
- 类的附加信息
    - 属性系统还有一个宏Q_CLASSINFO可以为类的元对象定义“名称-值”信息
    - 
        ```c++
        class QMyC1ass : public QObject
        { 
        Q_OBJECT
        Q_CLASSINFO("author", "Wang" )
        Q_CLASSINFO("company", "UPC" )
        Q_CLASSINFO("version "，"3.0.1")
        public:
        ...
        }
        ```
    -  通过QMetaClassInfo QMetaObject::classInfo(int index) const获得类附加信息名称和值。


## C++标准库
- 第二章 C++及标准库简介
    - 2.1 C++ Standard的历史
        - C++标准
            - 1.C++98，ISO/IEC 14882:1998
            - 2.C++03，ISO/IEC 14882:2003，和C++98一样都被视作“第一份C++标准规格”
            - 3.TR1，ISO/IEC 14882:2007
            - 4.C++11，ISO/IEC 14882:2011，是第二份C++标准，C++0x即是C++11
        - 2.1.1 C++11 Standard常见疑问
            - String class设计比较完善和安全，接口完备，并有异常抛出机制
            - STL 不是非常变量，也不对很多逻辑错误进行检查，需要使用者清楚其概念并妥善使用
        - 2.1.2 C++98和C++11的兼容性
            - 可以通过定义宏__cplusplus的值来指定当前文件以什么标准进行编译
            - 向后兼容仅保证源码兼容，不保证二进制兼容
    - 2.2 复杂度与Big-O标记
- 第三章
    - 3.1.1 微小但重要的语法提升
        - Template表达式内的空格
        - 
            ```
            vector<list<int> >; // C++11无需空格
            ```
        - nullptr和std::nullptr_t
            - nullptr代替原来的0和NULL
            - nullptr是一个新的基础类型std::nullptr_t
    - 3.1.2 以auto完成类型自动推导
        - test
    - 3.1.3 一致性初始化（Uniform Initialization）和初值列（Initializer List）
        - code
        - 
            ```c++
            int i; // has undefined value
            int j{}; // j is initialized by 0
            int *p{}; // p is initialized bny nullptr
            ```
        - 窄化（narrowing）也就是精度降低或造成数值变动，对大括号是不成立的
            - code
            - 
                ```c++
                    int x1(5.3); // OK
                    int x2 = 5.3; // OK
                    int x3{0.0}; // ERROR
                    vector<int> v1{1, 2.0}; // ERROR
                ```
        - 当“指明参数个数”和“指明一个初值列”的构造函数同时存在时，
        会优先使用带初值列的构造函数
        - 注意explict与带初值列的构造函数的关系

    - 3.1.4 Range-Based for循环
        - 类foreach循环
        - 为了避免调用每个元素的拷贝构造和析构函数，一般采用for(const auto&)的形式
        等同于 for (auto _pos = coll.begin(), _end=coll.end(); _pos!=_end; ++_pos) {}
        - 对大小已知的C-style array也可以使用Range-Based for循环
    - 3.1.5 Move语义与Rvalue Reference
        - 右值引用
        - 移动构造函数
        - C++标准库的class保证，在一次move后，对象处于有效但不确定状态
        可以在move后，对它赋予新值，但当前值是不确定的
        - SLT容器保证了，被搬移内容者，搬移后其值为空
        - Rvalue 和 lvalue Reference的重载规则
            左值引用只能接受左值
            常量左值引用能接受左值和右值
            当（常量）左值引用和右值引用重载同时存在时，
            左值找左值引用，右值找右值引用
        - 返回Rvalue Reference
            - 你不需要也不应该返回右值引用，即不该move()返回值
            - 编译器有自身的优化规则
                - RVO，返回值优化
                - 否则move
                - 否则copy
                - 否则报错
    - 3.1.6 新式的字符串字面常量
        - Raw String Literal R"()"
            - 完整形式R"delim(...)delim"，delim是个自定义字符序列
        - 编码的（Encoded）String Literal
            - u8, UTF-8 string literal, const char
            - u, string literal, char16_t
            - U, string literal, char32_t
            - L, wide string literal, wchar_t