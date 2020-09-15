#include <reg52.h>
#include <intrins.h>
#include "typedef.h"
#include "delay.h"
#include "spicommunication.h"

sbit SPI_SCLK = P2^4;	//SPI协议时钟总线
sbit SPI_SDO = P2^5;	//SPI协议信号总线
sbit SPI_CS = P2^2;		//SPI协议从机片选线

/***	主程序写法示例
}		***/

void Init_SPI_Communication(void)
{
		SPI_CS = 1;
		_nop_();
		SPI_SDO = 1;
		_nop_();
		SPI_SCLK = 1;
		_nop_();
}
void SPI_Communication_Write(uchar word)
{
		uchar temp, i;
	
		SPI_CS = 0;		///拉低片选
		_nop_();
		SPI_SCLK = 0;
		_nop_();
		for (i=0; i<8; i++)
		{
				temp = word;
				SPI_SDO = (temp >> (7-i))&1;
				_nop_();
				SPI_SCLK = 1;
				_nop_();
				SPI_SCLK = 0;	//拉低SCL, 表示该时钟周期结束
				_nop_();
		}
		SPI_SDO = 1;	//释放数据总线
		_nop_();
		SPI_CS = 1;		//拉高片选
}