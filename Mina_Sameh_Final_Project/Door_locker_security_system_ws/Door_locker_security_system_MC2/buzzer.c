/*
 * File name: buzzer.c
 *
 * Created on: 1 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description: Buzzer driver source file
 *
 ******************************************************************************/

#include "common_macros.h"
#include "std_types.h"
#include "buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

 /*
 * Description : Function to initialize the buzzer driver
 * 	1. Set the buzzer pin as output pin.
 * 	2. Write logical low on the buzzer pin.
 */
void Buzzer_Init(void)
{
	GPIO_setupPinDirection(Buzzer_PORT_ID, Buzzer_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);

}

 /*
 * Description : Function to turn the buzzer on:
 * 	1. write logical high on the buzzer pin.
 */
void Buzzer_ON(void)
{
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_HIGH);
}

 /*
 * Description : Function to turn the buzzer off:
 * 	1. write logical low on the buzzer pin.
 */
void Buzzer_OFF(void)
{
	GPIO_writePin(Buzzer_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);
}
