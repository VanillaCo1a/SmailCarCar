#include <reg52.h>
#include <intrins.h>
#include "main.h"
#include "typedef.h" //为unsigned char和unsigned int添加别名的头文件
#include "delay.h" //延时函数声明文件
#include "interrupt.h"
#include "ctrl.h"
#include "motor.h"
#include "i2ccommunication.h"
#include "spicommunication.h"
#include "serialcommunication.h"
#include "oled_i2c.h"
//#include "oled_spi.h"
#include "oled_font.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127