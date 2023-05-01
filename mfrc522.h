/* 
 * File:   mfrc522.h
 * Author: farid
 * 
 * Created on May 1, 2023, 4:47 PM
 */

#include <xc.h>
#include <stdint.h>

void     MFRC522_Write_Register(uint8_t addr, uint8_t val);
uint8_t  MFRC522_Read_Register(uint8_t addr);
void     MFRC522_Init(void);

