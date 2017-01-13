/* 
 * File:   pinout.h
 * Author: larry_000
 *
 * Created on January 9, 2017, 3:28 PM
 */

#ifndef PINOUT_H
#define	PINOUT_H

#ifdef	__cplusplus
extern "C" {
#endif

#define L_DIR_TRIS TRISA0   // Left motor direction toggle pin port
#define L_STP_TRIS TRISA1   // Left motor step pin port
#define L_ENB_TRIS TRISA2   // Left motor controller enable pin port
    
#define R_DIR_TRIS TRISA3   // Right motor direction toggle pin port
#define R_STP_TRIS TRISA4   // Right motor step pin port
#define R_ENB_TRIS TRISA5   // Right motor controller enable pin port
    
#define L_DIR RA0           // Left motor direction toggle pin
#define L_STP RA1           // Left motor step pin
#define L_ENB RA2           // Left motor controller enable pin
    
#define R_DIR RA3           // Right motor direction toggle pin
#define R_STP RA4           // Right motor step pin
#define R_ENB RA5           // Right motor controller enable pin

#ifdef	__cplusplus
}
#endif

#endif	/* PINOUT_H */

