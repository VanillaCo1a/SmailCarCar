#include "head.h"

/** 在静态整型变量time_ruptx中计算计时器x中需要装填入两个八位寄存器中的初值
    在静态整型变量time_THx_Init, time_TLx_Init中计算计时器x中第一次装填入两个八位寄存器的初值(经修正)
    在静态整型变量time_THx, time_TLx中计算计时器x中每次重装时装填入两个八位寄存器的初值(经修正)    **/
const uint time_rupt0 = 65536 - TIME_DELAY_RUPT0 / (12 / CRYSTAL_FREQUENCY);
const uint time_TH0_Init = ((uint)(65536 - TIME_DELAY_RUPT0 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT0_INIT)) / 256;
const uint time_TL0_Init = ((uint)(65536 - TIME_DELAY_RUPT0 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT0_INIT)) % 256;
const uint time_TH0 = ((uint)(65536 - TIME_DELAY_RUPT0 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT0)) / 256;
const uint time_TL0 = ((uint)(65536 - TIME_DELAY_RUPT0 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT0)) % 256;
const uint time_rupt1 = 65536 - TIME_DELAY_RUPT1 / (12 / CRYSTAL_FREQUENCY);
const uint time_TH1_Init = ((uint)(65536 - TIME_DELAY_RUPT1 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT1_INIT)) / 256;
const uint time_TL1_Init = ((uint)(65536 - TIME_DELAY_RUPT1 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT1_INIT)) % 256;
const uint time_TH1 = ((uint)(65536 - TIME_DELAY_RUPT1 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT1)) / 256;
const uint time_TL1 = ((uint)(65536 - TIME_DELAY_RUPT1 / (12 / CRYSTAL_FREQUENCY) + TIME_DELAY_STEP_RUPT1)) % 256;
//软件标志位, 用于记录中断是否触发
char flag_inter0 = 0, flag_inter1 = 0, flag_exter0 = 0, flag_exter1 = 0, flag_serial_receive = 0, flag_serial_transmit = 0;

void Init_Interrupt(void) //全体中断的初始化函数
{
    //Init_Exter_Interrupt0();
    //Init_Exter_Interrupt1();
    Init_Inter_Interrupt0();
    //Init_Inter_Interrupt1();
    //Init_Serial_Interrupt(1);
    EA = 1; //开启全局中断
    _nop_();
}
void Init_Exter_Interrupt0(void) //外部中断0的初始化函数
{
    EX0 = 1; //开启外部中断0
    PX0 = 0; //设置外部中断0的优先级。0为低优先级, 1为高优先级。单片机上电时寄存器默认为低电位, 即优先级默认为低
    IT0 = 0; //设置外部中断0的触发模式。0为低电平触发, 1为下降沿触发。默认为低电平触发
    _nop_();
}
void Init_Exter_Interrupt1(void) //外部中断1的初始化函数
{
    EX1 = 1; //开启外部中断1
    PX1 = 0; //设置外部中断1的优先级。0为低优先级, 1为高优先级
    IT1 = 1; //设置外部中断1的触发模式。0为低电平触发, 1为下降沿触发
    _nop_();
}
void Init_Inter_Interrupt0(void) //内部中断0的初始化函数
{
    ET0 = 1;                     //开启内部中断0
    PT0 = 0;                     //设置内部中断0的优先级。0为低优先级, 1为高优先级。单片机上电时寄存器默认为低电位, 即默认优先级为低
    TMOD = 0X01 + (TMOD & 0XF0); //在计数器工作方式寄存器TMOD中设置定时器0的工作方式
    TH0 = time_TH0_Init;         //将由TIME_DELAY_RUPT0(us)计算得到的初值取前八位(二进制下)装入高位寄存器
    TL0 = time_TL0_Init;         //将由TIME_DELAY_RUPT0(us)计算得到的初值取后八位(二进制下)装入低位寄存器
    TR0 = 1;                     //启动内部中断0的定时器0
    _nop_();
}
void Init_Inter_Interrupt1(void) //内部中断1的初始化函数
{
    ET1 = 1;                     //开启内部中断1
    PT1 = 0;                     //设置内部中断1的优先级。0为低优先级, 1为高优先级
    TMOD = 0X10 + (TMOD & 0X0F); //在计数器工作方式寄存器TMOD中设置定时器1的工作方式
    TH1 = time_TH1_Init;         //将由TIME_DELAY_RUPT1(us)计算得到的初值取前八位(二进制下)装入高位寄存器
    TL1 = time_TL1_Init;         //将由TIME_DELAY_RUPT1(us)计算得到的初值取后八位(二进制下)装入低位寄存器
    TR1 = 1;                     //启动内部中断1的定时器1
    _nop_();
}
void Init_Serial_Interrupt(char i) //使用定时器0的串口中断的初始化函数
{
    ES = 1; //开启串口中断
    PS = 0; //设置串口中断的优先级。0为低优先级, 1为高优先级
    if (i)
    {
        if ((SCON & 0XC0) == 0X40 || (SCON & 0XC0) == 0XC0)
        {
            TMOD = 0X20 + (TMOD & 0X0F); //在计数器工作方式寄存器TMOD中设置定时器1的工作方式
            TH1 = 0XFD;
            TL1 = 0XFD;
            TR1 = 1; //启动定时器1
        }
        _nop_();
    }
    else
    {
        if ((SCON & 0XC0) == 0X40 || (SCON & 0XC0) == 0XC0)
        {
            TMOD = 0X02 + (TMOD & 0XF0); //在计数器工作方式寄存器TMOD中设置定时器0的工作方式
            TH0 = 0XFD;
            TL0 = 0XFD;
            TR0 = 1; //启动定时器0
        }
    }
    _nop_();
}

