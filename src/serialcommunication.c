#include "head.h"

/***	主程序写法示例
void main(void)
{
	char word_serial_transmit, word_serial_receive;
	
	Init_Serial_Communication();
	Init_Interrupt();
	while()
	{
		Serial_Communication(&word_serial_transmit, &word_serial_receive);
	}
}		***/

void Init_Serial_Communication(void)
{
	SCON = 0X40 + (SCON & 0X3F); //设置串口工作模式
	SM2 = 0;					 //多机通信控制位
	REN = 1;					 //串行允许接收位
	PCON = 0X00 + (PCON & 0X7F); //设置波特率倍增位
	Init_Serial_Interrupt(1);
}
void Serial_Communication(char *word_transmit, char *word_receive)
{
	if (flag_serial_receive == 1) //表示串口通信中有数据等待接收
	{
		*word_receive = SBUF;
		flag_serial_receive = 0;
		//...
		flag_mode = 1;
		flag_serial_transmit = 1;
		*word_transmit = *word_receive;
	}
	if (flag_serial_transmit == 1) //表示串口通信中有数据需要传输
	{
		flag_serial_transmit = 2;
		SBUF = *word_transmit;
		while(flag_serial_transmit);
		//...
	}
}