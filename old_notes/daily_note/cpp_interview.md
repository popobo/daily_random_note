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
## 信号与槽
- QMetaObject::connectSlotByName()
    - 借助QMetaObject::connectSlotByName()的作用，按以下命名规则命名槽函数，
    “void on_<object name>_<signal name>(<signal parameters>);”，
    则connectSlotByName会自动进行信号与槽的连接
    - 若不希望由connectSlotByName进行自动连接，则需要注意槽函数的名字。
    否则轻则警告，重则导致重复连接
- connect的三种写法
    - 元方法式，编译时不做错误检查
        connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(pbutton_clicked()));
    - 函数指针式，省去字符查找，效率更高，并且编译时会对参数类型、个数进行检查，
    涉及到重载的函数，则比较麻烦
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::pbutton_clicked);
    - 
        ```c++
            QLCDNumber::display(int)
            QLCDNumber::display(double)
            QLCDNumber::display(QString)

            auto slider = new QSlider(this);
            auto lcd = new QLCDNumber(this);

            // String-based syntax
            connect(slider, SIGNAL(valueChanged(int)),
                    lcd, SLOT(display(int)));

            // Functor-based syntax, first alternative
            connect(slider, &QSlider::valueChanged,
                    lcd, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

            // Functor-based syntax, second alternative
            void (QLCDNumber::*mySlot)(int) = &QLCDNumber::display;
            connect(slider, &QSlider::valueChanged,
                    lcd, mySlot);

            // Functor-based syntax, third alternative
            connect(slider, &QSlider::valueChanged,
                    lcd, QOverload<int>::of(&QLCDNumber::display));

            // Functor-based syntax, fourth alternative (requires C++14)
            connect(slider, &QSlider::valueChanged,
                    lcd, qOverload<int>(&QLCDNumber::display));
        ```
    - functor式
    连接信号到任意Lambda、std::bind上
    - 
        ```c++
            QByteArray page = ...;
            QTcpSocket *socket = new QTcpSocket;
            socket->connectToHost("qt-project.org", 80);
            QObject::connect(socket, &QTcpSocket::connected, this, [=] () {
                    socket->write("GET " + page + "\r\n");
            }, Qt::AutoConnection);
        ```
    - 注意事项：
        - 槽函数参数个数可以小于信号参数个数，槽函数参数必须在信号中找到对应，多余的参数将被忽略。
        connect(sender, SIGNAL(mySignal(int, const QString &)),receiver, SLOT(mySlot(int)));
        - 信号可以连接信号
        - 如果一个信号被多个槽连接，则在发出信号时，将按照连接的顺序执行槽函数
        - disconnect 断开连接。
    - 连接类型，Qt::ConnectionType
        - Qt::AutoConnection
        默认连接类型，如果信号接收方与发送方在同一个线程，则使用Qt::DirectConnection，
        否则使用Qt::QueuedConnection，连接类型在信号发射时决定
        - Qt::DirectConnection
        信号所连接的槽函数将会被立即执行，并且是在发送信号的线程，
        倘若槽函数执行的是耗时操作，信号由UI线程发射，则会阻塞Qt的事件循环，UI会进入无响应状态
        - Qt::QueuedConnection
        槽函数将会在接收者的线程被执行，
        此中连接类型下的信号倘若被多次触发，相应的槽函数会在接收者的线程里被顺序执行相应次数，
        当使用Qt::QueuedConnection时，参数类型必须是Qt基本类型，或者使用qRegisterMetaType()注册了的自定义类型
        - Qt::BlockingQueuedConnection
        和Qt::QueuedConnnection类似，区别在于发送信号的线程在槽函数执行完毕之前一直处于阻塞状态，收发双方必须不在同一线程，否则会导致死锁
        - Qt::UniqueConnection
        执行方式与AutoConnection相同，不过关联时唯一的。
        如果相同两个对象，相同的信号关联到相同的槽，那么第二次 connect 将失败。
    - 信号-槽的元调用
        - moc（元对象编译器）为信号槽生成了一份元信息
        - QMetaObject::invokeMethod(&mSwitch, "change");
        - “运行期反射”的情况下（只知道有这个对象和函数名字）需要用到
    - 元类型
        - QObject的子类本身附带元信息，可以直接调用信号槽
        - Qt已经把基础数据类型注册进元系统，可以在QMetaType中查看
        - 对于自定义的结构体，类等，则需要注册元类型
        Q_DECLARE_METATYPE(MyStruct)
        Q_DECLARE_METATYPE(MyNamespace::MyStruct)
        - 若采用Qt::QueuedConnection的connect，还需要使用QRegisterMetaType()注册类型
        #include <QMetaType>
        Q_DECLARE_METATYPE(MyStruct);
        qRegisterMetaType<MyStruct>("MyStruct");
    - 返回值
        - 成功则返回连接的句柄QMetaObject::Connection，失败则返回无效的句柄，可将句柄转换为bool进行判断
    - 元对象编译器moc
        - 编译流程如下
        - 程序
            - moc预处理
                - moc_xxx.cpp
                    - Q_OBJECT展开生成函数声明以及moc生成实现
                    - 普通编译
        - Q_OBJECT宏展开
            - QMetaObject 是元对象，元对象系统是Qt另外一套核心机制。
            这个类存储了父类的源对象、我们当前类描述、函数描述和 qt_static_metacall 函数地址即可。
            - QMetaObject staticMetaObject
            构造一个 QMetaObject 对象，传入当前moc文件的动态信息。
            - QMetaObject metaObject()
            返回当前 QMetaObject，一般而言，虚函数 metaObject() 仅返回类的 staticMetaObject 对象。
            - qt_metacall(QMetaObject::Call, int, void)
            - qt_static_metacall(QObject * _o, QMetaObject::Call _c, int _id, void **_a)
            _o:调用者，_c:QMetaObject::InvokeMetaMethod或者QMetaObject::IndexOfMethod，_id函数索引
            根据函数索引进行槽函数的调用，有趣的是自行定义的信号也在其索引中，此为信号信号连接合法性佐证二。
        - 用户定义信号
            moc会帮助用户实现自定义信号
    - 对象之间的通信机制
        - 直接调用
        - 回调函数+映射表
        - 观察者模式
        - connect过程源码分析
        - connectSlotsByName()
        - 
            ```c++
            void QMetaObject::connectSlotsByName(QObject *o)
            {
                //... 
                // 遍历对象中的每个槽方法
                for (int i = 0; i < mo->methodCount(); ++i) {
                    //...
                    //检查列表中的每个对象
                    for(int j = 0; j < list.count(); ++j) {
                        // "on_<objectName>_<signal>"规则匹配
                        //...
                        //匹配到了相应的槽
                        if (Connection(QMetaObjectPrivate::connect(co, sigIndex, smeta, o, i))) {
                            foundIt = true;
                            break;
                        }
                    //...
                    }
                }
            }
            ```
        - connect()
            - connect(this,SIGNAL(user_signal()),this,SLOT(user_slot()));
                - 1 第一阶段
                    - 检查是否为信号
                    // 也就是说Qt是通过 char *signal 中的字符判断来确实它是不是信号的，1xxx 表示槽，2xxx 表示信号。
                    check_signal_macro(sender, signal, "connect", "bind")
                    ++signal; //skip code 将前面的标志去掉
                    - //获取信号名
                    QByteArray signalName = QMetaObjectPrivate::decodeMethodSignature(signal, signalTypes);
                    - //获取函数index
                    int signal_index = QMetaObjectPrivate::indexOfSignalRelative(&smeta, signalName, signalTypes.size(), signalTypes.constData());
                    - //获取membcode（QSLOT_CODE，QSIGNAL_CODE）
                    int membcode = extract_code(method);
                    - //获取槽函数名
                    QByteArray methodName = QMetaObjectPrivate::decodeMethodSignature(method, methodTypes);
                    - //槽函数运行是信号||槽（信号可以作为槽函数的证据之一）
                    switch (membcode)
                - 2 QMetaObject::Connection QMetaObjectPrivate::connect(...)
                    - /* moc文件中的qt_static_metacall函数的地址被传递给了staticMetaObject对象的static_metacall域。
                    这里又将static_metacall域的内容传递给callFunction。所以callFunction指向了moc文件中的qt_static_metacall函数。*/
                    QObjectPrivate::StaticMetaCallFunction callFunction = rmeta ? rmeta->d.static_metacall : nullptr;
                    - //创建connection对象，监听者模式
                    Connection的成员，sender，signal_index， receiver.storeRelaxed，receiverThreadData.storeRelaxed，method_relative，method_offset，connectionType，isSlotObject，argumentTypes.storeRelaxed，callFunction
                - 3 添加监听 void QObjectPrivate::addConnection(int signal, Connection *c)
                    - // 取得链接列表list，并把新的connection c存储进去
                    - // 操作元对象中的Connections
        - 槽函数调用过程
            - 源码分析
                - emit 是一个空宏
                #define emit
                - QMetaObject::activate
                //若信号非子类实现，则可能是其祖先类信号，遍历找到血缘关系最近且具有该信号的类
                while (mo->methodOffset() > signal_index)
                    mo = mo->superClass();
                - template <bool callbacks_enabled>
                void doActivate(QObject *sender, int signal_index, void **argv)
                    - 连接记录列表，获取ConnectionList
                    - 获取当前线程id
                    - 判断是否为发射者线程id
                    - 遍历链接
                    - 遍历槽
                    - 判断是否处于同一线程
                        - 队列连接或自动连接且不在同一线程则放入发射队列
                        - 阻塞连接
                        - 其他：直接调用槽函数或回调函数
            - 特点
                - 同一个线程内的信号-槽，就相当于函数调用，与观察者模式类似
                - 跨线程的信号-槽，在信号触发时，发送者线程将槽函数的调用转化成一次“调用事件”，放入事件循环中。
                接收者线程执行到下一次事件处理时，处理“调用事件”，调用相应的函数。
                
