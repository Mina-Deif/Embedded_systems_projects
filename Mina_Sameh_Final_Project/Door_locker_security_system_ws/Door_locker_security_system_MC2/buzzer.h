/*
 * File name: buzzer.h
 *
 * Created on: 1 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description: Buzzer driver header file.
 *
 ********************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define Buzzer_PORT_ID                 PORTB_ID
#define Buzzer_PIN_ID                  PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

 /*
 * Description : Function to initialize the buzzer driver
 * 	1. Set the buzzer pin as output pin.
 * 	2. Write logical low on the buzzer pin.
 */
void Buzzer_Init(void);

 /*
 * Description : Function to turn the buzzer on:
 * 	1. write logical high on the buzzer pin.
 */
void Buzzer_ON(void);

 /*
 * Description : Function to turn the buzzer off:
 * 	1. write logical low on the buzzer pin.
 */
void Buzzer_OFF(void);

#endif /* BUZZER_H_ */
