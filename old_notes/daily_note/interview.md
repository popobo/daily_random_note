## 总结一下完成每个项目收获了什么
- 基于Qt, FFmpeg的视频播放器
    - 1. 通过这个项目实践了多种设计模式
        - 设计模式原则
            - 依赖倒置原则：依赖指的是二进制依赖，编译上的依赖；
            高层模块不应依赖底层模块，二者都应依赖抽象；
            抽象不应依赖于实现细节，实现细节应该依赖抽象；
            即稳定不应依赖变化，变化应该依赖抽象；
            - 开放封闭原则：对扩展开放，对更改封闭，
            类模块应该是可扩展的，但是不可修改；
            - 单一职责原则：一个类应该仅有一个引起它变化的原因，变化的方向隐含着类的责任
            - 接口隔离原则：不应该强迫客户程序依赖它们不用的方法，接口应该小而完备
            - 优先使用组合而非继承：继承在某种程序破坏了封装性（子类可以访问到父类的方法，重载父类的方法等），子类父类耦合度高，
            对象组合只要求被组合的对象具有良好定义的接口，耦合度低
        - 观察者模式
            - 建立松耦合的通知依赖关系，可以独立地改变目标与观察者
            - 结合信号槽来讲
            - Demux解复用器，Decoder音频解码器，视频解码器，OpenGL Renderer渲染器，AudioPlayer就是通过观察者模式串联起来，通过抽象出一个数据基类作为通知函数notify的参数，来传递各种音视频数据，如FFmpeg的AVPacket，AVFrame，Audio数据等，通知的数据到达各个模块后，通过队列进行缓存，每个模块各自的线程里从数据队列中取出数据进行相应的处理
        - 适配器模式
            - 用于转化接口
            - 目标接口：客户端期望的接口，适配器：持有原有类实现目标接口，原有类
        - 工厂模式
            - [工厂模式](CPP\Design_Pattern\Factory_Pattern\MainForm2.cpp)，设计模式并不是消灭所有变化，而是把变化赶到一个局部的地方
            - ![工厂模式](CPP\Design_Pattern\Factory_Pattern\Snipaste_2023-07-15_11-26-52.png)
            - 用于创建对象而不需要将具体类暴露给客户端，将对象的创建与使用解耦
            - 是依赖倒置原则的实践，避免客户端（编译）依赖具体类，让客户端绕开new操作，多态new
            - 抽象工厂，抽象产品，具体工厂，具体产品
        - 单例模式
            - 线程非安全版本
            - 线程安全版本，一把大锁治天下，缺点就是读取操作频繁的话，效率很低
            - 双检查锁模式，锁前检查一次，可以避免频繁读取效率低问题，锁后检查一次，避免临界状况出现导致创建两次，但因为内存读写reorder不安全
                - m_instance = new Singleton();
                    - 假定new执行顺序，分配内存，执行构造函数，返回分配内存地址，但由于编译器优化和CPU乱序执行等原因，会导致new执行顺序可能变成分配内存，返回分配内存地址，执行构造函数，这就导致了锁前锁后判断都失去了意义。
            - C++11之后静态局部变量的初始化是线程安全的了，可以使用这个特性实现懒汉式单例模式，然后使用静态成员变量实现饿汉式单例模式
    - 2. 通过这个项目熟悉了Qt中的信号槽机制，多线程机制，事件循环
        - 信号槽机制，多线程，事件循环
            - 观察者模式的一种实现
            - 支持槽函数和信号参数不对等的情况，槽函数参数类型只要是信号参数子序列时就可以通过编译
            - 槽函数的类型支持广泛，支持类成员函数，lambda表达式，仿函数，普通函数，静态函数
            - 支持断开连接操作
            - 支持对象死亡时自动断开连接
            - 支持异步的编程方式
                - 每一个QObject都有一个自己所属的线程，在有信号触发这个对象相关的槽函数的时候，会将其打包成一个任务投递到目标线程的消息队列中去，然后每一个线程都有一个事件循环，不断地从消息队列中取出任务执行
    - 3. 通过这个项目尝试了一下集成使用ffmpeg库和OpenGL
        - 熟悉了ffmpeg的使用方法以及ffmpeg中各种资源是如何管理的，FFmpeg中各种资源的生命周期是由ffmpeg库内部自行管理，采用上下文和引用计数的方法，客户程序通过ffmpeg提供的API对资源进行资源深浅拷贝以及上下文的管理
        - 使用OpenGL将yuv420p数据转换成rgb数据，能够编写简单的shader对数据进行并行处理

        
