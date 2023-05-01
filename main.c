/*
 * File:   main.c
 * Author: farid
 * faridmdislam@gmail.com
 * Created on May 1, 2023, 3:20 PM
 */

#define _XTAL_FREQ 8000000UL
#include <xc.h>
#include "configbits.h"
#include "mfrc522.h"
#include "lcd.h"


void main(void) {
    MFRC522_Init();
    LCD_Init();
    LCD_Write_String("RFID READY");
    
    while(1){
        uint8_t status = MFRC522_Detect_Tag();
        if(status == 1){
            LCD_Clear();
            LCD_Write_String("CARD FOUND");
            __delay_ms(5000);
            __delay_ms(5000);
        }else{
            LCD_Clear();
        }
        
        __delay_ms(50);
    }
    
    return;
}
