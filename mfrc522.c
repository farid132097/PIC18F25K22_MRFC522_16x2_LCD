
#define  _XTAL_FREQ          8000000UL
#include <xc.h>
#include <stdint.h>
#include "spi.h"

//MF522 Command word
#define PCD_IDLE             0x00               //NO action;
#define PCD_AUTHENT          0x0E               //Authentication Key
#define PCD_RECEIVE          0x08               //Receive Data
#define PCD_TRANSMIT         0x04               //Transmit data
#define PCD_TRANSCEIVE       0x0C               //Transmit and receive data,
#define PCD_RESETPHASE       0x0F               //Reset
#define PCD_CALCCRC          0x03               //CRC Calculate

// Mifare_One card command word
#define PICC_REQIDL          0x26               // find the antenna area does not enter hibernation
#define PICC_REQALL          0x52               // find all the cards antenna area
#define PICC_ANTICOLL        0x93               // anti-collision
#define PICC_SELECTTAG       0x93               // election card
#define PICC_AUTHENT1A       0x60               // authentication key A
#define PICC_AUTHENT1B       0x61               // authentication key B
#define PICC_READ            0x30               // Read Block
#define PICC_WRITE           0xA0               // write block
#define PICC_DECREMENT       0xC0               // debit
#define PICC_INCREMENT       0xC1               // recharge
#define PICC_RESTORE         0xC2               // transfer block data to the buffer
#define PICC_TRANSFER        0xB0               // save the data in the buffer
#define PICC_HALT            0x50               // Sleep

//And MF522 The error code is returned when communication
#define MFRC522_OK                0x00
#define MFRC522_NOTAGERR          0x01
#define MFRC522_ERR               0x02

//------------------MFRC522 Register---------------
//Page 0:   Command and Status
#define     Reserved00            0x00
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivlEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F

//Page 1:   Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F

//Page 2:   CFG
#define     Reserved20            0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg              0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F

//Page 3:   TestRegister
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     Reserved31            0x3C
#define     Reserved32            0x3D
#define     Reserved33            0x3E
#define     Reserved34            0x3F


#define     MFRC522_RST_TRIS      TRISB
#define     MFRC522_RST_LAT       LATB
#define     MFRC522_RST_ANSEL     ANSELB
#define     MFRC522_RST_bp        0x01U


void MFRC522_Write_Register(uint8_t addr, uint8_t val){
    uint8_t data[2];
    data[0]=(addr<<1)&0x7E;
    data[1]=val;
    SPI_Transfer(data, 2);
}

uint8_t MFRC522_Read_Register(uint8_t addr){
    uint8_t data[2];
    data[0]=((addr<<1)&0x7E) | 0x80;
    data[1]=0;
    SPI_Transfer(data, 2);
    return data[1];
}

void MFRC522_Modify_Register_Bit(uint8_t addr, uint8_t bit_pos, uint8_t val){
    uint8_t data=0;
    data=MFRC522_Read_Register(addr);
    if(val == 1){
        data|=(1<<bit_pos);
    }else if(val == 0){
        data&=~(1<<bit_pos);
    }
    MFRC522_Write_Register(addr, data);
}

void MFRC522_Antenna_On(void){
    uint8_t data=0;
    data = MFRC522_Read_Register(TxControlReg);
    if (!(data & 0x03)){
        MFRC522_Modify_Register_Bit(TxControlReg, 0, 1);
        MFRC522_Modify_Register_Bit(TxControlReg, 1, 1);
    }
}

void MFRC522_Antenna_Off(void){
    MFRC522_Modify_Register_Bit(TxControlReg, 0, 0);
    MFRC522_Modify_Register_Bit(TxControlReg, 1, 0);
}

void MFRC522_Reset(void){
    MFRC522_Write_Register(CommandReg, PCD_RESETPHASE);
}

