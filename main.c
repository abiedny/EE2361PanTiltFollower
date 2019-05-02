/*
 * File:   main.c
 * Author: nick
 *
 * Created on May 2, 2019, 9:52 AM
 */


#include "xc.h"
#include "pantilt.h"

void setup(void);

int main(void) {
    setup();
    
    
    setPanSweep(-90, 90);
    setTiltSweep(-90, 90);
    while (1) {
        //setPanAndTilt(0, 0);
    }
    
    return 0;
}

void setup(void) {
    CLKDIV = 0;
    AD1PCFG = 0x9fff;
    setupPanTilt();
    setSweepSpeed(400);
    
    // Setup UART
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR18bits.U1RXR = 15;  // Use Pin RP15 (Table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    U1MODE = 0;
    U1MODEbits.BRGH = 0;
    U1BRG = 136; // 115200 baud

    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;

    
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;
    int test = U1RXREG;
}
