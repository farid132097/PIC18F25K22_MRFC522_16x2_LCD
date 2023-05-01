/* 
 * File:   lcd.h
 * Author: farid
 * faridmdislam@gmail.com
 * Created on May 1, 2023, 10:07 PM
 */


void LCD_RS_Output(void);
void LCD_RS_Output_Low(void);
void LCD_RS_Output_High(void);
void LCD_RW_Output(void);
void LCD_RW_Output_Low(void);
void LCD_RW_Output_High(void);
void LCD_E_Output(void);
void LCD_E_Output_Low(void);
void LCD_E_Output_High(void);
void LCD_D4_Output(void);
void LCD_D4_Output_Low(void);
void LCD_D4_Output_High(void);
void LCD_D5_Output(void);
void LCD_D5_Output_Low(void);
void LCD_D5_Output_High(void);
void LCD_D6_Output(void);
void LCD_D6_Output_Low(void);
void LCD_D6_Output_High(void);
void LCD_D7_Output(void);
void LCD_D7_Output_Low(void);
void LCD_D7_Output_High(void);
void LCD_Write_Nibble_To_Data_Pins(uint8_t val);
void LCD_Write_Cmd(uint8_t val);
void LCD_Write_Data(uint8_t val);
void LCD_Write_String(char* str);
void LCD_Clear(void);
void LCD_GPIO_Init(void);
void LCD_Registers_Init(void);
void LCD_Init(void);

