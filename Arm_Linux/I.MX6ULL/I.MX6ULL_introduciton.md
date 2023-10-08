# IMX6ULL IO表示形式
- 管脚名字：PAD_BOOT_MODE0, 管脚的复用功能
  IOMUXC_SNVS_SW\_<span style="color: red;">MUX</span>\_CTL_PAD_BOOT_MODE0
  IOMUXC_SNVS_SW\_<span style="color: red;">PAD</span>\_CTL_PAD_BOOT_MODE0

- 在 IMX6ULL 中，每个 IO 引脚有两个相关的寄存器，分别是 IOMUXC_SW_MUX_CTL_PAD 和 IOMUXC_SW_PAD_CTL_PAD。
    
    1. IOMUXC_SW_MUX_CTL_PAD：该寄存器用于控制 IO 引脚的复用功能。每个 IO 引脚可以配置为不同的功能，例如 GPIO、UART、I2C、SPI 等。通过设置 IOMUXC_SW_MUX_CTL_PAD 寄存器的值，可以选择将该引脚设置为特定功能。

    2. IOMUXC_SW_PAD_CTL_PAD：该寄存器用于配置 IO 引脚的电气特性和驱动能力。通过设置 IOMUXC_SW_PAD_CTL_PAD 寄存器的值，可以调整引脚的输入输出电阻、施加上拉或下拉电阻、设置引脚的驱动能力等参数。
- 查看管脚复用的步骤
    1. 打开参考手册32章，IO复用章节
    2. 查找对应的管脚

# I.汇编LED原理分析
- 为什么要学习Cortex-A汇编
    1. 需要用汇编初始化一些SOC外设
    2. 使用汇编初始化DDR（I.MX6U不需要）
    3. 设置sp指针，一般指向DDR，设置好C语言运行时环境
    
- ALPHA开发板LED硬件原理分析
    - STM32 IO 初始化流程
        1. 使能GPIO外设时钟
        2. 设置GPIO复用，将其复用位GPIO
        3. 配置GPIO的电气属性
        4. 使用GPIO，输出高低电平
    - IMUX6ULL IO 初始化
        1. 使能时钟CCGR0~CCGR6这7个寄存器控制着IMUX6ULL所有外设时钟的使能。为了简单CCGR0~CCGR6全部置位0xFFFF，相当于使能所有外设时钟
        2. IO复用，IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03寄存器的3~0bit设置为0101，GPIO1_IO0复用为GPIO
        3. IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03寄存器设置电气属性，包括压摆率，速度，驱动能力，开漏，上下拉等
        4. 配置GPIO功能，设置输出输出。设置GPIO1_GDIR寄存器bit3为1, 设置为输出模式。设置GPIO1_DR寄存器bit3为0输出低电平，为1输出高电平。

- 汇编简介
    - 写驱动最常用的指令是LDR和STR 

- 编写驱动
    - 编译程序
    - 链接起始地址，IMX6LL启动后PC指向的地址，对于IMX6LL，应该指向RAM，RAM分为内部RAM，外部RAM也就是DDR
    内部RAM地址范围0x900000~0x91FFFF
    512MB的DDR范围0x80000000~0x9FFFFFFF
    - 裸机代码的链接起始地址为0x87800000
    - 要使用DDR，必须初始化DDR，bin文件不能直接烧写到SD卡，EMMC，NAND等外置存储中，然后从这些外置存储中启动运行。
    需要添加一个头部，这个头部信息包含了DDR的初始化参数，IMX系列SOC内部boot rom会从外置存储中读取头部信息，然后初始化DDR，并将bin文件拷贝到指定的地方(链接起始地址)
    bin的运行地址一定要和链接起始地址一致，位置无关代码除外
    JLINK将bin文件直接下载到内部RAM可以直接运行

- 烧写bin文件
    - 6ULL支持SD卡，EMMC，NAND，SPI flash等启动
    - 裸机例程烧写到SD卡里
        - 不是拷贝而是将bin文件烧写到SD卡的绝对地址上，而且对于I.MX，不能直接烧写bin文件，必须现在bin文件前面添加头部。imxdownload
    - JLINK
        - 6ULL支持JTAG，因为没有烧写算法，所以无法烧写
        - 但是可以通过JTAG将bin文件下载到内部ram
        - 6ULL的JTAG口竟然和SAI复用，SAI连接WM8960音频DAC
        - 普通开发者在嵌入式linux开发中基本不适用JLINK