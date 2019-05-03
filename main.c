/*
 * File:   main.c
 * Author: nick
 *
 * Created on May 2, 2019, 9:52 AM
 */


#include "xc.h"
#include "pantilt.h"

#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

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
