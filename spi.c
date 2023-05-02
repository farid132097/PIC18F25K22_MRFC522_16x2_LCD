

#include <xc.h>
#include <stdint.h>

/*SPI SCK PIN Definitions*/
#define SPI_SCK_TRIS   TRISC
#define SPI_SCK_LAT    LATC
#define SPI_SCK_ANSEL  ANSELC
#define SPI_SCK_bp     0x03U

/*SPI MISO PIN Definitions*/
#define SPI_MISO_TRIS  TRISC
#define SPI_MISO_PORT  PORTC
#define SPI_MISO_ANSEL ANSELC
#define SPI_MISO_bp    0x04U

/*SPI MOSI PIN Definitions*/
#define SPI_MOSI_TRIS  TRISC
#define SPI_MOSI_LAT   LATC
#define SPI_MOSI_ANSEL ANSELC
#define SPI_MOSI_bp    0x05U

/*SPI Chip Select PIN Definitions*/
#define SPI_CS_TRIS    TRISB
#define SPI_CS_LAT     LATB
#define SPI_CS_ANSEL   ANSELB
#define SPI_CS_bp      0x02U

/*SPI Bit Length Counter Values*/
#define SPI_TIMER_ONE_BIT_COUNT  80U
#define SPI_TIMER_HALF_BIT_COUNT 40U

/*SPI MODE MSB First*/
#define SPI_CONFIG_MSB_FIRST


void SPI_SCK_Output(void){
    SPI_SCK_TRIS&=~(1<<SPI_SCK_bp);          /*Declare SPI SCK Pin As Output*/
    SPI_SCK_ANSEL&=~(1<<SPI_SCK_bp);         /*Declare SPI SCK Pin As Digital Output*/
}

void SPI_SCK_Output_Low(void){
    SPI_SCK_LAT&=~(1<<SPI_SCK_bp);           /*Set SPI SCK Pin As Logic Low*/
}

void SPI_SCK_Output_High(void){
    SPI_SCK_LAT|=(1<<SPI_SCK_bp);            /*Set SPI SCK Pin As Logic High*/
}

void SPI_MISO_Input(void){
    SPI_MISO_TRIS|=(1<<SPI_MISO_bp);         /*Declare SPI MISO Pin As Input*/
    SPI_MISO_ANSEL&=~(1<<SPI_MISO_bp);       /*Declare SPI MISO Pin as Digital Input*/
}

uint8_t SPI_MISO_State(void){
    if( SPI_MISO_PORT & (1<<SPI_MISO_bp) ){  /*Check SPI MISO Pin Current State*/
        return 1;
    }else{
        return 0;
    }
}

void SPI_MOSI_Output(void){
    SPI_MOSI_TRIS&=~(1<<SPI_MOSI_bp);        /*Declare SPI MOSI Pin As Output*/
    SPI_MOSI_ANSEL&=~(1<<SPI_MOSI_bp);       /*Declare SPI MOSI Pin As Digital Output*/
}

void SPI_MOSI_Output_Low(void){
    SPI_MOSI_LAT&=~(1<<SPI_MOSI_bp);         /*Set SPI MOSI Pin As Logic Low*/
}

void SPI_MOSI_Output_High(void){
    SPI_MOSI_LAT|=(1<<SPI_MOSI_bp);          /*Set SPI MOSI Pin As Logic High*/
}

void SPI_CS_Output(void){
    SPI_CS_TRIS&=~(1<<SPI_CS_bp);            /*Declare SPI CS Pin As Output*/
    SPI_CS_ANSEL&=~(1<<SPI_CS_bp);           /*Declare SPI CS Pin As Digital Output*/
}

void SPI_CS_Output_Low(void){
    SPI_CS_LAT&=~(1<<SPI_CS_bp);             /*Set SPI CS Pin As Logic Low*/
}

void SPI_CS_Output_High(void){
    SPI_CS_LAT|=(1<<SPI_CS_bp);              /*Set SPI CS Pin As Logic High*/
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
    T0CON = (1<<7);                            /*Use Timer0 For Bit Time Count*/
}

