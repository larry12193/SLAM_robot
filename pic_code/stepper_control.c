
#include "stepper_control.h"
#include "physical.h"
#include "pinout.h"
#include <p18f2550.h> 

// initialize timer0 for software PWM
void tmr0_init(void) {
    T0CONbits.PSA    = 0x00;    // Enable timer0 prescaler
    T0CONbits.T0PS   = 0x01;    // Set 1:8 prescaler (250k trans/sec)
    
    T0CONbits.T0CS   = 0x00;    // Trigger on internal clock source
    T0CONbits.T08BIT = 0x01;    // Set 8-bit timer operation
    //T0CONbits.TMR0ON = 0x01;    // Turn timer on
}

// turn timer0 on/off
void tmr0_state(uint8_t state) {
    if( state == DISABLED ) {
        T0CONbits.TMR0ON = 0x00;
    }
    else if( state == ENABLED) {
        T0CONbits.TMR0ON = 0x01;
    }
    
}
// initialize motor control struct
Stepper initializeMotor( uint8_t  leftOrRight) {
    
    Stepper motor;
    
    motor.isEnabled   = 0;
    motor.togglePoint = 0;
    motor.curCount    = 0;
    motor.curMode     = 0;
    motor.whichMotor  = leftOrRight;
    motor.numSteps    = 0;
    motor.targSteps   = 0;
    
    if( motor.whichMotor == 0 ) {
        L_DIR_TRIS = OUTPUT;
        L_STP_TRIS = OUTPUT;
        L_ENB_TRIS = OUTPUT;
        L_STP      = 0;
        L_DIR      = FORWARD;
        L_ENB      = DISABLED;
    }
    else {
        R_DIR_TRIS = OUTPUT;
        R_STP_TRIS = OUTPUT;
        R_ENB_TRIS = OUTPUT;
        R_STP      = 0;
        R_DIR      = FORWARD;
        R_ENB      = DISABLED;
    }
    return motor;
}

// acknowledge timer0 overflow for step signal generation
void processStepper(Stepper *stpr) {
    // Only process if the motor is enabled
    if( (*stpr).isEnabled == ENABLED ) {
        // Increment timer overflow counter for compare
        (*stpr).curCount++;
        // Compare count to toggle point
        if( (*stpr).curCount >= (*stpr).togglePoint ) {
            // Toggle the step pin to create PWM
            togglePWM(stpr);
            // Reset counter
            (*stpr).curCount = 0;
            // Increment step counter
            (*stpr).numSteps++;
        }
        
        // Check if we have reached target number of steps
        //  Note: numSteps will be twice the target steps since it increments
        //        on every edge change, rather then every rising edge
        if( (*stpr).numSteps >= (*stpr).targSteps ) {
            disableStepper(&(*stpr));
        }
    }
}

void togglePWM(Stepper *stpr) {
    if( (*stpr).curMode == 0) {
        (*stpr).curMode = 1;
    }
    else {
        (*stpr).curMode = 0;
    }
    
    if( (*stpr).whichMotor == LEFT ) {
        L_STP = (*stpr).curMode;
    }
    else if( (*stpr).whichMotor == RIGHT ){
        R_STP = (*stpr).curMode;
    }
}

void enableStepper(Stepper *stpr, uint16_t togPoint) {
    (*stpr).isEnabled = ENABLED;
    (*stpr).togglePoint = togPoint;
    (*stpr).curCount  = 0;
    (*stpr).numSteps  = 0;
    
    if( (*stpr).whichMotor == LEFT ) {
        L_ENB = ENABLED;
    }
    else if( (*stpr).whichMotor == RIGHT) {
        R_ENB = ENABLED;
    }
}

void disableStepper(Stepper *stpr) {
    (*stpr).isEnabled = DISABLED;
    (*stpr).togglePoint = 0;
    
    if( (*stpr).whichMotor == LEFT ) {
        L_ENB = DISABLED;
    }
    else if( (*stpr).whichMotor == RIGHT) {
        R_ENB = DISABLED;
    }
    tmr0_state(DISABLED);
}


void setDir(Stepper *stpr, uint8_t dir) {
    
    if( (*stpr).whichMotor == LEFT ) {
        L_DIR = dir;
    }
    else if( (*stpr).whichMotor == RIGHT ) {
        R_DIR = dir;
    } 
}
