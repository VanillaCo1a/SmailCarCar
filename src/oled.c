#include <reg52.h>
#include <intrins.h>
#include "typedef.h"
#include "delay.h"
#include "oled.h"
#include "oledfont.h"
#include "spicommunication.h"
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

#define OLED_MODE_CMD  0	//写命令
#define OLED_MODE_DATA 1	//写数据

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

sbit OLED_SCLK = P2^4;//D0（SCLK）时钟 
sbit OLED_SDO = P2^5;//D1（MOSI）数据
sbit OLED_RST = P2^6;//复位
sbit OLED_DC = P2^7;//数据/命令控制
sbit OLED_CS=P2^2; //片选

void OLED_Write(uchar word,uchar mode)
{
	if(mode)
	  OLED_DC = 1;
	else 
	  OLED_DC = 0;		  
	SPI_Communication_Write(word); 
	OLED_DC = 1;   	  
}

//初始化SSD1306					    
void Init_OLED(void)
{
  OLED_RST = 1;
	delay_ms(100);
	OLED_RST = 0;
	delay_ms(100);
	OLED_RST = 1; 

	OLED_Write(0xAE,OLED_MODE_CMD);//--turn off oled panel
	OLED_Write(0x00,OLED_MODE_CMD);//---set low column address
	OLED_Write(0x10,OLED_MODE_CMD);//---set high column address
	OLED_Write(0x40,OLED_MODE_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write(0x81,OLED_MODE_CMD);//--set contrast control register
	OLED_Write(0xCF,OLED_MODE_CMD); // Set SEG Output Current Brightness
	OLED_Write(0xA1,OLED_MODE_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_Write(0xC8,OLED_MODE_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_Write(0xA6,OLED_MODE_CMD);//--set normal display
	OLED_Write(0xA8,OLED_MODE_CMD);//--set multiplex ratio(1 to 64)
	OLED_Write(0x3f,OLED_MODE_CMD);//--1/64 duty
	OLED_Write(0xD3,OLED_MODE_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write(0x00,OLED_MODE_CMD);//-not offset
	OLED_Write(0xd5,OLED_MODE_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_Write(0x80,OLED_MODE_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write(0xD9,OLED_MODE_CMD);//--set pre-charge period
	OLED_Write(0xF1,OLED_MODE_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write(0xDA,OLED_MODE_CMD);//--set com pins hardware configuration
	OLED_Write(0x12,OLED_MODE_CMD);
	OLED_Write(0xDB,OLED_MODE_CMD);//--set vcomh
	OLED_Write(0x40,OLED_MODE_CMD);//Set VCOM Deselect Level
	OLED_Write(0x20,OLED_MODE_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write(0x02,OLED_MODE_CMD);//
	OLED_Write(0x8D,OLED_MODE_CMD);//--set Charge Pump enable/disable
	OLED_Write(0x14,OLED_MODE_CMD);//--set(0x10) disable
	OLED_Write(0xA4,OLED_MODE_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_Write(0xA6,OLED_MODE_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_Write(0xAF,OLED_MODE_CMD);//--turn on oled panel
	OLED_Write(0xAF,OLED_MODE_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_Write(0xb0+y,OLED_MODE_CMD);
	OLED_Write(((x&0xf0)>>4)|0x10,OLED_MODE_CMD);
	OLED_Write((x&0x0f)|0x01,OLED_MODE_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_Write(0X8D,OLED_MODE_CMD);  //SET DCDC命令
	OLED_Write(0X14,OLED_MODE_CMD);  //DCDC ON
	OLED_Write(0XAF,OLED_MODE_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_Write(0X8D,OLED_MODE_CMD);  //SET DCDC命令
	OLED_Write(0X10,OLED_MODE_CMD);  //DCDC OFF
	OLED_Write(0XAE,OLED_MODE_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uchar i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write (0xb0+i,OLED_MODE_CMD);    //设置页地址（0~7）
		OLED_Write (0x00,OLED_MODE_CMD);      //设置显示位置—列低地址
		OLED_Write (0x10,OLED_MODE_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_Write(0,OLED_MODE_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uchar x,uchar y,uchar chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_Write(F8X16[c*16+i],OLED_MODE_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_Write(F8X16[c*16+i+8],OLED_MODE_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_Write(F6x8[c][i],OLED_MODE_DATA);
				
			}
}
//m^n函数
uint oled_pow(uchar m,uchar n)
{
	uint result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uchar x,uchar y,uint num,uchar len,uchar size2)
{         	
	uchar t,temp;
	uchar enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(uchar x,uchar y,uchar *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(uchar x,uchar y,uchar no)
{      			    
	uchar t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_Write(Hzk[2*no][t],OLED_MODE_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_Write(Hzk[2*no+1][t],OLED_MODE_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_Write(BMP[j++],OLED_MODE_DATA);	    	
	    }
	}
}