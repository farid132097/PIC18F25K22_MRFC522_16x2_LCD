/*
 * File:   main.c
 * Author: farid
 * faridmdislam@gmail.com
 * Created on May 1, 2023, 3:20 PM
 */


#include <xc.h>
#include "configbits.h"
#include "mfrc522.h"


void main(void) {
    MFRC522_Init();
    
    
    while(1){
        uint8_t status = MFRC522_Detect_Tag();
        if(status == 1){
            //Tag Detected
            //LCD print "Detected"
            //Wait 10 Seconds
            //Clear LCD
        }else{
            //Do nothing
        }
        
        //Add some delay
    }
    
    return;
}
