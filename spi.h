/* 
 * File:   spi.h
 * Author: farid
 *
 * Created on May 1, 2023, 3:21 PM
 */

#include <xc.h>
#include <stdint.h>

void     SPI_SCK_Output(void);
void     SPI_SCK_Output_Low(void);
void     SPI_SCK_Output_High(void);
void     SPI_MISO_Input(void);
uint8_t  SPI_MISO_State(void);
void     SPI_MOSI_Output(void);
void     SPI_MOSI_Output_Low(void);
void     SPI_MOSI_Output_High(void);
void     SPI_CS_Output(void);
void     SPI_CS_Output_Low(void);
void     SPI_CS_Output_High(void);
void     SPI_GPIO_Init(void);
void     SPI_Timer_Init(void);
void     SPI_Timer_Wait_One_Bit_Time(void);
void     SPI_Timer_Wait_Half_Bit_Time(void);
uint8_t  SPI_Transfer_Byte_MSB_First(uint8_t val);
uint8_t  SPI_Transfer_Byte_LSB_First(uint8_t val);
void     SPI_Transfer(uint8_t *data, uint8_t len);
void     SPI_Init(void);