## QThread多线程
- QThread使用方法
    - QThread Class文档，https://doc.qt.io/qt-6.2/qthread.html#details
        - 使用方法
            - 组合QThread的方式，推荐做法
            - 
                ```c++
                /*--------------------DisconnectMonitor-------------------------*/
                class DisconnectMonitor : public QObject
                {
                    Q_OBJECT

                public:
                    explicit DisconnectMonitor();

                signals:
                    void StartMonitor(long long hanlde);
                    void StopMonitor();
                    // if Controller disconnect emit this signal
                    void Disconnect();

                private slots:
                    void slot_StartMonitor(long long hanlde);
                    void slot_StopMonitor();
                    // State machine
                    void Monitor();

                private:
                    long long ControllerHanlde;
                    QTimer *MonitorTimer;
                };

                DisconnectMonitor::DisconnectMonitor()
                {
                    // New a Timer monitor controller by timing
                    MonitorTimer = new QTimer;

                    ControllerHanlde = 0;

                    connect(MonitorTimer,&QTimer::timeout,this,&DisconnectMonitor::Monitor);
                    connect(this,&DisconnectMonitor::StartMonitor,this,&DisconnectMonitor::slot_StartMonitor);
                    connect(this,&DisconnectMonitor::StopMonitor,this,&DisconnectMonitor::slot_StopMonitor);

                    MonitorTimer->start(TAKETIME);
                }

                void DisconnectMonitor::Monitor(){

                    // if not Controller -> return
                    if(0 == ControllerHanlde){
                        return;
                    }
                    //else Listening
                    else{
                        int state = IsConnect(ControllerHanlde);
                        if (0 != state){
                            emit Disconnect();
                        }
                    }
                }

                /*---------------------------Controller----------------------------*/
                class Controller : public QObject
                {
                    Q_OBJECT
                    QThread workerThread;
                public:
                    Controller() {
                        DisconnectMonitor *monitor = new DisconnectMonitor;
                        monitor->moveToThread(&workerThread);
                        connect(workerThread, &QThread::finished, monitor, &QObject::deleteLater);
                        connect(monitor,SIGNAL(Disconnect()),this,SLOT(DisconnectManage()));
                        workerThread.start();
                    }
                    ~Controller() {
                        workerThread.quit();
                        workerThread.wait();
                    }
                private slots:
                    void DisconnectManage();
                };
                ```
                - 通过moveToThread()将Object对象移动到新线程，
                整个monitor都将在子线程中运行（感性理解）
                - 注意事项：
                    - 并不能认为monitor的控制权归属新线程，仍属于主线程
                    moveToThread()的作用是将槽函数在指定线程中调用，
                    仅有槽函数在指定线程中调用，包括构造函数等都在主线程中调用
                    - DisconnectMonitor必须继承自QObject，否则不能移动
                    - 如果Thread为nullptr，则该对象及其子对象的所有事件处理都将停止，因为它们不再与任何线程关联
                    - 调用 moveToThread() 时，移动对象的所有计时器将被重置。 
                    计时器首先在当前线程中停止，然后在targetThread中重新启动（以相同的间隔），这时定时器属于子线程。
                    若在线程之间不断移动对象可能会无限期地延迟计时器事件。
                    - moveToThread()线程不安全，
                    只能将一个对象move到另一个线程，
                    不能将对象从任意线程move到当前线程，除非这个对象不再与任何线程关联

            - 继承QThread的方式
            -
                ```c++
                    /*------------------------------WorkerThread-----------------------------------*/
                    class WorkerThread : public QThread
                    {
                        Q_OBJECT
                    public:
                        explicit WorkerThread();
                    protected:
                        void run();
                    signals:
                        void resultReady(const QString &s);
                    };

                    void WorkerThread::run(){
                        /* ... here is the expensive or blocking operation ... */
                    }

                    /*------------------------------MainWindow-----------------------------------*/
                    void MainWindow::startWorkInAThread()
                    {
                        WorkerThread *workerThread = new WorkerThread();
                        // Release object in workerThread
                        connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
                        workerThread->start();
                    }
                ```
                - 需要注意
                    - run()中未调用exec()开启event loop，那么在run()执行结束时，线程将自动退出
                    - 该案例中，WorkerThread存在于实例化它的旧线程中，
                    仅有run()中的代码是在子线程执行。
                    - 如果在 WorkerThread 的 run() 中使用了 WorkerThread 的成员变量，而且 QThread的其他方法也使用到了它，即我们从不同线程访问该成员变量，这时需要自行检查这样是否安全。
                    - QThread实例本身并不是一个线程
                    - Qt不推荐这种使用方式了
                    - 在 Qt 4.4 版本以前的 QThread 类是个抽象类，只能使用这种方式
        
        - 信号槽连接方式
            - Qt::AutoConnection
            - Qt::DirectConnection
            - Qt::QueuedConnection
            - Qt::BlockingQueuedConnection
            - Qt::UniqueConnection
        - 事件循环
            - 使用默认的run()方法或自行调用exec()，则QThread将开启事件循环。
            - QThread同样提供了exit()函数和quit()槽。
            - 如果一个线程没有开启事件循环，那么该线程中的timeout()将永远不会发射。
            - 如果在一个线程中创建了OBject 对象，那么发往这个对象的事件将由该线程的事件循环进行分派。
            - 可以手动使用 QCoreApplication::postEvent() 在任何时间先任何对象发送事件，该函数是线程安全的。
        - 如何正确退出线程
            - 删除QThread对象并不会停止其管理的线程的执行
            - 删除正在运行的线程的QThread将导致程序崩溃
            在删除QThread之前我们需要等待finish信号
            - 对于未开启事件循环的线程，我们仅需让run()执行结束即可终止线程
            - 对于开启了事件循环的线程，退出线程即退出事件循环
                - quit()/exit() + wait()
                - terminate() + wait()，很危险不推荐使用
                - finished
                    - 官方案例中都使用了finished信号，
                    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
                    - 如果QThread对象在栈上，则由操作系统自动完成，则析构时需要调用QThread::quit()和QThread::wait()
                    - 如果是堆分配，可以用过deleteLater来让线程自杀
                    QThread workerThread = new QThread();
                    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
            - 程序退出前，需要判断各线程是否都已退出
            如果线程的父对象是窗口对象，那么在窗体的析构函数中，还需要调用 wait() 等待线程完全结束再进行下面的析构。
        - 堆栈大小
            - 每个线程都有自己的栈，彼此独立，由编译器分配。一般在 Windows 的栈大小为 2M，在 Linux 下是 8M。
            - Qt 提供了获取以及设置栈空间大小的函数：stackSize() 、setStackSize(uint stackSize)。
            其中 stackSize() 函数不是返回当前所在线程的栈大小，而是获取用 setStackSize() 函数手动设置的栈大小。
        - 优先级
            - 可以通过QThread::setPriority()和QThread::priority()设置和获取线程优先级
            - QThread 类还提供了 sleep() 、msleep() 、usleep() 这三个函数，
            usleep() 并 不能保证准确性 。某些OS可能将舍入时间设置为10us/15us；在 Windows 上它将四舍五入为 1ms 的倍数。
        - 线程间通讯
            - 共享内存
            线程隶属于某一个进程，与进程内的其他线程一起共享这片地址空间。
            - 消息传递
            借助Qt的信号槽&事件循环机制。
        - 线程同步
        - 互斥锁，QMutex
        - 读写锁，QReadWriteMutex
        - 条件变量，QWaitCondition
        - 可重入与线程安全
            - 线程安全
            表示该函数可被多个线程调用，即使他们使用了共享数据，因为该共享数据的所有实例都被序列化了。
            - 可重入
            一个可重入的函数可被多个线程调用，但是只能是使用自己数据的情况下
            - 如果每个线程使用一个类的 不同实例 ，该类的成员函数可以被多个线程安全地调用，那么该类被称为可重入的；
            如果所有线程使用该类的 相同实例 ，该类的成员函数也可以被多个线程安全地调用，那么该类是线程安全的。
            - QObject的可重入性
            QObject是可重入的。它的大多数非GUI子类，如QTimer、QTcpSocket也都是可重入的，可以在多线程中使用。
            对于大部分 GUI类，尤其是 QWidget及其子类，都是不可重入的，我们只能在主线程中使用。QCoreApplication::exec() 也必须在主线程中调用。
            这些类被设计成在单一线程中进行创建和使用，在一个线程中创建一个对象，然后在另一个线程中调用这个对象的一个函数是无法保证一定可以工作的。需要满足以下三个条件：
                - QObject 的子对象必须在创建它的父对象的线程中创建。这意味这不要将 QThread 对象 (this) 作为在该线程中创建的对象的父对象。
                - 事件驱动对象只能在单一线程中使用。例如：不可以在对象所在的线程以外的其他线程中启动一个定时器或连接套接字。
                - 必须保证在删除 QThread 对象以前，删除在该线程中创建的所有对象。
        - 开启多少个线程合理
            - 可用核心数就是所有逻辑 CPU 的总数，这可以用 QThread::idealThreadCount() 静态函数获取

