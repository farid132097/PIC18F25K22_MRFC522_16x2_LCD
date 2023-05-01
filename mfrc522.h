/* 
 * File:   mfrc522.h
 * Author: farid
 * faridmdislam@gmail.com
 * Created on May 1, 2023, 4:47 PM
 */

#include <xc.h>
#include <stdint.h>

void     MFRC522_Write_Register(uint8_t addr, uint8_t val);
uint8_t  MFRC522_Read_Register(uint8_t addr);
void     MFRC522_Modify_Register_Bit(uint8_t addr, uint8_t bit_pos, uint8_t val);
void     MFRC522_Antenna_On(void);
void     MFRC522_Antenna_Off(void);
void     MFRC522_Reset(void);
uint8_t  MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen);
uint8_t  MFRC522_Anticoll(uint8_t *serNum);
uint8_t  MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t  MFRC522_Detect_Tag(void);
void     MFRC522_Reset_Output(void);
void     MFRC522_Reset_Output_Low(void);
void     MFRC522_Reset_Output_High(void);
void     MFRC522_Init(void);