void SPI_Timer_Wait_One_Bit_Time(void){
    TMR0 = 0;                                 /*Clear Timer0 Counter*/
    while( TMR0 < SPI_TIMER_ONE_BIT_COUNT);   /*Wait Until Counter Reaches To Defined Value*/ 
}

void SPI_Timer_Wait_Half_Bit_Time(void){
    TMR0 = 0;                                 /*Clear Timer0 Counter*/
    while( TMR0 < SPI_TIMER_HALF_BIT_COUNT);  /*Wait Until Counter Reaches To Defined Value*/ 
}

uint8_t SPI_Transfer_Byte_MSB_First(uint8_t val){
    uint8_t data=0;
    SPI_Timer_Wait_One_Bit_Time();            /*Wait One Bit Time*/
    SPI_Timer_Wait_Half_Bit_Time();           /*Wait Half Bit Time*/
    for(int i=7; i>=0; i--){                  /*Shift 8 Bits To MOSI Pin*/
        if( (val>>i) & 1){                    /*MSB First*/
            SPI_MOSI_Output_High();
        }else{
            SPI_MOSI_Output_Low();
        }
        SPI_Timer_Wait_Half_Bit_Time();       /*Wait Half Bit Time*/
        SPI_SCK_Output_High();                /*Rising Edge On SCK */
        data<<=1;                             /*Left Shift Previous MISO data*/
        data|=SPI_MISO_State();               /*Add Current MISO data*/
        SPI_Timer_Wait_Half_Bit_Time();       /*Wait Half Bit Time*/
        SPI_SCK_Output_Low();                 /*Falling Edge On SCK */
    }
    SPI_Timer_Wait_One_Bit_Time();            /*Wait One Bit Time*/
    SPI_MOSI_Output_Low();                    /*Hold MOSI Logic Low*/
    SPI_Timer_Wait_Half_Bit_Time();           /*Wait Half Bit Time*/
    return data;
}

uint8_t SPI_Transfer_Byte_LSB_First(uint8_t val){
    uint8_t data=0;
    SPI_Timer_Wait_One_Bit_Time();           /*Wait One Bit Time*/
    SPI_Timer_Wait_Half_Bit_Time();          /*Wait Half Bit Time*/
    for(int i=0; i<8; i++){                  /*Shift 8 Bits To MOSI Pin*/
        if( (val>>i) & 1){                   /*LSB First*/
            SPI_MOSI_Output_High();
        }else{
            SPI_MOSI_Output_Low();
        }
        SPI_Timer_Wait_Half_Bit_Time();     /*Wait Half Bit Time*/
        SPI_SCK_Output_High();              /*Rising Edge On SCK */
        data|=(SPI_MISO_State() << i);      /*Left Shift Current MISO data*/
        SPI_Timer_Wait_Half_Bit_Time();     /*Wait Half Bit Time*/
        SPI_SCK_Output_Low();               /*Falling Edge On SCK */
    }
    SPI_Timer_Wait_One_Bit_Time();          /*Wait One Bit Time*/
    SPI_MOSI_Output_Low();                  /*Hold MOSI Logic Low*/
    SPI_Timer_Wait_Half_Bit_Time();         /*Wait Half Bit Time*/
    return data;
}

void SPI_Transfer(uint8_t *data, uint8_t len){        /*Transfer Data Bytes Using SPI */
    SPI_CS_Output_Low();                              /*Enable Chip By CS Pin Logic Low*/
    for(uint8_t i=0; i<len; i++){
        #ifdef SPI_CONFIG_MSB_FIRST
        data[i]=SPI_Transfer_Byte_MSB_First(data[i]); /*Read MISO Data, MSB First Mode*/
        #else
        data[i]=SPI_Transfer_Byte_LSB_First(data[i]); /*Read MISO Data, LSB First Mode*/
        #endif
    }
    SPI_CS_Output_High();                             /*Disable Chip By CS Pin Logic High*/
}

void SPI_Init(void){
    SPI_GPIO_Init();                                  /*SPI GPIO Initialization*/
    SPI_Timer_Init();                                 /*SPI Bit Timer Initialization*/
}