## QML
- QML语法规则
    - QML 是一种多范式语言，它允许根据对象的属性以及它们如何关联和响应其他对象中的变化来定义对象。
    QML 的声明性语法将属性和行为变化直接集成到单个对象的定义中。
    - import statements
        - import语句在QML文档顶部
        - import类型
            - 已将类型注册到其中的版本命名空间
            - 包含类型定义的QML文档所在的相对目录
            - JavaScript文件
            合法的JavaScript文档才能够被导入，其中的属性和方法才能够被使用
        -  examples:
            - import QtQuick
            - import QtQuick.LocalStorage 2.0 as Database
            - import "../privateComponents"
            - import "somefile.js" as Script
    - 对象声明
        - 一个 QML 代码块定义了一个要创建的 QML 对象树。
        - 
            ```json
            import QtQuick 2.0

            Rectangle {
                width: 100
                height: 100
                color: "red"
            }
            ```
        - 简单对象可以放在一行
        - 
            ```json
            Rectangle { width: 100; height: 100; color: "red" }
            ```
        - 子对象
            - 任何对象声明都隐式地声明一个可能包含任意数量的子对象的对象树。
# C++标准库
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
    - 3.1 C++语言新特性
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
        - 3.1.7 关键字noexcept
            - noexcept用来指明某个函数无法或不打算抛出异常。
            void foo() noexcept;
            若有异常未在foo()内被处理即如果foo()抛出异常，则程序会被终止
            然后std::terminate()被调用并默认调用std::abort()
            - 主要用于优化性能
            - 析构函数不能抛出异常，不要让异常逃离析构函数
                - 析构函数抛出异常问题
                    - 如果析构函数抛出异常，则异常点后的语句不会被执行
                    就有可能会导致资源泄露的问题
                    - 通常异常发生时，C++的异常处理机制在异常的传播过程中会进行栈展开（stack-unwinding），因发生异常而逐步退出复合语句和函数定义的过程即栈展开。
                    在栈展开过程中就会调用已经在栈构造好的对象的析构函数来释放资源，此时若其他析构函数本身也抛出异常，则前一个异常尚未处理，又有新异常，会造成程序崩溃。
                - 解决办法
                    - 结束程序
                    - 
                        ```c++
                        DBConn::~DBconn()
                        {
                            try
                            {
                                db.close(); 
                            }
                            catch(...)
                            {
                                abort();
                            }
                        }
                        ```
                    - 吞下因调用close而发生的异常
                    - 
                        ```c++
                        DBConn::~DBConn
                        {
                            try{ db.close();}
                            catch(...) 
                            {
                                //制作运转记录，记下对close的调用失败！
                            }
                        }
                        ```
                    - 重新设计DBConn接口，使其客户有机会对可能出现的异常做出反应
                    - 
                        ```c++
                        class DBConn
                        {
                        public:
                            ...
                            void close() //供客户使用的新函数
                            {
                                db.close();
                                closed = true;
                            }
                            ~DBConn()
                            {
                                if(!closed)
                                {
                                    try        //关闭连接(如果客户不调用DBConn::close)
                                    {       
                                        db.close();
                                    }
                                    catch(...) //如果关闭动作失败，记录下来并结束程序或吞下异常
                                    { 
                                        制作运转记录，记下对close的调用失败；
                                        ...
                                    }
                                }
                            }
                        private:
                            DBConnection db;
                            bool closed;
                        };
                        ```
            - noexcept(...)中可以指定一个Boolean条件，符合则不抛出异常
            is_nothrow_move_assignable type trait，判断传入的类型是否存在一个不抛出异常的move assignment操作符
        - 3.1.8 关键字constexpr
            - constexpr让表达式核定于编译期
            - 
                ```c++
                constexpr int square(int x) {
                    return x*x;
                }
                float a[square(9)];//OK since C++11：a has 81 elements
                ```
        - 3.1.9 崭新的Template特性
            - Variable Template
                - 可以接受个数不定的template实参
                - 
                    ```c++
                    template <typename T, typename... Types>
                    void print(const T& firstArg, const Types&... args) {
                        std::cout << firstArg << std::endl; // print first argument
                        print(args...); //call print() for remaining arguments
                    }
                    ```
            - Alias Template（带别名的模板，或者叫Template Typedef）
            - 
                ```c++
                template<typename T>
                using Vec = std::vector<T, MyAlloc<T>>; // standard vector using own allocator
                ```
        - 3.1.10 Lambda
            - Lambda的语法
            - 
                ```c++
                []{};
                [](){};
                [=](){}; 
                []() mutable, throwSpec, ->retType {};
                // mutable关键字的作用，对[=]捕获的变量进行 去const操作
                ```
            - 
                ```c++
                int id = 0;
                //在f中，id是static的？
                auto f = [id]() mutable {
                    std::cout << "id: " << id << std::endl;
                    id++;
                }
                //可以理解为如下的function object
                class {
                private:
                    int id; //copy of outside id
                public:
                    void operator()(){
                        std::cout << "id: " << id << std::endl;
                        id++;
                    }
                };
                ```
            - Lambda的类型是不具名的function object（functor）
            每个Lambda表达式的类型是独一无二的。
            可以使用std::function<> class template进行表示
        - 3.1.11 关键字decltype
            - decltype可以让编译器找出表达式（expression）类型
            - decltype主要用于声明返回类型
            超编程（mataprogramming）
            或用来传递一个lambda类型
        - 3.1.12 新的函数声明语法
            - 有时候函数的返回值类型取决于某个表达式对实参的处理
            - 
                ```c++
                template<typename T1, typename T2>
                auto add(T1 x, T2 y) -> decltype(x+y);  
                ```
            - 和“为lambda声明返回类型”是一样的
        - 3.1.13 带领域的（Scoped）Eunmeration
            - C++枚举的新的实现
            - 
                ```c++
                eunm class Salutation : char {mr, ms, co, none};
                ```
            - 优点
                - 绝不会隐式转换至/自int
                - 如果数值（例如mr）不在enumeration被声明的作用域内，则必须改写为Salutation::mr
    - 3.2 虽旧犹新的语言特性
        - 3.2.1 基础类型的明确初始化
        int i{}; //i被初始化为0
        - 3.2.2 main()定义式
        main()只有两种定义式具备可移植性
        - 
            ```c++
            int main(){}
            int main(int argc, char* argv[]){}
            ```
        - 如果不想以“main返回”的方式结束C++程序，通常应该调用exit()，quick_exit()（C++11之后）或terminate()。
