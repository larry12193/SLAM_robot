/*
 * File:   main.c
 * Author: larry_000
 *
 * Created on November 4, 2016, 11:06 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p18f2550.h>
#include <stdint.h>
#include "uart.h"
#include "stepper_control.h"

#pragma config FOSC     = INTOSC_HS
#pragma config WDT      = OFF
#pragma config LVP      = OFF
#pragma config BOR      = OFF
#pragma config MCLRE    = ON
#pragma config PWRT     = ON
#pragma config PBADEN   = OFF

#define CW  0
#define CCW 1

volatile uint8_t timerOverflow = 0; // State of whether timer has overflown
volatile uint8_t receivedBytes = 0; // Flag to trigger read from UART RX buffer
char inChar;                        // UART received variable

// Stepper motor definitions
Stepper leftMotor;
Stepper rightMotor;

// prototypes
void delay(int d);                     // delay function
void interrupt_init(void);             // interrupt initialization
void processReceivedData(char data);  // Processes received data
void go(uint16_t numSteps, uint8_t dir);
void turn(float degrees, uint8_t dir);

int main(int argc, char** argv) {
    
    // System setup
    OSCCONbits.IRCF = 0x07;     // Set internal osc to 8MHz
    while(!OSCCONbits.IOFS);    // Wait for crystal to stablilize
    uart_init();                // Initialize UART port (9600 baud)
    tmr0_init();                // Initialize timer0 for software PWM
    ADCON1bits.PCFG = 0x0F;     // Setup all pins as digital I/O
    
    // Initialize motor definitions
    leftMotor = initializeMotor(LEFT);
    rightMotor = initializeMotor(RIGHT);
    
    interrupt_init();           // Initialize interrupts
    
    // Loop forever
    while(1) {
        
        // Handle timer overflow for motor step signal generation
        if( timerOverflow == 1 ) {
            processStepper(&leftMotor);
            processStepper(&rightMotor);
            timerOverflow = 0;
        }
        
        // Handle RX buffer full interrupt
        if( receivedBytes == 1 ) {
            inChar = uart_read();
            processReceivedData(inChar);
            receivedBytes = 0;
        }
    }
}

void delay(int d) {
    int i;
    while(d) {
        i = 100;
        while(i) {
            i--;
        }
        d--;
    }
}

// Initializes interrupts and enables them
void interrupt_init(void) {
    //RCONbits.IPEN      = 0x00;  // Disable interrupt priorities
    INTCONbits.TMR0IE  = 0x01;  // Enable timer0 overflow interrupt
    INTCON2bits.TMR0IP = 0x01;
    IPR1bits.RCIP      = 0x01;
    PIE1bits.RCIE      = 0x01;  // Enable RX receive interrupt
    
    INTCONbits.PEIE    = 0x01;  // Enable peripheral interrupts
    INTCONbits.GIEH    = 0x01;  // Enable global interrupts
    INTCONbits.GIEL    = 0x01;
}

void processReceivedData(char data) {
    switch(data) {
        case 119:   // w
            go(100, FORWARD);
            break;
        case 115:   // s
            go(100, BACKWARD);
            break;
        case 100:   //d
            turn(30.0, LEFT);
            break;
        case 97:    // a
            turn(30.0, RIGHT);
            break;
        case 120:   // x (stops motors)
            disableStepper(&leftMotor);
            disableStepper(&rightMotor);
            break;
    }
}

// Interrupt service routine
void interrupt isr(void) {
    
    // Disable global interrupts during interrupt handling
    INTCONbits.GIE = 0x00;
    
    // Timer0 overflow handling
    if( INTCONbits.TMR0IF == 1 ) {
        timerOverflow = 1;      // Set timer0 overflow process flag
        INTCONbits.TMR0IF = 0;  // Reset interrupt flag
    }
    
    // UART receive event handling
    if( INTCONbits.RBIF == 1 ) {
        receivedBytes = 1;      // Set RX bytes received process flag
        RCREG;                  // Reset RX interrupt flag
    }
    
    // Re-enable global interrupts
    INTCONbits.GIE = 0x01;
}

void go(uint16_t numSteps, uint8_t dir) {
    disableStepper(&leftMotor);
    disableStepper(&rightMotor);
    // Set both directions for requested motion
    setDir(&leftMotor, dir);
    setDir(&rightMotor, dir);
    
    // Set number of steps to move
    leftMotor.targSteps  = numSteps;
    rightMotor.targSteps = numSteps;
    
    // Enable motors 
    enableStepper(&leftMotor, 2);
    enableStepper(&rightMotor, 2);
    tmr0_state(ENABLED);
}

void turn(float degrees, uint8_t dir) {
    disableStepper(&leftMotor);
    disableStepper(&rightMotor);
    // Setup proper motor directions for intended turn direction
    if( dir == LEFT ) {
        setDir(&leftMotor,  BACKWARD);
        setDir(&rightMotor, FORWARD);
    }
    else {
        setDir(&leftMotor,  FORWARD);
        setDir(&rightMotor, BACKWARD);
    }
        
    // Set number of steps for degrees in turn
    leftMotor.targSteps  = (uint16_t)(degrees*DEGREE_PER_STEP);
    rightMotor.targSteps = (uint16_t)(degrees*DEGREE_PER_STEP);
    
    // Enable motors 
    enableStepper(&leftMotor, 2);
    enableStepper(&rightMotor, 2);
    tmr0_state(ENABLED);
}