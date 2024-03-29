# PA1 开天辟地的篇章：最简单的计算机
- 在开始愉快的PA之旅之前
    - NEMU是什么？
    - 选择你的角色
    - 还等什么呢？
- 开天辟地的篇章
    - 最简单的计算机
        - 计算机可以没有寄存器吗？
        如果没有寄存器，计算机仍然可以工作，但是它的执行效率会受到很大的影响，因为没有寄存器的计算机必须从主存储器中读取和存储数据，这会导致非常慢的执行速度。
        在没有寄存器的情况下，计算机的编程模型也会发生很大变化。通常，程序员需要在每个指令中明确地指定操作数的内存地址，而不是将它们存储在寄存器中。这将导致更长的指令序列和更慢的执行速度。
        此外，缺乏寄存器可能会使编写高级编程语言变得更加困难。现代编译器和解释器通常使用寄存器来管理变量和临时数据，从而在运行时提高程序的执行效率。没有寄存器可能需要使用更复杂的技术来实现相同的优化，这会使编译器的实现更加复杂。
        - 尝试理解计算机如何计算
        - 计算机是个状态机
    - 重新认识程序：程序是个状态机
        - 从状态机视角理解程序运行
        - 程序的状态机视角有什么好处？
        - "程序在计算机上运行"的微观视角：程序是个状态机
- RTFSC
    - 框架代码初探
        - 需要多费口舌吗？
        - 对vim的使用感到困难？
    - 准备第一个客户程序
        - 为什么全部都是函数？
        - 参数的处理过程
        - BIOS和计算机启动
        - 初探操作系统启动
        - 物理内存的起始地址
        - 实现x86的寄存器结构体
            - 什么是匿名union?
            - reg_test()是如何测试你的实现的?
    - 运行第一个用户程序
        - 究竟要执行多久?
        - 潜在的威胁
        - 谁来指示程序的结束？
        - 有始有终
        - 理解框架代码
        - RTFSC != 盯着代码看
        - 阅读Makefile
    - 就这么简单
- 基础设施
    - 基础设施-提高项目开发的效率
        - 真实故事
        - 总有一天会找上来的bug
    - 解析命令
        - 如何测试字符串处理函数？
    - 单步执行
    - 打印寄存器
    - 扫描内存
        - 实现单步执行，打印寄存器，扫描内存
        - 我怕代码写错了啊，怎么办？
    - 表达式求值
        - 数学表达式求值
            - 词法分析
                - 实现算术表达式的词法分析
                - 调试公理
                - 如何调试
                - 为什么printf()的输出要换行?
                printf()函数在执行输出操作时会使用缓冲区（buffer），将需要输出的数据先暂存在缓冲区中，待缓冲区满或遇到换行符'\n'时才将缓冲区中的数据一次性输出到目标设备上，比如屏幕或文件等。
                这种使用缓冲区的方式可以提高输出效率，减少频繁的I/O操作对系统性能的影响。但也会带来一些问题，比如缓冲区未满时程序异常退出导致缓冲区中的数据没有被输出；或者在不及时刷新缓冲区的情况下，需要等到缓冲区满或遇到换行符才能看到输出结果，可能会给调试程序带来困难等等。
                因此，在需要及时看到输出结果或需要确保缓冲区中的数据被及时输出的情况下，可以使用fflush()函数来刷新缓冲区。例如，调用printf()输出一条消息后，紧接着调用fflush(stdout)来确保消息被及时输出。
                - 系统设计的黄金法则-KISS法则
            - 递归求值
                - 实现算数表达式的递归求值
                - 实现带有负数的算数表达式的求值（选做）
                - 从表达式求值窥探编译器
                - 如何测试你的代码
        - 如何测试你的代码
            - 表达式生成器如何获得C程序的打印结果
            - 为什么需要使用无符号类型
            - 除0的确切行为
            - 过滤除0行为的表达式
            - 实现表达式生成器
    - 监视点
        - 扩展表达式求值的功能
            - 扩展表达式求值的功能
            - riscv64中的表达式求值
            - 测试的局限性
        - 实现监视点
            - 实现监视点池的管理
            - 温故而知新
            - 实现监视点
    - 调试工具与原理
        - 你会如何测试你的监视点的实现
        - 强大的GDB
    - 断点
        - 如何提高断点的效率
        - 一点也不能长
        - 随心所欲的断点
        - NEMU的前世今生
