#include "head.h"

sbit I2C_SCL = P2^4;
sbit I2C_SDA = P2^5;

/***	主程序写法示例
void main(void)
{
	uchar word_I2C;

	Init_I2C1_Communication();
	I2C1_Communication_Start();
	I2C1_Communication_Write(0XA0);
	I2C1_Communication_Wait();
	I2C1_Communication_Write(123);
	I2C1_Communication_Wait();
	...
	I2C1_Communication_Stop();
	delay_ms(5);	// AT24C02的写入周期是5ms,延时一定要大于或者等于5ms 
	
	
	I2C1_Communication_Start();
	I2C1_Communication_Write(0XA1);
	I2C1_Communication_Wait();
	word_I2C = I2C1_Communication_Read();
	I2C1_Communication_Wait();
	...
	I2C1_Communication_Stop();

	while (1);
}		***/


void Init_I2C_Communication(void)
{
		I2C_SDA = 1;
		_nop_();
		I2C_SCL = 1;
		_nop_();
}
void I2C_Communication_Start(void)
{
		I2C_SDA = 1;
		_nop_();
		I2C_SCL = 1;
		delay_5us();
		I2C_SDA = 0;
		delay_5us();
}
void I2C_Communication_Stop(void)
{
		I2C_SDA = 0;
		_nop_();
		I2C_SCL = 1;
		delay_5us();
		I2C_SDA = 1;
		delay_5us();
}
void I2C_Communication_Write(uchar word)
{
		uchar temp, i;
	
		I2C_SCL = 0;
		_nop_();
		for (i=0; i<8; i++)
		{
				temp = word;
				I2C_SDA = (temp >> (7-i))&1;
				_nop_();
				I2C_SCL = 1;
				delay_5us();
				I2C_SCL = 0;	//拉低I2C_SCL, 表示该时钟周期结束
				_nop_();
		}
		I2C_SDA = 1;	//释放数据总线
		_nop_();
}
uchar I2C_Communication_Read(void)
{
		uchar word, i;
	
		I2C_SCL = 0;
		_nop_();
		I2C_SDA = 1;
		_nop_();
		for (i=0; i<8; i++)
		{
				I2C_SCL = 1;
				delay_5us();
				word = (word << 1) | I2C_SDA;
				I2C_SCL = 0;
				_nop_();
		}
		I2C_SDA = 1;	//释放数据总线
		return word;
}
void I2C_Communication_Wait(void)
{
		uchar i = 0;
	
		I2C_SCL = 0;
		_nop_();
		I2C_SDA = 1;
		_nop_();
		I2C_SCL = 1;
		_nop_();
		while (I2C_SDA == 1 && i <= 5) i++;
		_nop_();
		I2C_SCL = 0;
		_nop_();
}
void I2C_Communication_Respond(void)
{		
		I2C_SCL = 0;
		_nop_();
		I2C_SDA = 0;
		_nop_();
		I2C_SCL = 1;
		delay_5us();
		I2C_SCL = 0;
		_nop_();
}