- NJU ICS PA
    - 1. 计算机是个状态机，程序是个状态机
    - 2. 熟悉了汇编语言，能够读懂汇编和写一点简单的汇编，包括riscv架构，x86架构，arm架构
    - 3. 学习了一些二进制工具的使用方法objdump反汇编可执行可链接文件，readelf查看和分析elf文件内容（节表，符号表等），objcopy复制elf文件的代码段，addr2line查询elf文件中的地址得到对应源代码位置
    - 4. 熟悉gdb的使用，设置断点breakpoint，观察点watch point，设置layout，编写gdb启动脚本，连接远端的gdb进行调试等
    - 5. 知道如何配置vscode来调试大型C++程序，使用bear生成compile_commands.json即可，再将compile_commands.json配置C/C++插件中
    - 6. 能够读懂复杂的Makefile，该项目中为了管理多个架构，多个模块，Makefile是较为巧妙和复杂的
    - 7. PA1
        - 实现一个简易调试器，通过realline读取命令，再通过strtok去解析命令，然后调用CPU模拟器nemu提供的接口进行相应的操作，单步执行，打印寄存器等
        - 实现表达式求值，包括词法分析，递归求值，同时实现了一个表达式生成器，主要思路是在程序中生成一个随机表达式放到一个临时C文件里，编译执行得到结果
    - 8. PA2
        - 了解CPU取指，译码，执行，更新PC
        - 读懂riscv指令集手册，并在CPU模拟器nemu实现相应指令的取指，译码和执行，可能大概实现了30多条指令
        - 了解什么是运行时环境，了解了硬件，运行时环境，应用程序间的关系
        - 实现了运行时环境的输入输出的部分模块，知道了是什么是端口I/O，在I/O指令中给出对应端口号，CPU就知道要访问那一个设备寄存器， 
        常见设备端口号由专门的规定，简单但是限制很大，为了兼容性，只能添加不能修改，指令集I/O地址空间有限
        - 内存映射，将物理地址空间中的一部分划分出来给到I/O去使用，访问这部分地址即访问设备
    - 9. PA3
        - 批处理系统
            - 一个后台程序，当一个前台程序执行结束后，后台程序自动加载一个新的前台程序来执行
        - 实现批处理操作系统中程序上下文的管理，包括自陷操作，保存上下文，恢复上下文等内容
        - 实现加载用户程序，实现系统调用，标准输入输出和堆区管理
        - 实现简易的虚拟文件系统
    - 10. PA4
        - 实现操作系统的上下文切换
            - 进程控制块，内核线程，用户进程等概念
        - 实现了操作系统的虚存管理
            - 实现分页机制和MMU
        - 实现分时多任务（用户程序主动让出）和抢占多任务（定时中断强行切换）

- 操作系统
    - 1. 对于应用程序而言，操作系统就是一系列的对象和API，对象包括文件，进程，线程，锁，信号量等，API即创建，读写，销毁等
    - 2. 多处理器编程：从入门到放弃
        - 放弃原子性：“处理器一次执行一条指令” 的基本假设在今天的计算机系统上不再成立
        - 放弃执行顺序：编译器的优化会与多处理器运行逻辑冲突，[sum.c](C/OS/sum.c)
        - 放弃处理器间的可见性：现代处理器也是一个动态编译器，电路将连续的指令“编译”成更小的uops，
        [store-load.c](C/OS/store-load.c)，当x，y指代的不同的内存的时候，对x，y的内存读写可以交换顺序。