- 第4章 一般概念
    - 4.1 命名空间
    - 4.2 头文件
    - 4.3 差错与异常（Error and Exception）的处理
    标准库有一部分，如string class，支持具体的差错处理，它们检查所有可能发生的差错，并于差错发生时抛出异常。
    其他部分，效率重于安全
        - 4.3.1 标准的Exception Class（异常类）
# 设计模式
- 设计模式简介
    - 面向对象
        - 底层思维：向下，如何把握机器底层，从微观理解对象构造
            - 语言构造
            - 编译转换
            - 内存模型
            - 运行时机制
        - 抽象思维：向上，如何将我们周围的世界抽象为代码
            - 面向对象
            - 组件封装
            - 设计模式
            - 架构模式
        - 深入理解面向对象
            - 向下：深入理解三大面向对象机制
                - 封装，隐藏内部实现
                - 继承，复用现有代码
                - 多态，改写对象行为
            - 向上：掌握什么是 “好的面向对象设计”
        - 软件设计固有的复杂性
            - 根本原因：变化
                - 客户需求的变化
                - 技术平台的变化
                - 开发团队的变化
                - ...
            - 如何解决复杂性
                - 分解，分而治之，例如结构化语言
                    - 分别存储各种图形，并且分别绘制
                - 抽象，忽略非本质细节，而是处理泛化和理想化的对象模型
                    - 将各种图形抽象为一个基类Shape，并提供一个同一个draw()接口
            - 软件设计的目标：复用