- 如何阅读手册
    - 学会使用目录
        - 尝试通过目录定位关注的问题
    - 逐步细化搜索范围

# PA2-简单复杂的机器：冯诺依曼计算机系统
- 不停计算的机器
    - 不停计算的机器
        - 取指(instruction fetch, IF)
        - 译码(instruction decode, ID)
        - 执行(execute, EX)
        - 更新PC
    - YEMU: 一个简单的CPU模拟器
        - 理解YEMU如何执行程序
- RTFSC(2)
    - RTFM
        - i386手册勘误
        - RISC-与CISC平行的另一个世界
        - RISC-V-一款设计精巧的指令集
    - RTFSC(2)
        - 取指(instruction fetch, IF)
        - 译码(instruction decode, ID)
            - 操作数宽度的处理
            - x86的变长指令
            - 立即数背后的故事
            - 立即数背后的故事(2)
            对于MIPS32和RISC-V32这样的32位指令集架构，无法像x86那样直接编码32位常数到一条指令中。这是因为指令长度限制了常数的大小，而且指令中的大部分位用于指定操作码和寄存器操作数。
            为了解决这个问题，MIPS32和RISC-V32使用了两种主要的方法：
            立即数寻址：这种方法允许将一个小的常数（通常为16位或20位）直接编码到指令中，然后在执行时将其扩展为32位。例如，在MIPS32指令集中，可以使用ADDI指令将16位的立即数添加到寄存器中。同样，在RISC-V32指令集中，可以使用ADDI指令将12位的立即数添加到寄存器中。
            加载立即数：另一种方法是将常数存储在内存中，并使用加载指令将其加载到寄存器中。这种方法允许加载更大的常数（通常为32位），但需要额外的指令和存储器访问。例如，在MIPS32指令集中，可以使用LUI指令将16位的立即数加载到寄存器的高位，然后使用ORI指令将另一个16位的立即数加载到寄存器的低位，从而将32位的常数加载到寄存器中。同样，在RISC-V32指令集中，可以使用LUI和ADDI指令将32位的立即数加载到寄存器中。
            总的来说，MIPS32和RISC-V32可以使用立即数寻址和加载立即数这两种方法，将常数加载到寄存器中，以执行操作。这两种方法都有其优点和缺点，需要根据具体情况进行选择。
        - 执行(execute, EX)
        - 更新PC
            - 静态指令和动态指令
            - 驾驭项目, 而不是被项目驾驭
            - 拦截客户程序访存越界的非法行为
    - 结构化程序设计
        - Copy-Paste - 一种糟糕的编程习惯
        - 指令执行的共性部分
        - 用RTL表示指令行为
            - 64位ISA特有的RTL基本指令
            - RTL指令和二进制翻译
            - 神秘的host内存访问 (建议二周目思考)
            - 小型调用约定
            - 计算机系统中的约定与未定义行为
            - UB, 编译优化和datalab
            - RTL寄存器中值的生存期
        - 实现新指令
            - 感觉RTL临时寄存器不够用了?
            - 偏心的框架代码
            - RTFSC理解指令执行的过程
        - 运行第一个C程序
            - 准备交叉编译环境
            - 为什么执行了未实现指令会出现上述报错信息
            - 我选择x86, 但出错信息中显示的字节序列有所不同
            - 交叉编译工具链
            - 去除endbr32指令
            - 再提供一些x86的提示吧
            - 运行第一个客户程序
