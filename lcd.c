
#define  _XTAL_FREQ   8000000UL
#include <xc.h>
#include <stdint.h>

#define LCD_RS_TRIS   TRISC
#define LCD_RS_LAT    LATC
#define LCD_RS_PORT   PORTC
#define LCD_RS_ANSEL  ANSELC
#define LCD_RS_bp     0x02U

#define LCD_RW_TRIS   TRISC
#define LCD_RW_LAT    LATC
#define LCD_RW_PORT   PORTC
#define LCD_RW_ANSEL  ANSELC
#define LCD_RW_bp     0x06U

#define LCD_E_TRIS    TRISC
#define LCD_E_LAT     LATC
#define LCD_E_PORT    PORTC
#define LCD_E_ANSEL   ANSELC
#define LCD_E_bp      0x07U

#define LCD_D4_TRIS   TRISB
#define LCD_D4_LAT    LATB
#define LCD_D4_PORT   PORTB
#define LCD_D4_ANSEL  ANSELB
#define LCD_D4_bp     0x04U

#define LCD_D5_TRIS   TRISB
#define LCD_D5_LAT    LATB
#define LCD_D5_PORT   PORTB
#define LCD_D5_ANSEL  ANSELB
#define LCD_D5_bp     0x05U

#define LCD_D6_TRIS   TRISB
#define LCD_D6_LAT    LATB
#define LCD_D6_PORT   PORTB
#define LCD_D6_ANSEL  ANSELB
#define LCD_D6_bp     0x06U

#define LCD_D7_TRIS   TRISB
#define LCD_D7_LAT    LATB
#define LCD_D7_PORT   PORTB
#define LCD_D7_ANSEL  ANSELB
#define LCD_D7_bp     0x07U

void LCD_RS_Output(void){
    LCD_RS_TRIS&=~(1<<LCD_RS_bp);
    LCD_RS_ANSEL&=~(1<<LCD_RS_bp);
}

void LCD_RS_Output_Low(void){
    LCD_RS_LAT&=~(1<<LCD_RS_bp);
}

void LCD_RS_Output_High(void){
    LCD_RS_LAT|=(1<<LCD_RS_bp);
}

void LCD_RW_Output(void){
    LCD_RW_TRIS&=~(1<<LCD_RW_bp);
    LCD_RW_ANSEL&=~(1<<LCD_RW_bp);
}

void LCD_RW_Output_Low(void){
    LCD_RW_LAT&=~(1<<LCD_RW_bp);
}

void LCD_RW_Output_High(void){
    LCD_RW_LAT|=(1<<LCD_RW_bp);
}

void LCD_E_Output(void){
    LCD_E_TRIS&=~(1<<LCD_E_bp);
    LCD_E_ANSEL&=~(1<<LCD_E_bp);
}

void LCD_E_Output_Low(void){
    LCD_E_LAT&=~(1<<LCD_E_bp);
}

void LCD_E_Output_High(void){
    LCD_E_LAT|=(1<<LCD_E_bp);
}

void LCD_D4_Output(void){
    LCD_D4_TRIS&=~(1<<LCD_D4_bp);
    LCD_D4_ANSEL&=~(1<<LCD_D4_bp);
}

void LCD_D4_Output_Low(void){
    LCD_D4_LAT&=~(1<<LCD_D4_bp);
}

void LCD_D4_Output_High(void){
    LCD_D4_LAT|=(1<<LCD_D4_bp);
}

void LCD_D5_Output(void){
    LCD_D5_TRIS&=~(1<<LCD_D5_bp);
    LCD_D5_ANSEL&=~(1<<LCD_D5_bp);
}

void LCD_D5_Output_Low(void){
    LCD_D5_LAT&=~(1<<LCD_D5_bp);
}

void LCD_D5_Output_High(void){
    LCD_D5_LAT|=(1<<LCD_D5_bp);
}

void LCD_D6_Output(void){
    LCD_D6_TRIS&=~(1<<LCD_D6_bp);
    LCD_D6_ANSEL&=~(1<<LCD_D6_bp);
}

void LCD_D6_Output_Low(void){
    LCD_D6_LAT&=~(1<<LCD_D6_bp);
}

void LCD_D6_Output_High(void){
    LCD_D6_LAT|=(1<<LCD_D6_bp);
}

void LCD_D7_Output(void){
    LCD_D7_TRIS&=~(1<<LCD_D7_bp);
    LCD_D7_ANSEL&=~(1<<LCD_D7_bp);
}

void LCD_D7_Output_Low(void){
    LCD_D7_LAT&=~(1<<LCD_D7_bp);
}

void LCD_D7_Output_High(void){
    LCD_D7_LAT|=(1<<LCD_D7_bp);
}

void LCD_Write_Nibble_To_Data_Pins(uint8_t val){
    if(val & 0x01){
        LCD_D4_Output_High();
    }else{
        LCD_D4_Output_Low();
    }
    if(val & 0x02){
        LCD_D5_Output_High();
    }else{
        LCD_D5_Output_Low();
    }
    if(val & 0x04){
        LCD_D6_Output_High();
    }else{
        LCD_D6_Output_Low();
    }
    if(val & 0x08){
        LCD_D7_Output_High();
    }else{
        LCD_D7_Output_Low();
    }
}

void LCD_Write_Cmd(uint8_t val){
    LCD_RS_Output_Low();
    LCD_Write_Nibble_To_Data_Pins(val>>4);
    LCD_E_Output_High();
    __delay_ms(20);
    LCD_E_Output_Low();
    LCD_Write_Nibble_To_Data_Pins(val & 0x0F);
    LCD_E_Output_High();
    __delay_ms(20);
    LCD_E_Output_Low();
}

void LCD_Write_Data(uint8_t val){
    LCD_RS_Output_High();
    LCD_Write_Nibble_To_Data_Pins(val>>4);
    LCD_E_Output_High();
    __delay_ms(20);
    LCD_E_Output_Low();
    LCD_Write_Nibble_To_Data_Pins(val & 0x0F);
    LCD_E_Output_High();
    __delay_ms(20);
    LCD_E_Output_Low();
}

void LCD_Write_String(char* str){
    while (*str != '\0'){
        LCD_Write_Data(*str);
        str++;
        __delay_ms(20);
    }
}

void LCD_Clear(void){
    LCD_Write_Cmd(0x00);
    LCD_Write_Cmd(0x01);
    __delay_ms(20);
}

void LCD_GPIO_Init(void){
    LCD_RS_Output();
    LCD_RW_Output();
    LCD_E_Output();
    LCD_D4_Output();
    LCD_D5_Output();
    LCD_D6_Output();
    LCD_D7_Output();
    LCD_RS_Output_Low();
    LCD_RW_Output_Low();
    LCD_E_Output_Low();
    LCD_D4_Output_Low();
    LCD_D5_Output_Low();
    LCD_D6_Output_Low();
    LCD_D7_Output_Low();
}

void LCD_Registers_Init(void){
    __delay_ms(200);
    LCD_Write_Cmd(0x03);
    __delay_ms(20);
    LCD_Write_Cmd(0x02);
    __delay_ms(20);
    LCD_Write_Cmd(0x28);
    __delay_ms(20);
    LCD_Write_Cmd(0x01);
    __delay_ms(20);
    LCD_Write_Cmd(0x0C);
    __delay_ms(20);
    LCD_Write_Cmd(0x06);
    __delay_ms(20);
}

void LCD_Init(void){
    __delay_ms(200);
    LCD_GPIO_Init();
    LCD_Registers_Init();
    LCD_Clear();
    __delay_ms(200);
}