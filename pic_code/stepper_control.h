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
#ifndef STEPPER_CONTROL_H
#define	STEPPER_CONTROL_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "physical.h"

#define STANDARD_STEP   1.8     // 1.8 degree per full step
#define MICRO_STEP      16.0
#define DEGREE_PER_STEP STANDARD_STEP/MICRO_STEP
#define LEN_PER_STEP    (DEGREE_PER_STEP*PI*WHEEL_DIAM)/360.0 // mm per step
#define LEN_PER_REV     204.2   // mm traveled per 

#define OUTPUT   0
#define INPUT    1

#define FORWARD  0
#define BACKWARD 1

#define DISABLED 1
#define ENABLED  0

#define LEFT     0
#define RIGHT    1

//struct Stepper;

typedef struct {
    uint8_t  isEnabled;     // Whether motor driver is enabled
    uint16_t togglePoint;   // Trigger point to change step output state
    uint16_t curCount;      // Timer0 overflow count to compare with togglePoint
    uint8_t  curMode;       // Current state of step output pin
    uint8_t  whichMotor;    // Defines left (0) or right (1) motor
    uint16_t numSteps;      // Number of steps taken
    uint16_t targSteps;     // Target number of steps to take
} Stepper;

// initialize timer0
void tmr0_init(void);
// turn timer0 on/off
void tmr0_state(uint8_t state);
// initializes motor control struct
Stepper initializeMotor( uint8_t  leftOrRight);
// process timer overflow
void processStepper(Stepper *stpr);
// enable motor for operation
void enableStepper(Stepper *stpr, uint16_t togPoint);
// disables motor
void disableStepper(Stepper *stpr);
// Toggles step pin mode
void togglePWM(Stepper *stpr);
// sets motor dir
void setDir(Stepper *stpr, uint8_t dir);
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