- 程序，运行时环境与AM
    - 运行时环境
    为了运行最简单的程序, 我们需要提供什么呢? 
    其实答案已经在PA1中了: 只要把程序放在正确的内存位置, 然后让PC指向第一条指令, 计算机就会自动执行这个程序, 永不停止.

    - 将运行时环境封装成库函数
    运行时环境的一种普遍的存在方式: 库. 
    通过库, 运行程序所需要的公共要素被抽象成API, 不同的架构只需要实现这些API
        - 这又能怎么样呢
    - AM-裸机(bare-metal)运行时环境
        - AM的诞生和Project-N的故事
        - 穿越时空的羁绊
        - 为什么要有AM？
            解耦ISA与Nanos-lite
    - RTFSC
        - 阅读Makefile
    - 运行更多的程序
        - 实现更多的指令
        - 不要以为只需要在TODO处写代码
        - 调试的利器-日志
        - x86指令相关的注意事项
        - mips32的分支延迟槽
        - mips32-NEMU的分支延迟槽
        - 指令名对照
    - 实现常用的库函数
        - 实现字符串处理函数
        - 免责声明
        - 实现sprintf
        - stdarg是如何实现的?
    - 重新认识计算机：计算机是个抽象层
        - "程序在计算机上运行"的宏观视角: 计算机是个抽象层
        - PA究竟在干什么
- 基础设施
    - AM作为基础设施
        - 如何生成native的可执行文件
        - 奇怪的错误码
        - 这是如何实现的？
        - 先实现memcpy()
        - 编写可一直的程序
    - 测试你的klib
        - 编写更多的测试（1）
        - 这些库函数这么简单，我可以不测试吗？
        - 编写更多的测试（2）
        - 编写更多的测试（3）
    - Differential Testing
        - 基础设置-龙芯杯获胜的秘诀
        - 实现DiffTest
        - 需要校准的指令
        - KVM: 一个比QEMU更高效的REF
        - 使用QEMU作为REF时, 不要同时运行两份NEMU
        - 匪夷所思的QEMU行为 (有点难度)
    - 一键回归测试
        - NEMU的本质
        - 捕捉死循环（有点难度）
            - 状态机视角，状态空间有一个闭环
- 输入输出
    - 设备与CPU
    访问设备 = 读出数据 + 写入数据 + 控制状态.
    - 端口I/O
        - 有没有一种熟悉的感觉
    - 内存映射I/O
        理解volatile关键字
    -  状态机视角下的输入输出
    - NEMU中的输入输出
        - 映射和I/O方式
    - 设备
    - 将输入输出抽象成IOE
    - 串口
        - 运行Hello World
        - 设备和DiffTest
        - 理解mainargs
        - 实现print
    - 时钟
        - 实现IOE
        - 不要再native连接到klib时运行IOE相关的测试
        - RTFSC尽可能了解一切细节
        - 看看NEMU跑多快
        - 如何调试复杂程序
        - NEMU和语言解释器
        - 先完成，后完美-抑制住优化代码的冲动
        - 运行红白机模拟器
    - 键盘
        - 神秘的MAP宏
        - 实现IOE（2）
        - 如何检测多个键同时被按下？
        - 运行红白机模拟器（2）
    - VGA
        - 神奇的调色板
        - 实现IOE（3）
        - 实现IOE（4）
        - 运行红白机模拟器（3）
    - 声卡
        - 实现声卡
        - 一些提示
        - 注意音量，保护耳朵
        - 音频播放的原理
        - 播放自己的音乐
        - 运行红白机模拟器
    - 冯诺依曼计算机系统
        - 展示你的计算机系统
        - 游戏是如何运行的
        - 体会到AM的好处了吧？
        - RTFSC指南
        - LiteNES如何工作的
        - 优化LiteNES
# PA3-穿越时空的旅程：批处理系统
- 最简单的操作系统
    - 没有调不出的bug，只有不理解的系统
    - 批处理系统
        - 自动加载新程序
        - 什么是操作系统？
            - 硬件管理
            - 进程和线程管理
            - 文件系统管理
            - 用户界面
            - 安全管理
            - 系统资源管理
    - 最简单的操作系统
        - Nanos-lite
        - 操作系统是个C程序
    - 来自操作系统的新需求
        - 硬件需要提供一种可以限制入口的执行流切换方式
    - 等级森严的制度
        - DPL
        - RPL
        - CPL
        - 对RPL的补充
            - ”操作系统代表用户进行操作“
        - 保护相关的概念和检查过程都是硬件实现的
        - 分崩离析的秩序
            - Meltdown和Spectre
