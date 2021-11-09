/*
 * File name: Timer0.c
 *
 * Created on: 1 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description: ATmega16 timer0 driver source file
 *
 ********************************************************************************/

#include "common_macros.h"
#include "Timer0.h"
#include <avr/io.h> /* To use Timer0 Registers */
#include <avr/interrupt.h> /* For ISRs */
#include "gpio.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer0_Init(const Timer0_ConfigType * Config_Ptr)
{
	switch(Config_Ptr->Timer0_OpMode){

		case OVERFLOW:

			SREG  |= (1<<7);           // Enable global interrupts in MC.

			TCNT0 = Config_Ptr->Timer0_InitialValue; //Set Timer Initial value

			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt

			/* Configure the timer control register
			 * CS00 CS01 CS02 bits according to clock value
			 */
			TCCR0 = Config_Ptr->Timer0prescaler;

			/* Configure the timer control register
			* 1. Non PWM mode FOC0=1
			* 2. Normal Mode WGM01=0 & WGM00=0
			* 3. Normal Mode COM00=0 & COM01=0
			*/
			TCCR0 |= (1<<FOC0);
			break;

		case COMPARE:

			SREG  |= (1<<7);           // Enable global interrupts in MC.

			TCNT0 = Config_Ptr->Timer0_InitialValue; //Set Timer Initial value

			OCR0  = Config_Ptr->Timer0_CompareValue; // Set Compare Value

			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt

			/* Configure the timer control register
			 * CS00 CS01 CS02 bits according to clock value
			 */
			TCCR0 = Config_Ptr->Timer0prescaler;

			/* Configure timer control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 */
			TCCR0 |= (1<<FOC0) | (1<<WGM01);

			/* Configure timer control register
			 * Toggle OC0 on compare match COM00=1 & COM01=0
			 */
			//TCCR0 = (1<<COM00);

			break;

		case PWM:

			TCNT0 = Config_Ptr->Timer0_InitialValue; //Set Timer Initial value

			OCR0  = (uint8)(Config_Ptr->Timer0_CompareValue*255/100); // Set Compare Value

			GPIO_setupPinDirection( PORTB_ID, PIN3_ID, PIN_OUTPUT); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.

			/* Configure the timer control register
			 * CS00 CS01 CS02 bits according to clock value
			 */
			TCCR0 = Config_Ptr->Timer0prescaler;

			/* Configure timer control register
			 * 1. Fast PWM mode FOC0=0
			 * 2. Fast PWM Mode WGM01=1 & WGM00=1
			 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
			 */
			TCCR0 |= (1<<WGM00) | (1<<WGM01) | (1<<COM01);
			break;
	}


}


void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

void Timer0_DeInit(void)
{
	/* Clear All Timer0 Registers */
	TCCR0 = 0;
	TCNT0 = 0;
	OCR0  = 0;

	/* Disable the overflow and compare interrupts */
	TIMSK &= ~(1<<TOIE0);
	TIMSK &= ~(1<<OCIE0);
}




/* Description:
  * Initialize the PWM mode of ATmega16 timer 0 on output pin OC0(PB3).
  * INPUTS:  Duty cycle percentage of PWM of type uint8.
  */

