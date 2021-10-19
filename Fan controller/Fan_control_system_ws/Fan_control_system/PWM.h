/*
 * PWM.h
 *
 *  Created on: 12 Oct 2021
 *  Description : header file for for the ATmega16 PWM driver
 *  Author: Mina Sameh
 */

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

/*******************************************************************************
 *                               Functions' prototypes                         *
 *******************************************************************************/

/* Description:
  * Initialize the PWM mode of ATmega16 timer 0 on output pin OC0(PB3).
  * INPUTS:  Duty cycle percentage of PWM of type uint8.
  */
void PWM_Timer0_Init(uint8 duty_cycle_precentage);

#endif /* PWM_H_ */