- 穿越时空的旅程
    - 穿越时空的旅程
        - 硬件需要提供一种可以限制入口的执行流切换方式
            - 自陷指令
            - 异常入口地址
            - 中断/异常响应机制
        - x86
            - 自陷指令 int
            - 异常入口地址 门描述符
        - mips32
            - 自陷指令 syscall
            - 异常入口地址总是 0x80000180
        - riscv32
            - 自陷指令 ecall
            - CSR寄存器
            - stvec存储异常入口地址
        - riscv32的特权模式的简化
        - 特殊的原因？
            - 这些程序状态必须由硬件来保存，不能通过软件来保存。因为这些状态与CPU的执行状态密切相关，保存在硬件中可以更加高效地管理和调度CPU的运行。同时，这些状态通常被用于中断处理、异常处理、进程切换等操作，这些操作涉及到对CPU的直接控制，必须由硬件来完成。
            保存这些状态需要用到CPU的特殊寄存器，这些寄存器的访问权限通常是受限的，并且只能通过硬件指令来访问和修改。而软件只能通过特定的指令或函数调用来间接地操作这些状态，而不能直接保存或修改这些状态。
            另外，这些程序状态中的一些状态（如EFLAGS、STATUS、SSTATUS等）还涉及到CPU的运行模式和特权级别等问题，必须由硬件来控制和管理。因此，这些状态必须由硬件来保存，不能通过软件来保存。
        - 异常处理结束后，恢复程序状态
        - 状态机视角下的异常响应机制
            - 系统寄存器 R={GPR, PC, SR}
            - 状态转移的扩充 raise_intr
        - 将上下文管理抽象成CTE
            - 引发执行流切换的原因
            - 程序的上下文
            - 用软件模拟指令
            - 在AM中执行浮点指令是UB
            - 另一个UB
            - 设置异常入口地址
                - 触发自陷操作
                - 实现异常响应机制
        - 保存上下文
            - x86 pusha 把通用寄存器的值压栈 mips32 riscv32 把通用寄存器压栈
            - 触发异常时的PC和处理器状态
            - 异常号
            - 地址空间
            - 异常号的保存
            - 对比异常处理和函数调用
                - 函数调用和上下文保存是两种不同的概念和需求，虽然它们都涉及到保存状态信息，但其目的、内容和方式可能不同。
                在函数调用中，主要目的是保存调用者的状态，包括返回地址和一些需要调用者保存的寄存器。这是为了在函数调用结束后能够正确返回到调用点，并保持调用者在函数调用前的状态不受影响。调用者保存的寄存器通常是由编程约定（calling convention）来规定的，以确保在函数调用过程中，被调用函数可以使用一些寄存器来保存临时数据，而不会影响调用者的状态。
                而上下文保存则是在异常处理、进程切换等情况下，需要保存当前的系统状态，包括当前指令地址、寄存器状态、控制寄存器等信息，以便在恢复执行时能够继续从中断或异常发生时的状态开始执行，确保系统能够从中断或异常中恢复正常执行。
                上下文保存需要保存的信息通常比函数调用要更多，因为它需要保存整个系统的状态，包括当前指令地址、控制寄存器、中断状态、处理器状态等。这些信息在异常处理或进程切换时都是必要的，以确保能够正确地恢复系统
            - 诡异的x86代码
            - 重新组织Context结构体
            - 给一些提示吧
            - !我乱改一通，居然通过了，嘿嘿嘿
            - 必答题-理解上下文结构的前世今生
        - 事件分发
        - 恢复上下文
            - 恢复上下文
            - 必答题-理解穿越时空的旅程
            - mips32延迟槽和异常
