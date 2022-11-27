## 20221114
### confusion
- MinGW 在链接时是链接到 *.a 库引用文件，生成的可执行程序运行时依赖 *.dll，而 Linux/Unix 系统里链接时和运行时都是使用 *.so。
### something new
- Explicit Linking 和 Implicit Linking

    动态链接库通常都有其导出函数列表， 告知其他可执行程序可以使用它的哪些函数。可执行程序使用这些导出函数有两种方式：一是在运行时使用主动加载动态库的函数，Linux 里比如用 dlopen 函数打开并加载动态库，Windows 里一般用 LoadLibrary 打开并加载动态库，只有当程序代码执行到这些函数时，其参数里的动态库才会被加载，这就是显式链接。显式链接方式是在运行时加载动态库，其程序启动时并不检查这些动态库是否存在。

    隐式链接是最为常见的，所有的编译环境默认都是采用隐式链接的方式使用动态库。隐式链接会在链接生成可执行程序时就确立依赖关系，在该程序启动时，操作系统自动会检查它依赖的动态库，并一一加载到该程序的内存空间，程序员就不需要操心什么时候加载动态库了。比如 VC 编译环境，链接时使用动态库对应的 .lib 文件（包含动态库的导出函数声明，但没有实际功能代码），在 .exe 程序运行前系统会检查依赖的 .dll。

## 20221115
### confusion
- Qt属性系统
- 隐式共享
- 光栅化与片段插值

    这个图片可能不是你所期望的那种，因为我们只提供了3个颜色，而不是我们现在看到的大调色板。这是在片段着色器中进行的所谓片段插值(Fragment Interpolation)的结果。当渲染一个三角形时，光栅化(Rasterization)阶段通常会造成比原指定顶点更多的片段。光栅会根据每个片段在三角形形状上所处相对位置决定这些片段的位置。
    
    基于这些位置，它会插值(Interpolate)所有片段着色器的输入变量。比如说，我们有一个线段，上面的端点是绿色的，下面的端点是蓝色的。如果一个片段着色器在线段的70%的位置运行，它的颜色输入属性就会是一个绿色和蓝色的线性结合；更精确地说就是30%蓝 + 70%绿。

    这正是在这个三角形中发生了什么。我们有3个顶点，和相应的3个颜色，从这个三角形的像素来看它可能包含50000左右的片段，片段着色器为这些像素进行插值颜色。如果你仔细看这些颜色就应该能明白了：红首先变成到紫再变为蓝色。片段插值会被应用到片段着色器的所有输入属性上。
### something new
- connectSlotsByName(QWDialog) 函数将搜索 QWDialog 界面上的所有组件，将信号与槽函数匹配的信号和槽关联起来，它假设槽函数的名称是：
void on_<object name>_<signal name>(<signal parameters>);
- 一个信号可以连接多个槽, 当一个信号与多个槽函数关联时，槽函数按照建立连接时的顺序依次执行

  一个信号可以连接另外一个信号
- Qt元对象系统
- Qt属性系统
- 隐式共享

    正确代码
    ```c++
    const QList<int> sizes = splitter->sizes();
    QList<int>::const_iterator i;
    for (i = sizes.begin (); i != sizes.end(); ++i)
        ...
    ```
    错误代码
    ```c++
    QList<int>::const_iterator i;
    for (i = splitter->sizes().begin(); i != splitter->sizes().end(); ++i)
    ```
    错误的原因是，每次调用QSplitter::sizes()都返回一个新的QList<int>值，如果不存储这个值，C++在开始迭代之前就会自动将其销毁，而只留下一个浮动迭代器。更糟的是，每次循环运行由于都调用了splitter->sizes().end()，QSplitter::sizes()都必须生成一个新的列表的副本。总之，如果你使用STL风格迭代器，总是要在返回值的容器副本上进行遍历。而Java Style不需要，因为它自动生成了副本.


## 20221116
### confusion
- libopengl_world.so.4.0.6
    https://blog.csdn.net/ShareProgress/article/details/121561252


## 20221117
### confusion

### something new
- 不要把所有图片都放到Qt资源文件，否则会导致源代码过大
- 创建和清理mat空间
    ```
    Mat mat(3000, 4000, CV_8UC3);
    mat.create(rows, cols, CV_8UC1); //会把之前的内存清理掉
    release或者析构 引用计数为1时释放
    ```
- isContinuous

    判断存储空间是否连续
    
    通过step记录

## 20221118
### confusion
- 片段插值
- 指定多级渐远纹理的级别

### something new
- Visual Studio
    
    相对路径起点默认是vcxproj文件所在目录，如LearnOpenGL.vcxproj在E:\Visual_Studio\LearnOpenGL\LearnOpenGL下，则E:\Visual_Studio\LearnOpenGL\LearnOpenGL\src下的main.cpp中的
    "res/shaders/shader.vs"相当于"E:\Visual_Studio\LearnOpenGL\LearnOpenGL\res\shaders\shader.vs"

## 20221119
### confusion
- 
### something new
- 在静态库中最好不要去存放全局变量，也不要在这里创建单例对象等。如果对程序文件大小有要求，最好使用动态库。


## 20221120
### confusion
### something new
- C++11规定了local static在多线程条件下的初始化行为，要求编译器保证了内部静态变量的线程安全性


## 20221123
### confusion
### something new
- 编译ffmpeg，https://blog.csdn.net/DemoFY/article/details/127636507
sudo ./configure --enable-version3 --enable-gpl --enable-nonfree --enable-small --enable-libmp3lame --enable-libx264 --enable-libx265 --enable-libvpx --enable-libtheora --enable-libvorbis --enable-libopus --enable-libfdk-aac --enable-libass --enable-libwebp --enable-librtmp --enable-postproc --enable-libfreetype --enable-pthreads --enable-openssl --enable-shared --disable-debug --disable-doc --disable-ffplay 