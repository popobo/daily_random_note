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
            - emit 是一个空宏
            #define emit
            - 源码分析
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