/*
 * File:   main.c
 * Author: nick
 *
 * Created on May 2, 2019, 9:52 AM
 */


#include "xc.h"
#include "pantilt.h"

void setup(void);

int buffer[2];
int bufptr = 0;

int main(void) {
    setup();
    
    
    setPanSweep(-90, 90);
    setTiltSweep(-90, 90);
    while (1) {
        //setPanAndTilt(0, 0);
        int i = IFS0bits.U1RXIF;
        if (IFS0bits.U1RXIF) {
            IFS0bits.U1RXIF = 0;
            buffer[bufptr++] = U1RXREG;
            if (bufptr == 2) {
                bufptr = 0;
            }
        }
    }
    
    return 0;
}

void setup(void) {
    CLKDIV = 0;
    AD1PCFG = 0x9fff;
    setupPanTilt();
    setSweepSpeed(400);
    
    _TRISB6 = 0;  // U1TX output
    _TRISB10 = 1; // U1RX input
    
    U1MODE = 0;  // UEN<1:0> bits control the pins
    U1MODEbits.BRGH = 0;
    U1STA = 0;
    U1BRG = 25; // 38400 baud (check the calculation with the datasheet)
    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    // Peripheral Pin Select
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;  
    _U1RXR = 10;   //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    IFS0bits.U1RXIF = 0;
    //IEC0bits.U1RXIE = 1;

    
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;
    buffer[bufptr++] = U1RXREG;
    if (bufptr == 2) {
        bufptr = 0;
    }
}