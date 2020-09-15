/** 修改TIME_DELAY_RUPTx的值以表示计时器x中每次触发中断间隔多少微秒
    TIME_DELAY_STEP_RUPTx_INIT与TIME_DELAY_STEP_RUPTx为修正值*, 可通过Keil仿真确定  
    在STEP_RUPTx_INIT中填入修正步数使得第一次中断正好在上电的TIME_DELAY_RUPTx(us)后触发
    在STEP_RUPTx中填入修正步数使得每次中断正好在上一次触发的TIME_DELAY_RUPTx(us)后触发
        *修正值: 1.由于单片机首次上电时需要消耗一段时间进行初始化(耗时约500(us), 根据程序的复杂程度而定), 
        因此使用STEP_RUPTx_INIT进行修正, 使单片机上电TIME_DELAY_RUPTx(us)后恰好触发第一次中断(TIME_DELAY_RUPTx大于1000us时可用)
        2.由于单片机每次触发中断后需要消耗一段时间进入中断函数并重装初值, 并且当单片机在执行主函数中需要多个机器周期的C语句时会延后进入中断函数, 
        因此使用STEP_RUPTx进行修正, 使单片机每次触发中断都恰好间隔TIME_DELAY_RUPTx(us)
        具体的数值根据实际情况结合Keil仿真进行调整。另一方面, 也可以通过计算得出: 
        默认情况下进入中断函数需要4个机器周期, 当中断函数内存在函数的调用等情况时进入中断函数的耗时会增加。
        在本代码中, 在计数器重新装填好并再次开启前有4个C语句, 需要6个机器周期, 因此设置TIME_DELAY_RUPTx的值为4+6=10        **/
#define CRYSTAL_FREQUENCY 11.0592           //51(2)单片机晶振频率, 取决于硬件电路, 视情况修改为11.0592或12.0000
#define TIME_DELAY_RUPT0 625                //计时器0每次触发中断之间间隔的微秒数
#define TIME_DELAY_STEP_RUPT0_INIT 0        //计时器0第一次装填中断值时的修正步数
#define TIME_DELAY_STEP_RUPT0 10            //计时器0每次进入中断函数并重装初值所消耗的步数
#define TIME_DELAY_RUPT1 5000               //计时器1每次触发中断之间间隔的微秒数
#define TIME_DELAY_STEP_RUPT1_INIT 1224-50  //计时器1第一次装填中断值时的修正步数
#define TIME_DELAY_STEP_RUPT1 10            //计时器1每次进入中断函数并重装初值所消耗的步数

//软件标志位, 用于记录中断是否触发
extern char flag_inter0, flag_inter1, flag_exter0, flag_exter1, flag_serial_receive, flag_serial_transmit;

void Init_Interrupt(void);          //全局中断初始化函数的函数声明
void Init_Exter_Interrupt0(void);   //外部中断0初始化函数的函数声明
void Init_Exter_Interrupt1(void);   //外部中断1初始化函数的函数声明
void Init_Inter_Interrupt0(void);   //内部中断0初始化函数的函数声明
void Init_Inter_Interrupt1(void);   //内部中断1初始化函数的函数声明
void Init_Serial_Interrupt(char);	//串口中断初始化函数的函数声明