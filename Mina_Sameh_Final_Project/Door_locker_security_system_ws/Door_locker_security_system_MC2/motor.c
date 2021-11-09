/*
 * motor.c
 *
 *  Created on: 12 Oct 2021
 *  Description : Source file for DC motor driver
 *  Author: Mina Sameh
 */
#include "common_macros.h"
#include "std_types.h"
#include "motor.h"

/*
 * Description :
 * Initialize the DC Motor:
 * 1. Setup the input pins of motor L293D H-bridge directions using the GPIO driver.
 * 2. Initialize the output value of the input pins of motor driver by zero.
 */
void DcMotor_Init(void){
	GPIO_setupPinDirection(Motor_Direction_PORT_ID, Motor_Direction_PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(Motor_Direction_PORT_ID, Motor_Direction_PIN2_ID, PIN_OUTPUT);
	GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN1_ID, LOGIC_LOW);
	GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN2_ID, LOGIC_LOW);
#if (PWM_REQUIRED ==0)
	GPIO_setupPinDirection(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID	, PIN_OUTPUT);
	GPIO_writePin(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID	, LOGIC_LOW);
#endif
}

/*
 * Description :
 * Function responsible for setting motor rotation direction using GPIO driver
 * And setting rotation speed using PWM driver.
 * INPUTS:  Rotation direction of type uint8.
 * 			Rotation speed of type uint8
 */
void DcMotor_Rotate (DcMotor_State state,uint8 speed){
	switch(state){
	case OFF:
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN2_ID, LOGIC_LOW);
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID,     LOGIC_LOW);
		break;
	case Clockwise:
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN2_ID, LOGIC_LOW);

#if (PWM_REQUIRED ==0)
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID	, LOGIC_HIGH);
#endif

#if (PWM_REQUIRED ==1)
		Timer0_DeInit();
		Timer0_ConfigType Timer0_Structure={PWM,F_CPU_1024,0,speed};
		Timer0_Init(&Timer0_Structure);

#endif
		break;
	case Counter_Clockwise:
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN2_ID, LOGIC_HIGH);


#if (PWM_REQUIRED ==0)
		GPIO_writePin(Motor_Direction_PORT_ID, PIN3_ID, LOGIC_HIGH);
#endif

#if (PWM_REQUIRED ==1)
		Timer0_DeInit();
		Timer0_ConfigType Timer0_Structure={PWM,F_CPU_1024,0,speed};
		Timer0_Init(&Timer0_Structure);

#endif
		break;
	default:
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Direction_PIN2_ID, LOGIC_LOW);
		GPIO_writePin(Motor_Direction_PORT_ID, Motor_Enable_PIN_ID,     LOGIC_LOW);
	}

}
