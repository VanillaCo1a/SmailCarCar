//OLED¿ØÖÆÓÃº¯Êý
void OLED_Write_Byte(uchar,uchar);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void Init_OLED(void);
void OLED_Clear(void);
void OLED_DrawPoint(uchar, uchar, uchar);
void OLED_Fill(uchar, uchar, uchar, uchar, uchar);
void OLED_ShowChar(uchar, uchar, uchar, uchar);
void OLED_ShowNum(uchar, uchar, uint, uchar, uchar);
void OLED_ShowString(uchar, uchar, uchar *,uchar);	 
void OLED_Set_Pos(uchar, uchar);
void OLED_ShowCHinese(uchar, uchar, uchar);
void OLED_DrawBMP(uchar, uchar, uchar, uchar, uchar *);
void fill_picture(uchar);
void Picture(void);
void OLED_I2C_Write_Command(uchar);
void OLED_I2C_Write_Data(uchar);