- 用户程序和系统调用
    - 加载第一个用户程序
        - 可执行文件在哪里
        - 代码和数据在可执行文件的哪个位置
        - 代码和数据有多少
        - ”正确的内存位置“在哪里
        - navy-apps
            - Newlib 专为嵌入式系统提供的C库
            - navy-apps/libs/libos/src/crt0/start/$ISA.S _start->call_main()->main()->exit()
            - C库的代码”总是“对的
            - PE(Portable Executable)
            - ELF(Executable and Linkable Format)
            - System V ABI(Application Binary Interface)
                - API defines the order in which you pass arguments to a function. 
                - ABI defines the mechanics of how these arguments are passed(registers, stack, etc.). 
                - API defines which functions are part of your library
                - ABI defines how code is stored inside the library file
            - 堆和栈在哪里？
            - 如何识别不同格式的可执行文件？
            - ELF文件
                - 面向链接过程的section视角
                    - 提供了用于链接与重定位的信息
                - 面向执行的segment视角
                    - 提供了加载可执行文件的信息
                - 一个segment可能由0个或多个section组成, 但一个section可能不被包含于任何segment中.
            - 冗余的属性？
            - 实现loader
            - 检查ELF文件魔数
            - 检查ELF文件的ISA类型
            - 将Nanos-lite编译到native
    - 操作系统的运行时环境
        - 程序运行需要运行时环境支撑
        - 运行时环境的功能需要使用资源
        - 操作系统对资源统一管理，服务用户程序，以一种统一的接口来呈现
        - 系统调用
            - 内核区
            - 用户区 strcpy()
        - 系统调用的必要性
    - 系统调用
        - yield()
        - _syscall_()
        - 识别系统调用
        - 实现SYS_yield系统调用
        - 实现SYS_exit系统调用
    - 操作系统之上的TRM
        - 标准输出
            - 在Nanos-lite上运行Hello world
        - 堆区管理
            - 实现堆区管理
            - 缓冲区与系统调用开销
            - printf和换行
    - 必答题-hello程序是什么，它从何而来，要到哪里去
- 简易文件系统
    - 文件的本质就是字节序列，另外由一些额外的属性构成
    - 记得更新镜像文件
    - 文件描述符
    - 文件偏移量和用户程序
    - 让loader使用文件
    - 实现完整的文件系统
    - 记得更新应用程序列表
- 一切皆文件
    - 一切都是字节序列，一切都是文件
    - 虚拟文件系统
        - VFS是对不同种类的真实文件系统的抽象
        - Finfo中的读写指针
        - 用C语言模拟面向对象编程
    - 操作系统之上的IOE
        - 把串口抽象成文件
        - 实现gettimeofday
        - 实现NDL的时钟
        - 把按键输入抽象成文件
        - 用fopen()还是open()?
            - fopen()和open()都是用于打开文件的函数，但它们有一些不同之处。
            open()是POSIX标准定义的一个函数，它接收一个文件路径和一组标志作为参数，并返回一个文件描述符，可以用来读取或写入文件。它是一个较低级别的函数，可以更好地控制打开文件时的标志和行为。
            fopen()是标准C库提供的函数，它也接收一个文件路径和一个打开模式字符串作为参数，并返回一个FILE指针。它提供了一些高级的文件读写功能，例如缓冲、格式化输入输出等。
            如果只是简单地打开和读写文件，建议使用open()函数。如果需要更高级的文件读写功能，例如缓冲和格式化输入输出，可以使用fopen()函数。
        - 在NDL中获取屏幕大小
        - 把VGA显存抽象成文件
- 精彩纷呈的应用程序
    - 更丰富的运行时环境
    - 多媒体库
    - 定点算数
        - 比较fixedpt和float
        - 神奇的fixdpt_rconst
        - 实现更多的fixedptc API
        - 如何将浮点变量转换成fixedpt类型
    - Navy作为基础设施
        - 操作系统相关的运行时环境
            - libos
            - libc(Newlib)
            - 一些特殊文件
        - 面向应用程序的库
        - 神奇的LD_PRELOAD
        - Wine, WSL和运行时环境兼容
    - Navy中的应用程序
        - NSlider
            - 运行NSlider
            - 注意ramdisk镜像的大小
            - 运行NSlider(2)
        - MENU(开机菜单)
            - 运行开机菜单
        - NTerm(NJU Terminal)
            - 运行NTerm
            - 实现内建的echo命令
        - Flappy Bird
            - 运行Flappy Bird
            - ”计算机是个抽象层“的应用：移植和测试
        - PAL(仙剑奇侠传)
        - am-kernels
        - FCEUX
        - oslab0
    - 基础设施(3)
        - 自由开关DiffTest模式
        - 快照
    - 展示你的批处理系统

