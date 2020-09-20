#include "head.h"

void delay_5us(void)	//延时5个机器周期的函数, 5~5.5us
{
	_nop_();
}
void delay_100us(uchar nhus)
{
	uchar x, y;
	for (x = nhus; x > 0; x--)
	{
		for (y = 7; y > 0; y--)
			;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}
void delay_ms(uchar nms)
{
	uchar x, y;
	for (x = nms; x > 0; x--)
	{
		for (y = 90; y > 0; y--)
			;
		_nop_();
		_nop_();
		_nop_();
	}
}
void delay_100ms(uchar nhms)
{
	uchar x, y, z;
	for (x = nhms; x > 0; x--)
	{
		for (y = 85; y > 0; y--)
		{
			for (z = 82; z > 0; z--)
			{
				_nop_();
				_nop_();
				_nop_();
			}
		}
		_nop_();
		_nop_();
	}
}
void delay_s(uchar ns)
{
	uchar x, y, z;
	for (x = 2 * ns; x > 0; x--)
		for (y = 189; y > 0; y--)
			for (z = 242; z > 0; z--)
				;
}