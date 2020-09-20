#include "head.h"

#define OLED_MODE_CMD  0	//д����
#define OLED_MODE_DATA 1	//д����

//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64					  
//-----------------OLED�˿ڶ���----------------  					   


sbit I2C_SCL = P2^4;
sbit I2C_SDA = P2^5;

/**********************************************
// I2C Write Command
**********************************************/
void OLED_I2C_Write_Command(uchar cmd)
{
		I2C_Communication_Start();
		I2C_Communication_Write(0x78);
		I2C_Communication_Wait();
		I2C_Communication_Write(0x00);
		I2C_Communication_Wait();
		I2C_Communication_Write(cmd);
		I2C_Communication_Wait();	
		I2C_Communication_Stop();
}
/**********************************************
// I2C Write Data
**********************************************/
void OLED_I2C_Write_Data(uchar word)
{
		I2C_Communication_Start();
		I2C_Communication_Write(0x78);
		I2C_Communication_Wait();
		I2C_Communication_Write(0x40);
		I2C_Communication_Wait();
		I2C_Communication_Write(word);
		I2C_Communication_Wait();	
		I2C_Communication_Stop();
}
void OLED_Write_Byte(uchar word, uchar mode)
{
	if(mode)
	{
			OLED_I2C_Write_Data(word);
	}
	else {
			OLED_I2C_Write_Command(word);
		}
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(uchar fill_Data)
{
	uchar m,n;
	for(m=0;m<8;m++)
	{
		OLED_Write_Byte(0xb0+m,0);		//page0-page1
		OLED_Write_Byte(0x02,0);		//low column start address
		OLED_Write_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_Write_Byte(fill_Data,1);
			}
	}
}


//��������
	void OLED_Set_Pos(uchar x, uchar y) 
{ 	OLED_Write_Byte(0xb0+y,OLED_MODE_CMD);
	OLED_Write_Byte((((x+2)&0xf0)>>4)|0x10,OLED_MODE_CMD);
	OLED_Write_Byte(((x+2)&0x0f),OLED_MODE_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_Write_Byte(0X8D,OLED_MODE_CMD);  //SET DCDC����
	OLED_Write_Byte(0X14,OLED_MODE_CMD);  //DCDC ON
	OLED_Write_Byte(0XAF,OLED_MODE_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_Write_Byte(0X8D,OLED_MODE_CMD);  //SET DCDC����
	OLED_Write_Byte(0X10,OLED_MODE_CMD);  //DCDC OFF
	OLED_Write_Byte(0XAE,OLED_MODE_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uchar i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Byte (0xb0+i,OLED_MODE_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Byte (0x02,OLED_MODE_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Byte (0x10,OLED_MODE_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Byte(0,OLED_MODE_DATA); 
	} //������ʾ
}
void OLED_On(void)  
{  
	uchar i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Byte (0xb0+i,OLED_MODE_CMD);    //����ҳ��ַ��0~7��
		OLED_Write_Byte (0x02,OLED_MODE_CMD);      //������ʾλ�á��е͵�ַ
		OLED_Write_Byte (0x10,OLED_MODE_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_Write_Byte(1,OLED_MODE_DATA); 
	} //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar Char_Size)
{      	
	uchar c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_Write_Byte(F8X16[c*16+i],OLED_MODE_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_Write_Byte(F8X16[c*16+i+8],OLED_MODE_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_Write_Byte(F6x8[c][i],OLED_MODE_DATA);
				
			}
}
//m^n����
uint oled_pow(uchar m,uchar n)
{
	uint result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uchar x,uchar y,uchar *chr,uchar Char_Size)
{
	uchar j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(uchar x,uchar y,uchar no)
{      			    
	uchar t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_Write_Byte(Hzk[2*no][t],OLED_MODE_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_Write_Byte(Hzk[2*no+1][t],OLED_MODE_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(uchar x0, uchar y0,uchar x1, uchar y1,uchar BMP[])
{ 	
 uint j=0;
 uchar x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_Write_Byte(BMP[j++],OLED_MODE_DATA);	    	
	    }
	}
} 

//��ʼ��SSD1306					    
void Init_OLED(void)
{ 	
 
OLED_Write_Byte(0xAE,OLED_MODE_CMD);//--display off
	OLED_Write_Byte(0x00,OLED_MODE_CMD);//---set low column address
	OLED_Write_Byte(0x10,OLED_MODE_CMD);//---set high column address
	OLED_Write_Byte(0x40,OLED_MODE_CMD);//--set start line address  
	OLED_Write_Byte(0xB0,OLED_MODE_CMD);//--set page address
	OLED_Write_Byte(0x81,OLED_MODE_CMD); // contract control
	OLED_Write_Byte(0xFF,OLED_MODE_CMD);//--128   
	OLED_Write_Byte(0xA1,OLED_MODE_CMD);//set segment remap 
	OLED_Write_Byte(0xA6,OLED_MODE_CMD);//--normal / reverse
	OLED_Write_Byte(0xA8,OLED_MODE_CMD);//--set multiplex ratio(1 to 64)
	OLED_Write_Byte(0x3F,OLED_MODE_CMD);//--1/32 duty
	OLED_Write_Byte(0xC8,OLED_MODE_CMD);//Com scan direction
	OLED_Write_Byte(0xD3,OLED_MODE_CMD);//-set display offset
	OLED_Write_Byte(0x00,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0xD5,OLED_MODE_CMD);//set osc division
	OLED_Write_Byte(0x80,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0xD8,OLED_MODE_CMD);//set area color mode off
	OLED_Write_Byte(0x05,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0xD9,OLED_MODE_CMD);//Set Pre-Charge Period
	OLED_Write_Byte(0xF1,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0xDA,OLED_MODE_CMD);//set com pin configuartion
	OLED_Write_Byte(0x12,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0xDB,OLED_MODE_CMD);//set Vcomh
	OLED_Write_Byte(0x30,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0x8D,OLED_MODE_CMD);//set charge pump enable
	OLED_Write_Byte(0x14,OLED_MODE_CMD);//
	
	OLED_Write_Byte(0xAF,OLED_MODE_CMD);//--turn on oled panel
}