# PA4-虚实交错的魔法：分时多任务
- 多道程序
    - 在内存中可以同时存在多个线程
    - 在满足某些条件下，可以让执行流在这些进程之间切换
    - 术语变更-程序->进程
    - 其实我在骗你
    -为什么需要使用不同的栈空间
- 上下文切换
    - 基本原理
        - 切换栈指针
    - 进程控制块
        - cp指针(context pointer)记录上下文结构位置
        - 进程控制块(PCB, process control block)
- 内核线程
    - 创建内核线程上下文
        - 为什么不叫”内核进程“？
    - 线程/进程调度
        - 任务调度 schedule()
        - 机制和策略解耦
        - 实现上下文切换
            - kcontext()
            - context_kload()
            - schedule()
            - ...
    - 内核线程的参数
        - mips32和riscv32的调用约定
    - 实现上下文切换(2)
    - 保持kcontext()的特性(x86)
- 用户进程
    - 创建用户程序上下文
        - 如果内核线程发生了栈溢出，怎么办？
        - PCB中栈-内核栈
        - 用户区的栈-用户栈
            - _start把栈顶位置真正设置到栈指针寄存器
        - 实现多道程序
             - ucontext()
             - context_uload()
             - _start
        - 一山不能藏二虎
    - 用户进程的参数
        - 阅读ABI手册，理解计算机系统
            - ELF文件格式
            - 对函数调用使用标准调用约定，指定参数如何传递给函数以及返回值如何返回。
            - 对函数使用标准栈帧布局，指定局部变量和函数参数如何存储在内存中。
            - 使用标准异常处理机制，允许程序以一致的方式处理错误和异常。
        - 给用户进程传递参数
            - 为什么少了一个const?
            - 如何在A的执行流中创建用户进程B
            - 如何结束A的执行流
            - 操作系统的内存管理
            - 实现带参数的execve()
        - 运行Busybox
            - 2和error
        - 运行Busybox(2)
- 虚实交错的魔法
    - 程序和内存位置
        - 绝对代码
            - 程序的内存位置是在链接时刻(link time)确定的
        - 可重定位代码
            - 自重定位
            - 运行时重定位
            - 加载时重定位
        - 位置无关代码
            - 程序中所有寻址都是针对程序位置来进行相对寻址操作
            - PIC
            - PIE
            - 实现基于PIE的loader
        - 虚实交错的魔法
            - 程序连接到固定虚拟地址，加载到不同物理的地址，维护好映射关系
            - 处理器和存储器之间的MMU(Memory Management Unit)
            - OS和MMU合作
            - 分段
        - 超越容量的界限
            - 段的粒度太大了
            - 分页
                - 虚拟页 物理页
                - 页表
                - 虚存管理中PIC的好处
                - 虚存管理可以将物理内存分割成多个页面，每个页面可以被不同的进程所共享。如果程序采用了PIC编译方式，那么它的代码段和数据段中的地址都是相对地址，因此不会和其他进程的代码和数据发生地址冲突。这意味着不同的进程可以共享同一份代码和数据，从而提高了内存利用率，节省了内存空间。
                - page table walk
                - 理解分页机制
                - riscv64需要实现三级页表
                - 空指针的真的是”空“吗
            - 状态机视角下的虚存管理
                - M[addr]->M[fvm(addr)]
                - TLB-地址转换的加速
                - 软件管理的TLB
            - 将虚存管理抽象成VME
                - 虚存机制-映射
                - 在分页机制上运行Nanos-lite
                    - 恒等映射(identical mapping)
                - 在分页机制上运行用户进程
            - 支持虚存管理的多道程序
            