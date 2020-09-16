//OLED控制用函数
void OLED_Write(uchar, uchar);	
void Init_OLED(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Clear(void);
void OLED_DrawPoint(uchar x, uchar y, uchar t);
void OLED_Fill(uchar x1, uchar y1, uchar x2, uchar y2, uchar dot);
void OLED_ShowChar(uchar x, uchar y, uchar chr);
void OLED_ShowNum(uchar x, uchar y, uint num, uchar len, uchar size2);
void OLED_ShowString(uchar x, uchar y, uchar *p);	 
void OLED_Set_Pos(uchar x, uchar y);
void OLED_ShowCHinese(uchar x, uchar y, uchar no);
void OLED_DrawBMP(uchar x0, uchar y0, uchar x1, uchar y1, uchar BMP[]);