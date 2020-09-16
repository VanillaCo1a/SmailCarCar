#include <reg52.h>
#include <intrins.h>
#include "typedef.h" //为unsigned char和unsigned int添加别名的头文件
#include "delay.h" //延时函数声明文件
#include "interrupt.h"
#include "motor.h"
#include "serialcommunication.h"
#include "spicommunication.h"
#include "oled.h"
#include "main.h"

sbit infrared_avoidance_left = P3 ^ 4;
sbit infrared_avoidance_right = P3 ^ 5;
sbit infrared_searchline_right = P3 ^ 6;
sbit infrared_searchline_left = P3 ^ 7;
//sbit ultrasound_follow = ;

char flag_mode = 0, word_mode;

void Mode_Ctrl(char*, uchar*, char*, uchar*, char, uchar, char*);
void Mode_Bluetooth_Ctrl(char*, uchar*, uchar, char*);
void Mode_Infrared_Avoidance(char*, uchar*, uchar, char*);
void Mode_Infrared_Searchline(char*, uchar*, uchar, char*);
void Mode_DJ(char);
void main()
{uchar t;
    char flag_bluetooth = 0, flag_time = 0, flag_time_s = 0, priority[5] = { HIGH_PRIORITY, MEDIUM_PRIORITY, NONE_PRIORITY };
		uchar word_serial_transmit, word_serial_receive, word_bluetooth;

		Init_OLED();			//初始化OLED  
    Init_Serial_Communication();
    Init_Interrupt();
		
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
    while (1) {
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
        Serial_Communication(&word_serial_transmit, &word_serial_receive);
        Mode_Ctrl(&flag_mode, &word_mode, &flag_bluetooth, &word_bluetooth, flag_time_s, word_serial_receive, priority);
	}
}

void Mode_Ctrl(char* flag, uchar* word, char* flag_bluetooth, uchar* word_bluetooth, char flag_time_s, uchar word_serial_receive, char* priority)
{
    if (*flag == 1) {
        *flag = 0;
        if (word_serial_receive >= '0' && word_serial_receive <= '9') {
            *word = word_serial_receive;
						switch (*word) {
						case '1':
								priority[AVOIDANCE] = NONE_PRIORITY;
								priority[SEARCHLINE] = NONE_PRIORITY;
								priority[BLUETOOTH] = HIGH_PRIORITY;
								break;
						case '2':
								priority[AVOIDANCE] = HIGH_PRIORITY;
								priority[SEARCHLINE] = MEDIUM_PRIORITY;
								priority[BLUETOOTH] = LOW_PRIORITY;
								break;
						case '3':
								priority[AVOIDANCE] = HIGH_PRIORITY;
								priority[SEARCHLINE] = MEDIUM_PRIORITY;
								priority[BLUETOOTH] = NONE_PRIORITY;
								break;
						case '4':
								priority[AVOIDANCE] = NONE_PRIORITY;
								priority[SEARCHLINE] = NONE_PRIORITY;
								priority[BLUETOOTH] = NONE_PRIORITY;
								break;
						}
        } else {
            *flag_bluetooth = 1;
        }
    }
    if (priority[AVOIDANCE] == HIGH_PRIORITY)
    {
        Mode_Infrared_Avoidance(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
    }
    else if (priority[SEARCHLINE] == HIGH_PRIORITY)
    {
        Mode_Infrared_Searchline(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
    }
    else if (priority[BLUETOOTH] == HIGH_PRIORITY)
    {
        Mode_Bluetooth_Ctrl(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
    }
    else
    {
        Mode_DJ(flag_time_s);
    }
}

void Mode_Infrared_Avoidance(char* flag_bluetooth, uchar* word_bluetooth, uchar word_serial_receive, char* priority)
{
		if (infrared_avoidance_left == NON_OBSTACLE && infrared_avoidance_right == OBSTACLE) {
				Car_Left();
		} else if (infrared_avoidance_left == OBSTACLE && infrared_avoidance_right == NON_OBSTACLE) {
				Car_Right();
		} else if (infrared_avoidance_left == NON_OBSTACLE && infrared_avoidance_right == NON_OBSTACLE) {
				if (priority[AVOIDANCE] - 1 == priority[SEARCHLINE]) {
						Mode_Infrared_Searchline(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
				} else if (priority[AVOIDANCE] - 1 == priority[BLUETOOTH]) {
						Mode_Bluetooth_Ctrl(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
				} else {
						Car_Forward();
				}
		} else {
				Car_Stop();
		}
}
void Mode_Infrared_Searchline(char* flag_bluetooth, uchar* word_bluetooth, uchar word_serial_receive, char* priority)
{
		if (infrared_searchline_left == BLACK && infrared_searchline_right == NOT_BLACK) {
				Car_Left();
		} else if (infrared_searchline_left == NOT_BLACK && infrared_searchline_right == BLACK) {
				Car_Right();
		} else if (infrared_searchline_left == NOT_BLACK && infrared_searchline_right == NOT_BLACK) {
				if (priority[SEARCHLINE] - 1 == priority[AVOIDANCE]) {
						Mode_Infrared_Avoidance(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
				} else if (priority[SEARCHLINE] - 1 == priority[BLUETOOTH]) {
						Mode_Bluetooth_Ctrl(flag_bluetooth, word_bluetooth, word_serial_receive, priority);
				} else {
						Car_Forward();
				}
		} else {
				Car_Stop();
		}
}
void Mode_Bluetooth_Ctrl(char* flag, uchar* word, uchar word_serial_receive, char* priority)
{
		if (*flag == 1) {
				*flag = 0;
				*word = word_serial_receive;
				}
		switch (*word) {
		case 'A':
				if (priority[BLUETOOTH] - 1 == priority[SEARCHLINE]) {
						Mode_Infrared_Avoidance(flag, word, word_serial_receive, priority);
				} else if (priority[BLUETOOTH] - 1 == priority[AVOIDANCE]) {
						Mode_Infrared_Searchline(flag, word, word_serial_receive, priority);
				} else {
						Car_Forward();
				}
				break;
		case 'B':
				Car_Back();
				break;
		case 'C':
				Car_Left();
				break;
		case 'D':
				Car_Right();
				break;
		case 'E':
				if (priority[BLUETOOTH] - 1 == priority[SEARCHLINE]) {
						Mode_Infrared_Avoidance(flag, word, word_serial_receive, priority);
				} else if (priority[BLUETOOTH] - 1 == priority[AVOIDANCE]) {
						Mode_Infrared_Searchline(flag, word, word_serial_receive, priority);
				} else {
						Car_Forward_Lower();
				}
				break;
		case 'F':
				if (priority[BLUETOOTH] - 1 == priority[SEARCHLINE]) {
						Mode_Infrared_Avoidance(flag, word, word_serial_receive, priority);
				} else if (priority[BLUETOOTH] - 1 == priority[AVOIDANCE]) {
						Mode_Infrared_Searchline(flag, word, word_serial_receive, priority);
				} else {
						Car_Forward_Faster();
				}
				break;
		case 'Z':
				Car_Stop();
				break;
}
}
void Mode_DJ(char flag_time_s)
{
		if (flag_time_s % 2 == 0)
		{
				if (flag_time_s % 4 == 0)
				{
						Car_Left();
				}
				else 
				{
						Car_Right();
				}
		}
}