- 面向对象设计原则
    - 面向对象设计最大的优势：抵御变化
    - 重新认识面向对象
        - 隔离变化
        - 各司其职
        - 对象是什么
            - 语言层面，对象封装了代码和数据
            - 规格层面，对象是一系列可被使用的公共接口
            - 概念层面，对象是某种拥有责任的抽象
    - 1.依赖倒置原则（DIP）
        - 高层模块（稳定）不应该依赖于底层模块（变化），二者都应该依赖于抽象（稳定）
        - 抽象（稳定）不应该依赖于实现细节（变化），实现细节应该依赖于抽象（稳定）
    - 2.开闭原则（OCP）
        - 对扩展开放，对更改封闭
        - 类模块应该是可扩展二点，但是不可修改
    - 3.单一职责原则（SRP)
        - 一个类应该仅有一个引起它变化的原因。
        - 变化的方式隐含着类的责任
    - 4.Liskov替换原则（LSP）
        - 子类必须能够替换它们的基类（IS-A）
        - 继承表达类型抽象
    - 5.接口隔离原则（ISP）
        - 不应该强迫客户程序依赖它们不用的方法
        - 接口应该小而完备
    - 6.优先使用对象组合，而不是类继承
        - 类继承通常为“白箱复用”，对象组合通常为“黑箱复用”
        - 继承在某种程序上破坏了封装性，子类父类耦合度高。父类对子类暴露过多。
        - 而对象组合则只要求被组合对象具有良好定义的接口。耦合度低。
    - 7.封装变化点
        - 使用封装创建对象之间的分界层
    - 8.针对接口编程，而不是针对实现编程
        - 不将变量类型声明为某个特定的具体类，而是声明为某个接口
        - 客户程序无需获知对象的具体类型，只需要知道对象所具有的接口