#define MAX_LEN 16
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint8_t *backLen){
    uint8_t  status = MFRC522_ERR;
    uint8_t  irqEn = 0x00;
    uint8_t  waitIRq = 0x00;
    uint8_t  lastBits;
    uint8_t  n;
    uint16_t i;

    switch (command){
        case PCD_AUTHENT:           
        {
            irqEn = 0x12;
            waitIRq = 0x10;
            break;
        }
        case PCD_TRANSCEIVE:      
        {
           irqEn = 0x77;
           waitIRq = 0x30;
           break;
        }
        default:
        break;
  }

    MFRC522_Write_Register(CommIEnReg, irqEn|0x80);      //Interrupt request
    MFRC522_Modify_Register_Bit(CommIrqReg, 7, 0);       //Clear all interrupt request bit
    MFRC522_Modify_Register_Bit(FIFOLevelReg, 7, 1);     //FlushBuffer=1, FIFO Initialization
    MFRC522_Write_Register(CommandReg, PCD_IDLE);        //NO action;
    for (i=0; i<sendLen; i++){
        MFRC522_Write_Register(FIFODataReg, sendData[i]);
    }

    MFRC522_Write_Register(CommandReg, command);
    if (command == PCD_TRANSCEIVE){
        MFRC522_Modify_Register_Bit(BitFramingReg, 7, 1); //StartSend=1,transmission of data starts
    }

    i = 2000;        
    do{
        n = MFRC522_Read_Register(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n & waitIRq));
    MFRC522_Modify_Register_Bit(BitFramingReg, 7, 0);     //StartSend=0
    if (i != 0){
        if(!(MFRC522_Read_Register(ErrorReg) & 0x1B)){
            status = MFRC522_OK;
            if (n & irqEn & 0x01){
              status = MFRC522_NOTAGERR;                      
            }
            if (command == PCD_TRANSCEIVE){
                n = MFRC522_Read_Register(FIFOLevelReg);
                lastBits = MFRC522_Read_Register(ControlReg) & 0x07;
                if (lastBits){
                    *backLen = (n-1)*8 + lastBits;
                }
                else{
                    *backLen = n*8;
                }
                if (n == 0){
                    n = 1;
                }
                if (n > MAX_LEN){
                    n = MAX_LEN;
                }
                for (i=0; i<n; i++){
                    backData[i] = MFRC522_Read_Register(FIFODataReg);
                }
            }
       }
       else{
           status = MFRC522_ERR;
       }
    }
    return status;
}

uint8_t MFRC522_Anticoll(uint8_t *serNum){
    uint8_t status;
    uint8_t i;
    uint8_t serNumCheck=0;
    uint8_t unLen;
    MFRC522_Write_Register(BitFramingReg, 0x00); 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MFRC522_OK){
        for (i=0; i<4; i++){
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]){
            status = MFRC522_ERR;
        }
    }
    return status;
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType){
    uint8_t status;
    uint8_t backBits;                        

    MFRC522_Write_Register(BitFramingReg, 0x07);  

    TagType[0] = reqMode;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

    if ((status != MFRC522_OK) || (backBits != 0x10)){
        status = MFRC522_ERR;
    }
    return status;
}

uint8_t MFRC522_Detect_Tag(void){
    uint8_t info[MAX_LEN];
    uint8_t status = MFRC522_Request(PICC_REQIDL, info);
    status = MFRC522_Anticoll(info);
    if(status == MFRC522_OK){
        return 1;
    }else{
        return 0;
    }
}

void MFRC522_Reset_Output(void){
    MFRC522_RST_TRIS&=~(1<<MFRC522_RST_bp);
    MFRC522_RST_ANSEL&=~(1<<MFRC522_RST_bp);
}

void MFRC522_Reset_Output_Low(void){
    MFRC522_RST_LAT&=~(1<<MFRC522_RST_bp);
}

void MFRC522_Reset_Output_High(void){
    MFRC522_RST_LAT|=(1<<MFRC522_RST_bp);
}

void MFRC522_Init(void){
    MFRC522_Reset_Output();
    SPI_Init();
    MFRC522_Reset_Output_Low();
    __delay_ms(50);
    MFRC522_Reset_Output_High();
    MFRC522_Reset();
    MFRC522_Write_Register(TModeReg, 0x8D);
    MFRC522_Write_Register(TPrescalerReg, 0x3E);
    MFRC522_Write_Register(TReloadRegL, 30);
    MFRC522_Write_Register(TReloadRegH, 0);
    MFRC522_Write_Register(TxAutoReg, 0x40);  
    MFRC522_Write_Register(ModeReg, 0x3D);
    MFRC522_Antenna_On();
}

