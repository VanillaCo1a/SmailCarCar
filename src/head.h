#include <reg52.h>
#include <intrins.h>
#include "main.h"
#include "typedef.h" //Ϊunsigned char��unsigned int��ӱ�����ͷ�ļ�
#include "delay.h" //��ʱ���������ļ�
#include "interrupt.h"
#include "ctrl.h"
#include "motor.h"
#include "i2ccommunication.h"
#include "spicommunication.h"
#include "serialcommunication.h"
#include "oled_i2c.h"
//#include "oled_spi.h"
#include "oled_font.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127