- 模式分类
    - 从目的来看
        - 创建型
        - 结构型
        - 行为型
    - 从范围来看
        - 类模式处理类与子类的静态关系，继承
        - 对象模式处理对象间的动态关系，组合
    - 从封装角度对模式分类
- 重构获得模式
    - 寻找变化点，在变化点处应用设计模式
    - 什么时候、什么地点应用设计模式更重要
    - 不要一上来就是用设计模式，要Refactoring to Patterns（书名）
    - 重构关键技法
        - 静态 -> 动态
        - 早绑定 -> 晚绑定
        - 继承 -> 组合
        - 编译时依赖 -> 运行时依赖
        - 紧耦合 -> 松耦合
- 组件协作模式
    - 现代软件专业分工的结果“框架与应用程序的划分”，
    “组件协作”通过晚期绑定，来实现框架与应用程序间的松耦合
    - 模板模式
        - 动机：某一项任务，有稳定的整体操作结构，各个子步骤有很多改变的需求
        - application 早绑定-> library，晚实现绑定早实现
        - library 晚绑定-> application，早实现绑定晚实现
        - 模式定义
            - 定义一个操作中的算法的骨架（稳定），而将一些步骤（变化）延迟到子类。
            延迟：即基类定义虚函数让子类去实现
            - 子类可以不改变（复用）一个算法的结构又可重定义（override）该算法的某些特定步骤
        - 要点
            - 模板设计模式是一种非常基础性的设计模式
                - 机制简洁（虚函数的多态性）
                - 为许多应用程序框架提供了灵活的扩展点
                扩展：继承加多态
            - “不要调用我，让我调用你”的反向调用结构的典型应用
            
    - 策略模式
        - 动机：某些对象使用的算法多种多样，经常改变，
        如果将这些算法都编码到对象中，是对象变得复杂，
        而且支持不使用的算法也是一种负担。
        如何在运行时根据需要透明地改变对象的算法
        - 要动态地去看软件，加上时间轴，考虑未来可能的变化
        - 复用性：二进制层面的复用性即编译单位的复用性，而不是源代码片段级的复用
        - 模式定义
            - 定义一系列算法，把它们一个个封装起来，并且使它们可以相互替换（变化）
            - 该模式使得算法可独立于使用它的客户程序（稳定）而变化（扩展，子类化）
        - 要点
            - Strategy及其子类提供了一系列可复用的算法，使得类型可以在
            运行时
            方便地根据需要在各个算法间切换
            - 消除条件判断语句，看到很多“if else”或者switch的话就可以考虑使用策略模式
            - 如果Strategy对象没有实例变量（只有接口），那么各个上下文可以共享一个Strategy对象（单例模式），节省开销
    - 观察者模式/事件模式
        - 动机
            - 为某些对象建立一种“通知依赖关系”，
            目标对象的状态发生改变，所有的观察者对象都将得到通知。
        - 模式定义
            - 定义对象间的一种一对多（变化）的依赖关系
            当一个对象（subject）的状态发射改变时，所有依赖它的对象都得到通知并自动更新
        - 要点
            - 观察者模式使得我们可以独立地改变目标与观察者，使两者之前的关系达到松耦合
            - 目标发送通知时，无需指定观察者，通知会自动传播
            - 观察者自己决定是否订阅事件，目标对象对此一无所知
