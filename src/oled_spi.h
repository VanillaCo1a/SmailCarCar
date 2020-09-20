//OLED控制用函数
void OLED_Write(uchar, uchar);	
void Init_OLED(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Clear(void);
void OLED_DrawPoint(uchar, uchar, uchar);
void OLED_Fill(uchar, uchar, uchar, uchar, uchar);
void OLED_ShowChar(uchar, uchar, uchar);
void OLED_ShowNum(uchar, uchar, uint, uchar, uchar);
void OLED_ShowString(uchar, uchar, uchar *);	 
void OLED_Set_Pos(uchar, uchar);
void OLED_ShowCHinese(uchar, uchar, uchar);
void OLED_DrawBMP(uchar, uchar, uchar, uchar, uchar *);