//中断函数不需要前加声明, 函数原型的写法与C语言标准函数原型一致,
//但需要在末尾加上"interrupt X", 其中X为该中断的默认优先级。单片机依靠X来辨别中断函数所对应的中断
//51(2)单片机的默认优先级为INT0>T0>INT1>T1>串口中断>T2(如果有)
//因此外部中断0的优先级为0, 外部中断1的优先级为2, 内部中断0的优先级为1, 内部中断1的优先级为3
//52单片机特有的内部中断2寄存器设置较为不同, 不在此处进行设置
void exter0(void) interrupt 0 //外部中断0的中断函数, 当外部中断0的中断请求被响应时, 单片机进入函数内部执行语句
{
    flag_exter0 = 1; //外部中断0触发时, 记软件标志位为1
    _nop_();
}
void exter1(void) interrupt 2 //外部中断1的中断函数
{
    flag_exter1++;    //外部中断1触发时, 记软件标志位为1
    delay_100us(100); //下降沿模式下的消抖处理
}
void inter0(void) interrupt 1 //内部中断0的中断函数, 当内部中断0的中断请求被响应时, 单片机进入函数内部执行语句
{
    ET0 = 0;        //进入中断函数, 先关闭中断位, 取消对中断的响应, 1个机器周期
    TR0 = 0;        //停止加1计数器的累加, 1个机器周期
    TH0 = time_TH0; //进入中断函数, 重新为高位寄存器装填初值
    TL0 = time_TL0; //进入中断函数, 重新为低位寄存器装填初值
    TR0 = 1;        //开始下一次中断触发的累加
    flag_inter0++;  //内部中断0触发时, 软件标志位累加

    _nop_(); //当中断函数内的步骤与主函数内的步骤数奇偶性不同时, 补充此语句
    ET0 = 1; //中断函数结束, 开启中断位
}
void inter1(void) interrupt 3 //内部中断1的中断函数
{
    ET1 = 0;        //进入中断函数, 先关闭中断位, 取消对中断的响应, 1个机器周期
    TR1 = 0;        //停止加1计数器的累加, 1个机器周期
    TH1 = time_TH1; //进入中断函数, 重新为高位寄存器装填初值, 2个机器周期
    TL1 = time_TL1; //进入中断函数, 重新为低位寄存器装填初值, 2个机器周期
    TR1 = 1;        //开始下一次中断触发的累加
    flag_inter1++;  //内部中断1触发时, 软件标志位累加

    _nop_(); //当中断函数内的步骤与主函数内的步骤数奇偶性不同时, 补充此语句
    ET1 = 1; //中断函数结束, 开启中断位
}
void serial(void) interrupt 4 //串口中断的中断函数
{
    ES = 0;      //进入中断函数, 先关闭中断位, 取消对中断的响应, 1个机器周期
    if (TI == 1) //2个机器周期
    {
        TI = 0;                   //1个机器周期
        flag_serial_transmit = 0; //2个机器周期
        _nop_();                  //当中断函数内的步骤与主函数内的步骤数奇偶性不同时, 补充此语句
    }
    if (RI == 1) //2个机器周期
    {
        RI = 0;                  //1个机器周期
        flag_serial_receive = 1; //2个机器周期
        _nop_();                 //当中断函数内的步骤与主函数内的步骤数奇偶性不同时, 补充此语句
    }
    ES = 1; //中断函数结束, 开启中断位, 1个机器周期
}