- “单一职责”模式
    - 如果责任划分不清晰，使用继承的结果往往是随着需求的变化，子类急剧膨胀，同时充斥着重复代码。
    - 装饰器模式
        - 动机
        某些情况下，过度使用继承来扩展对象的功能
        由于继承为类型引入的静态特质（FileStream::read()，NetworkStream::read()），使得这种扩展方式缺乏灵活性
        各种子类（扩展功能）的组合会导致子类数量的急剧膨胀
        - 定义
            动态（组合）
            地给一个对象增加一些额外的职责。就增加功能而言，Decorator模式比生成子类（继承）更为灵活（消除重复代码和减少子类个数）
        - 要点
            - 通过采用组合而非继承的手法，Decorator模式试了在
            运行时
            动态扩展对象功能的能力。
            - Decorator类在接口上表现为is-a component的继承关系，即Decorator类继承了Component类所具有的接口。
            但在实现上又表现为has-a component的组合关系，即Decorator又使用了另外一个Component对象。
            - Decorator模式的目的并未解决“多子类衍生的多继承”问题。
            Decorator应用的要点在于解决“主体类在多个方向上的扩展功能”。
            - 编译时复用，变化放到运行时
        - 
            ```c++
            // Component类
            class FileStream: public Stream {
            public:
                ...
                virtual char read(int number) {
                }
            }

            // Decorator类
            class CryptoStream: public Stream {
                Stream* stream;//FileStream, NetworkStream, ...
            public:
                ...
                virtual char read(int number) {
                    //额外的加密操作
                    stream->read(number);
                }
            }

            class BufferStream: public Stream {
                Stream* stream;//FileStream, NetworkStream, ...
            public:
                ...
                virtual char read(int number) {
                    //额外的缓存操作
                    stream->read(number);
                }
            }

            int main() {
                // 运行时装配
                FileStream *fs = new FileStream();
                //装饰 1
                CryptoStream *cs = new CryptoStream(fs);
                //装饰 2
                BufferStream *bs = new BufferStream(cs);
            }
            ```
    - 桥模式
        - 动机
- “对象创建”模式
    - 通过“对象创建”模式绕开new，避免对象创建（new）过程中所导致的紧耦合（依赖具体类），从而支持对象创建的稳定。这是接口抽象之后的第一步工作。
    - 工厂模式
        - 动机
        由于需求的变化，需要创建的对象的具体类型经常变化
        - 并不是消灭变化，而是把变化“关到”一个局部的地方
        - 
            ```c++
            //产品类
            class BinarySpliter: public ISpliter {}

            //工厂基类
            class SpliterFactory {
            public:
                ISpliter* createSpliter() = 0;
                virtual ~SpliterFactory() {}
            }

            class BinarySpliterFactory:public SpliterFactory {
            public:
                ISpliter* createSpliter() {
                    return new BinarySpliter();
                }
                virtual ~BinarySpliterFactory() {}
            }

            class User {
                SpliterFactory *factory;
            public:
                User(SpliterFactory *_factory): factory(_factory) {}

                void run(){
                    ISpliter *spliter = factory->createSpliter();
                }
            }
            ```
        - 要点
            - 工厂模式用于隔离类对象的使用者和具体类型间的耦合关系。
            - 工厂模式通过面向对象的手法，将创建具体对象的工作延迟到子类，从而实现一种扩展（而非更改）的策略。
            - 缺点在于要求创建方法/参数相同

# SLT各容器简单介绍
- 什么STL
    - STL（Standard Template Library），即标准模板库，是一个高效的C++程序库，包含诸多常用的基本数据结构和基本算法。
    - 从逻辑层次来看，在STL中体现了泛型化程序设计的思想。
    - 从实现层次看，整个STL是以一种类型参数化（type parameterized）的方式实现的，基于模板（template）
    - STL有六大组件，但主要包括容器、迭代器和算法三个部分
        - 容器（Containers）：用来管理某类对象的集合。每一种对象都有其优缺点，为了应付程序中的不同需求，STL准备了七种基本容器类型。
        - 迭代器（Iterators）：用来在一个对象集合的元素上进行遍历动作。这个对象集合或许是个容器，或许是容器的一部分。
        - 算法（Algorithms）：用来处理对象集合的元素，比如sort，search，copy，erase。
        通过迭代器的协助，我们只需要撰写一次算法，就可以将它应用于任意容器，因为所有容器的迭代器都提供一致的接口。
        - STL的基本观念是将数据和操作分离。数据由容器进行管理，操作则由算法进行，而迭代器在两者之间充当粘合剂，使任何算法都可以和任何容器交互运作。
