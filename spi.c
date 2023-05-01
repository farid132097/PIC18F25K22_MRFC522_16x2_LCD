

#include <xc.h>
#include <stdint.h>

#define SPI_SCK_TRIS   TRISC
#define SPI_SCK_LAT    LATC
#define SPI_SCK_ANSEL  ANSELC
#define SPI_SCK_bp     0x03U

#define SPI_MISO_TRIS  TRISC
#define SPI_MISO_PORT  PORTC
#define SPI_MISO_ANSEL ANSELC
#define SPI_MISO_bp    0x04U

#define SPI_MOSI_TRIS  TRISC
#define SPI_MOSI_LAT   LATC
#define SPI_MOSI_ANSEL ANSELC
#define SPI_MOSI_bp    0x05U

#define SPI_CS_TRIS    TRISB
#define SPI_CS_LAT     LATB
#define SPI_CS_ANSEL   ANSELB
#define SPI_CS_bp      0x02U

#define SPI_TIMER_ONE_BIT_COUNT  80U
#define SPI_TIMER_HALF_BIT_COUNT 40U

#define SPI_CONFIG_MSB_FIRST

void SPI_SCK_Output(void){
    SPI_SCK_TRIS&=~(1<<SPI_SCK_bp);
    SPI_SCK_ANSEL&=~(1<<SPI_SCK_bp);
}

void SPI_SCK_Output_Low(void){
    SPI_SCK_LAT&=~(1<<SPI_SCK_bp);
}

void SPI_SCK_Output_High(void){
    SPI_SCK_LAT|=(1<<SPI_SCK_bp);
}

void SPI_MISO_Input(void){
    SPI_MISO_TRIS|=(1<<SPI_MISO_bp);
    SPI_MISO_ANSEL&=~(1<<SPI_MISO_bp);
}

uint8_t SPI_MISO_State(void){
    if( SPI_MISO_PORT & (1<<SPI_MISO_bp) ){
        return 1;
    }else{
        return 0;
    }
}

void SPI_MOSI_Output(void){
    SPI_MOSI_TRIS&=~(1<<SPI_MOSI_bp);
    SPI_MOSI_ANSEL&=~(1<<SPI_MOSI_bp);
}

void SPI_MOSI_Output_Low(void){
    SPI_MOSI_LAT&=~(1<<SPI_MOSI_bp);
}

void SPI_MOSI_Output_High(void){
    SPI_MOSI_LAT|=(1<<SPI_MOSI_bp);
}

void SPI_CS_Output(void){
    SPI_CS_TRIS&=~(1<<SPI_CS_bp);
    SPI_CS_ANSEL&=~(1<<SPI_CS_bp);
}

void SPI_CS_Output_Low(void){
    SPI_CS_LAT&=~(1<<SPI_CS_bp);
}

void SPI_CS_Output_High(void){
    SPI_CS_LAT|=(1<<SPI_CS_bp);
}

void SPI_GPIO_Init(void){
    SPI_SCK_Output();
    SPI_SCK_Output_Low();
    SPI_MISO_Input();
    SPI_MOSI_Output();
    SPI_MOSI_Output_Low();
    SPI_CS_Output();
    SPI_CS_Output_High();
}

void SPI_Timer_Init(void){
    T0CON = (1<<7);
}

void SPI_Timer_Wait_One_Bit_Time(void){
    TMR0 = 0;
    while( TMR0 < SPI_TIMER_ONE_BIT_COUNT);
}

void SPI_Timer_Wait_Half_Bit_Time(void){
    TMR0 = 0;
    while( TMR0 < SPI_TIMER_HALF_BIT_COUNT);
}

uint8_t SPI_Transfer_Byte_MSB_First(uint8_t val){
    uint8_t data=0;
    SPI_Timer_Wait_One_Bit_Time();
    SPI_Timer_Wait_Half_Bit_Time();
    for(int i=7; i>=0; i--){
        if( (val>>i) & 1){
            SPI_MOSI_Output_High();
        }else{
            SPI_MOSI_Output_Low();
        }
        SPI_Timer_Wait_Half_Bit_Time();
        SPI_SCK_Output_High();
        data<<=1;
        data|=SPI_MISO_State();
        SPI_Timer_Wait_Half_Bit_Time();
        SPI_SCK_Output_Low();
    }
    SPI_Timer_Wait_One_Bit_Time();
    SPI_MOSI_Output_Low();
    SPI_Timer_Wait_Half_Bit_Time();
    return data;
}

uint8_t SPI_Transfer_Byte_LSB_First(uint8_t val){
    uint8_t data=0;
    SPI_Timer_Wait_One_Bit_Time();
    SPI_Timer_Wait_Half_Bit_Time();
    for(int i=0; i<8; i++){
        if( (val>>i) & 1){
            SPI_MOSI_Output_High();
        }else{
            SPI_MOSI_Output_Low();
        }
        SPI_Timer_Wait_Half_Bit_Time();
        SPI_SCK_Output_High();
        data|=(SPI_MISO_State() << i);
        SPI_Timer_Wait_Half_Bit_Time();
        SPI_SCK_Output_Low();
    }
    SPI_Timer_Wait_One_Bit_Time();
    SPI_MOSI_Output_Low();
    SPI_Timer_Wait_Half_Bit_Time();
    return data;
}

void SPI_Transfer(uint8_t *data, uint8_t len){
    SPI_CS_Output_Low();
    for(uint8_t i=0; i<len; i++){
        #ifdef SPI_CONFIG_MSB_FIRST
        data[i]=SPI_Transfer_Byte_MSB_First(data[i]);
        #else
        data[i]=SPI_Transfer_Byte_LSB_First(data[i]);
        #endif
    }
    SPI_CS_Output_High();
}

void SPI_Init(void){
    SPI_GPIO_Init();
    SPI_Timer_Init();
}