- Linux嵌入式开发入门
    - uboot简介
        - Universal Boot Loader，负责初始化DDR（硬盘）等外设，将Linux内核从flash（NAND，NOR，SD）拷贝到内存中
    - 交叉编译uboot
    - uboot烧写与启动
    - uboot命令的使用
        - 信息查询命令，看板子信息，有DRAM的起始地址和大小、启动参数保存起始地址、波特率、sp(堆栈指针)起始地址等。
        - 环境变量操作命令，新增，修改，保存环境变量
        - 内存操作命令，对内存进行查询，写入，复制等操作
        - 网络操作命令，配置网络环境
        - EMMC和SD卡操作命令，查询，写入等
        - 文件系统操作命令，查询，写入等
        - tftp，nfs的使用，用于从远端复制Linux镜像
        - boot命令，用来指定如何启动Linux
    - uboot Makefile
    - 了解了一下uboot的启动流程
        1. 机器上电和复位：
            - 系统上电后，处理器会进入复位状态，所有寄存器都被清零。
            - 处理器（Program counter）会跳转到预定义的启动地址，通常是存储器的起始地址。
        2. 硬件以及处理器初始化：
            - U-Boot 首先进行基本硬件的初始化，如串口、内存控制器等。
            - 进行处理器体系结构相关的初始化，如设置中断向量表等。
        3. 读取和解析 U-Boot 配置文件：
            - U-Boot 会尝试从存储介质（如闪存、SD卡等）中读取一个配置文件（例如 uEnv.txt 或者 boot.scr）。
            - 配置文件包含了启动参数、环境变量等信息，用于后续的启动过程。
        4. 加载操作系统内核镜像：
            - U-Boot 根据配置文件中指定的加载地址和文件名，从存储介质中加载操作系统的内核镜像到内存中。
            - U-Boot 还会加载根文件系统、设备树和其他需要的组件。
        5. 设置操作系统启动参数：
            - U-Boot 会根据配置文件中的启动参数，如内核命令行参数、设备树地址等，设置启动参数。
            - 这些参数将被传递给操作系统内核，用于操作系统的初始化和配置。
        6. 跳转到操作系统：
            - U-Boot 通过设置处理器寄存器，将控制权交给操作系统内核。
            - 控制权转移后，操作系统将继续完成系统的初始化和启动过程。
    - uboot的移植
        - 由于不同的硬件平台和系统架构具有不同的硬件组件和外设，因此需要对U-Boot进行适当的修改和配置
        - 配置硬件参数，交叉编译uboot，配置设备树等
    - Linux内核的编译与vscode工程配置
        - 使用compile_commands.json
    - Linux内核启动流程
        - 内核初始化：
            - Linux内核在初始化过程中会进行一系列的初始化操作，包括设置页表、初始化内存管理、设备驱动的注册等。
            - 内核会根据设备树（Device Tree）中的描述来识别和初始化硬件设备。
        - 初始化进程和用户空间：
            - 内核初始化完成后，它会启动第一个用户空间进程（通常是init进程）。
            - init进程负责执行系统初始化脚本、加载用户空间程序，完成系统初始化和用户环境的准备。
        - 用户空间初始化：
            - 用户空间进程会根据启动脚本和配置文件，执行相应的初始化工作，如挂载文件系统、启动系统服务等。
            - 此时，系统进入用户空间，用户可以开始使用系统的各种功能和服务。
    - 用BusyBox构建根文件系统
    - 系统的烧写，烧写uboot镜像，设备树，根文件系统，Linux内核镜像
    - 字符设备驱动开发
    - Linux设备树
        - 设备树文件（DTS device tree source），采用树形结构描述机器上的设备信息
        - 树的主干就是系统总线， IIC 控制器、 GPIO 控制器、 SPI 控制器等都是接到系统主线上的分支
        - DTS语法
            - dtsi头文件，设备树中一些共同的信息是通用的，放在dtsi中
            - 设备节点，设备树中每个设备都是节点，每个设备节点都会有一些属性描述
            - 标准属性
                - compatible属性，用于设备和驱动间的匹配
                - model属性，设备名字等
                - status属性
    - pinctrl子系统和gpio子系统
        - 驱动分离是指将硬件的不同功能和模块分别实现为独立的驱动程序
        - 驱动分层是指将驱动程序按照功能和抽象层次进行划分，形成多个层次化的驱动结构。例如，在一个网络设备的驱动中，底层驱动层负责与网络硬件交互，控制数据的发送和接收。而上层驱动层负责实现网络协议栈，处理网络数据包、连接管理等更高级别的网络功能。
        - pinctrl子系统会获取设备树中pin的信息，然后对其进行相应的复用和初始化（初始化设备寄存器等）
    - RTC驱动，I2C驱动，SPI驱动