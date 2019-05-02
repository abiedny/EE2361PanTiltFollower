#include "pantilt.h"

void setupPanTilt() {
    T3CON = 0x0010; //Stop Timer, Tcy clk source, PRE 1:8

    TMR3 = 0;     // Initialize to zero (also best practice)
    PR3 = 39999; // Set period to be 20 ms

    T3CONbits.TON = 1; // Restart 16-bit Timer3
    
    // Set output compare
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18;  // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    RPOR3bits.RP7R = 19;  // Use Pin RP7 for Output Compare 2 = "19" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    // Timer 3 setup should happen before this line
    
    // Output compare 1
    OC1CON = 0;    // turn off OC1 for now
    OC1R = 20000;   // servo start position. We won?t touch OC1R again
    OC1RS = 20000;  // We will only change this once PWM is turned on
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
    
    OC2CON = 0;    // turn off OC1 for now
    OC2R = 20000;   // servo start position. We won?t touch OC1R again
    OC2RS = 20000;  // We will only change this once PWM is turned on
    OC2CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC2CONbits.OCM = 0b110; // Output compare PWM w/o faults
    
    // Sweep Timer
    T2CON = 0x000; //Stop Timer, Tcy clk source, PRE 1:256
    T2CONbits.TCKPS = 0b11;

    TMR2 = 0;     // Initialize to zero (also best practice)
    PR2 = 65535; // Set period to be maximum

    T2CONbits.TON = 1; // Restart 16-bit Timer2
    
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
}

int pan_state = PANTILT_PAN_STATE_IDLE;
int tilt_state = PANTILT_TILT_STATE_IDLE;

int lerpAngleToServoArgument(int angle) {
    int angleRange = SERVO_MAX_ANGLE - SERVO_MIN_ANGLE;
    int pulseRange = SERVO_MAX_PULSE_LEN - SERVO_MIN_PULSE_LEN;
    int us = ((float)(angle - SERVO_MIN_ANGLE) / angleRange) * pulseRange + SERVO_MIN_PULSE_LEN;
    return us / 2;
}

void _setPanAngle(int angle) {
    OC1RS = lerpAngleToServoArgument(angle);
}

void _setTiltAngle(int angle) {
    OC2RS = lerpAngleToServoArgument(angle);
}

void setPanAngle(int angle) {
    pan_state = PANTILT_PAN_STATE_IDLE;
    _setPanAngle(angle);
}

void setTiltAngle(int angle) {
    tilt_state = PANTILT_TILT_STATE_IDLE;
    _setTiltAngle(angle);
}

void setPanAndTilt(int pan, int tilt) {
    setPanAngle(pan);
    setTiltAngle(tilt);
}

void setSweepSpeed(int degreesPerSec) {
    // Minimum 1 degree/sec, maximum 65565 degree/sec
    PR2 = 65565 / degreesPerSec;
}

// Sweep

int sweepAnglePan = 0;
int sweepAngleTilt = 0;
int sweepDirPan = 1;
int sweepDirTilt = 1;
int sweepPanMax = 180;
int sweepTiltMax = 180;
int sweepPanMin = -180;
int sweepTiltMin = -180;
void __attribute__((interrupt, auto_psv)) _T2Interrupt() {
    IFS0bits.T2IF = 0;
    sweepAnglePan += sweepDirPan;
    sweepAngleTilt += sweepDirTilt;
    
    // Apply tilt angles
    if (pan_state == PANTILT_PAN_STATE_SWEEP) {
        _setPanAngle(sweepAnglePan);
    }
    if (tilt_state == PANTILT_TILT_STATE_SWEEP) {
        _setTiltAngle(sweepAngleTilt);
    }
    
    // If at bounds, change direction
    if (sweepAnglePan == sweepPanMax) {
        sweepDirPan = -1;
    }
    if (sweepAngleTilt == sweepTiltMax) {
        sweepDirTilt = -1;
    }
    if (sweepAnglePan == sweepPanMin) {
        sweepDirPan = 1;
    }
    if (sweepAngleTilt == sweepTiltMin) {
        sweepDirTilt = 1;
    }
}

void setTiltSweep(int minAngle, int maxAngle) {
    sweepTiltMin = minAngle;
    sweepTiltMax = maxAngle;
    sweepAngleTilt = minAngle;
    sweepDirTilt = 1;
    tilt_state = PANTILT_TILT_STATE_SWEEP;
}

void setPanSweep(int minAngle, int maxAngle) {
    sweepPanMin = minAngle;
    sweepPanMax = maxAngle;
    sweepAnglePan = minAngle;
    sweepDirPan = 1;
    pan_state = PANTILT_PAN_STATE_SWEEP;
}