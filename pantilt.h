/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PANTILT_H
#define	PANTILT_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    /**
     * Sets up the pan/tilt servos, including setting up Timer3 and enabling 
     * output compares
     * 
     * This sets the pan servo on pin RP6 and tilt servo on pin RP7
     */
    void setupPanTilt();

    /**
     * Sets the pan servo to the angle between -90 and 90 degrees
     * This will cancel sweep on this axis
     * @param angle The angle to pan to
     */
    void setPanAngle(int angle);
    /**
     * Sets the tilt servo to the angle between -90 and 90 degrees
     * This will cancel sweep on this axis
     * @param angle The angle to tilt to
     */
    void setTiltAngle(int angle);
    /**
     * Sets the pan and tilt angles in one call
     * @param pan The pan angle
     * @param tilt The tilt angle
     */
    void setPanAndTilt(int pan, int tilt);
    /**
     * Set the speed of sweep operations, from 1 degree/sec to 400 deg/sec
     * (servo slew rate limitation)
     * @param degreesPerSec The slew rate of the sweep operation
     */
    void setSweepSpeed(int degreesPerSec);
    /**
     * Enable sweep in the pan axis. This both sets the sweep bounds and enables
     * sweep.
     * @param minAngle The minimum (and start angle) of the sweep operation -90..90
     * @param maxAngle The maximum angle of the sweep operation -90..90
     */
    void setPanSweep(int minAngle, int maxAngle);
    /**
     * Enable sweep in the tilt axis. This both sets the sweep bounds and enables
     * sweep.
     * @param minAngle The minimum (and start angle) of the sweep operation -90..90
     * @param maxAngle The maximum angle of the sweep operation -90..90
     */
    void setTiltSweep(int minAngle, int maxAngle);
    
#define PANTILT_PAN_STATE_IDLE 0
#define PANTILT_PAN_STATE_SWEEP 1
#define PANTILT_TILT_STATE_IDLE 0
#define PANTILT_TILT_STATE_SWEEP 1
    
#define SERVO_MIN_ANGLE -90
#define SERVO_MAX_ANGLE 90
#define SERVO_MIN_PULSE_LEN 500
#define SERVO_MAX_PULSE_LEN 2500

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