- 容器（Containers）
    - 序列式容器（Sequence containers），可序集群，其中每个元素有固定位置取决于插入时机，和元素值无关。以追加方式向容器插入元素，排列次序和插入次序一致。
    STL提供了三个序列式容器：向量（vector），双端队列（deque），列表（list）
    还有queue，stack
    - 关联式容器（Associative containers），已序集群，元素位置取决于特定的排序准则以及元素值，和插入次序无关。
    STL提供了四个关联式容器：集合（set），多重集合（multiset），映射（map）和多重映射（multimap）
- vector
    - 类似与数组，但能够进行动态扩展。当内存空间不够时，会重新申请一块足够大的内存，并把原来数据拷贝过去（简单理解）。
    - 特点
        - 拥有一段连续的内存空间，支持随机访问，即[]操作符和at()
        - 当向其头部或中间插入或删除元素时，为了保持原本的相对次序，插入点或删除点之后的所有元素都必须移动，效率较低。
        - 在vector之后插入删除元素最快，此时一般不需要移动内存
    - 总结：相当于可扩展的数组（动态数组），随机访问快，在头部和中间插入或删除元素效率低，但在尾部插入或删除效率高。
    - 使用场景：适用于对象简单，变化较小，并且频繁随机访问的场景。
- deque
    - deque（double-ended queue）时双向开口的连续内存空间（动态将多个连续空间通过指针数组接合在一起），随时可以增加一段新的空间。
    deque最大的任务就是在这些分段的连续空间上，维护其整体连续的假象，并提供随机存取的接口。
    - 特点
        - 一旦要在deque的头部或尾部增加新空间，便配置一段定量连续空间，链接在整个deque的头部或尾部。
        因此不论在头部或尾部插入元素都十分迅速。
        - 在中间部分安插元素则效率较低，因为必须移动其他元素。
        - deque是list和vector的折中方案。兼有list的头尾插入快的优点，也有vector随机访问效率高的优点。
    - 总结：支持随机访问，但效率没有vector高，在头部和尾部插入或删除效率高，但在中间插入和删除效率低。
    - 适用场景：使用于既要频繁随机访问，又要关心两端数据的插入与删除的场景。
- list
    - list有双向列表（doubly linked list）实现，元素存放在堆中，每个元素都是放在一块内存中。
    没有空间预留习惯，所有没分配一个元素都会从内存中分配，每删除一个元素都会释放它占用的内存。
    - 特点
        - 内存空间不连续，通过指针来进行数据访问，所以随机存取效率很低，因此没有提供[]操作符
        - 由于链表的特点，在任意位置的插入和删除效率较高。
        - 只支持首尾两元素的直接存取，想获取其他元素，则需要遍历列表。
    - 总结：不支持随机访问，在任意位置的插入和删除效率都高。
    - 适用场景：适用于经常插入和删除操作并且不经常随机访问的场景。
- set
    - set（集合），数学上的集合——每个元素最多只出现一次，并且set中的元素已经从小到大排好序。
    - 特点
        - 使用红黑树实现，其内部元素依据其值自动排序，每个元素值只能出现一次，不允许重复。
        - 每次插入值的时候，都需要调整红黑树，效率有一定影响。
        - map和set的插入和删除效率比用其他序列容器高，因为对于关联容器来说，不需要做内存拷贝和内存移动。
        - 当set集合中的元素为类时，该类必须实现运算符 < 的重载
    - 总结：由红黑树实现，其内部元素依据其值自动排序，每个元素值只能出现一次，不允许重复，且插入和删除效率比用其他序列容器高。
    对于单次查询时间较为敏感，必须保持查询性能的稳定性，比如实时应用等等。

- map
    - map由红黑树实现，其元素都是“键值/实值”所形成一个对组（key/value pairs）
    map主要用于元素一对一映射的情况，map内部自建一颗红黑树，这棵树会根据key值自动排序，所以map内部所有的数据都是有序的。
    - 特点
        - 每个元素都有一个键，且只能出现一次，不允许重复
        - 根据key值能快速查找记录，查找的复杂度基本是O(logN)，效率高
        - 每次插入值的时候，都需要调整红黑树，效率由一定影响。
        - 对于迭代器来说，可以修改value，但不能修改key
        - 查询、插入、删除操作的时间复杂度都是O(logN)，性能稳定
    - 总结：元素为key-value pair，key和value可以是任意你需要的类型，每个元素都有一个键，且只能出现一次，不允许重复，根据key能够快速查找记录
    - 适用场景：适用于需要存储一个数据字典，并要求方便地根据key找value的场景
- multimap
    - multimap 和 map 相同，但允许重复元素，也就是说 multimap 可包含多个键值（key）相同的元素。
- unordered_map
    - unordered_map是基于哈希表（也叫散列表）实现的。通过把key值生成hash映射到hash表中的一个位置。
    - 特点
        - 元素无序存储。
        - 大多数情况下插入和查询的时间复杂度接近于O(1)（没有冲突的情况下）
        - 性能不是稳定的，如果hash函数映射的关键码出现的冲突过多，查询插入最坏时间复杂度也会达到O(n)。（但基本不会）
    - 适用场景：在需要元素有序性或者对单次查询性能要求较为敏感时，请使用map，其余情况下应使用unordered_map。

# OpenGL
- 多光源
    - GLSL中的函数
    - 使用多个光源的方法
    - 定向光

