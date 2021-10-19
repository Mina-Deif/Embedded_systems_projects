/*
 * motor.h
 *
 *  Created on: 12 Oct 2021
 *	Description : header file for DC motor driver
 *  Author: Mina Sameh
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "gpio.h"
#include "PWM.h"

#define Motor_Direction_PORT_ID    PORTB_ID
#define Motor_Direction_PIN1_ID    PIN1_ID
#define Motor_Direction_PIN2_ID    PIN2_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	OFF, Clockwise, Counter_Clockwise
}DcMotor_State;

/*******************************************************************************
 *                               Functions' prototypes                         *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC Motor:
 * 1. Setup the input pins of motor L293D H-bridge directions using the GPIO driver.
 * 2. Initialize the output value of the input pins of motor driver by zero.
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for setting motor rotation direction using GPIO driver
 * And setting rotation speed using PWM driver.
 * INPUTS:  Rotation direction of type uint8.
 * 			Rotation speed of type uint8
 */
void DcMotor_Rotate (DcMotor_State state,uint8 speed);

#endif /* MOTOR_H_ */
