/*
 * PWM.c
 *
 *  Created on: 12 Oct 2021
 *  Description : Source file for for the ATmega16 PWM driver
 *  Author: Mina Sameh
 */



#include "std_types.h"
#include "PWM.h"

/* Description:
  * Initialize the PWM mode of ATmega16 timer 0 on output pin OC0(PB3).
  * INPUTS:  Duty cycle percentage of PWM of type uint8.
  */

void PWM_Timer0_Init(uint8 duty_cycle_precentage)
{

	TCNT0 = 0; //Set Timer Initial value

	OCR0  = (uint8)(duty_cycle_precentage*255/100); // Set Compare Value

	DDRB  = DDRB | (1<<PB3); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
