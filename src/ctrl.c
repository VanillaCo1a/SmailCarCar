#include "head.h"

sbit infrared_avoidance_left = P3 ^ 4;
sbit infrared_avoidance_right = P3 ^ 5;
sbit infrared_searchline_right = P3 ^ 6;
sbit infrared_searchline_left = P3 ^ 7;

void OLED_Ctrl(void)
{
		OLED_Clear();
		OLED_DrawBMP(0, 0, 127, 7, BMP);
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