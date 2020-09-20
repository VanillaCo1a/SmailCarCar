#include "head.h"

//sbit ultrasound_follow = ;
char flag_mode = 0, word_mode, flag_time = 0, flag_time_s = 0;

void main()
{
    char flag_bluetooth = 0, priority[5] = { HIGH_PRIORITY, MEDIUM_PRIORITY, NONE_PRIORITY };
		uchar word_serial_transmit, word_serial_receive, word_bluetooth;

		Init_OLED();			//初始化OLED
    Init_Serial_Communication();		//初始化串口通信(和一个中断计时器)
    Init_Interrupt();				//初始化系统中断
		
		OLED_Ctrl();	//更新一次OLED屏幕，后续不再变更(太占资源辣)
    while (1) {
			Time_Calculation();	//根据内部中断自上电起计算时间，flag_time等变量广泛用于其他函数
			
       Serial_Communication(&word_serial_transmit, &word_serial_receive);	//串口通信，用于检测是否收到蓝牙命令
			Mode_Ctrl(&flag_mode, &word_mode, &flag_bluetooth, &word_bluetooth, flag_time_s, word_serial_receive, priority);	//模式控制，用于小车各模式的切换，和各模块对于电机的综合控制
	}
}

void Time_Calculation(void)
{
	       if (flag_inter0 >= 5) {
	            flag_inter0 = 0;
            flag_time++;
            if (flag_time == 80)
            {
                flag_time = 0;
                flag_time_s ++;
                if (flag_time_s == 100)
                {
                    flag_time_s = 0;
                }  
            }
        }

}