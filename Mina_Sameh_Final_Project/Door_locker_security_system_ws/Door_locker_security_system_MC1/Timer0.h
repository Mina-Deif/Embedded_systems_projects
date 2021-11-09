/*
 * File name: Timer0.h
 *
 * Created on: 1 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description: ATmega16 timer0 driver header file
 *
 *******************************************************************************/

#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"

typedef enum
{
	OVERFLOW, COMPARE, PWM
}Timer0_OperationMode;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer0_Prescaler;

/* structure for dynamic configuration of Timer0 operation mode, prescaler, initial value and compare value*/
typedef struct{
	Timer0_OperationMode Timer0_OpMode;
	Timer0_Prescaler Timer0prescaler;
	uint8 Timer0_InitialValue;
	uint8 Timer0_CompareValue;	/*also used as input for the Duty Cycle Percentage in case of PWM*/
}Timer0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Timer0_Init(const Timer0_ConfigType * Config_Ptr);

void Timer0_setCallBack(void(*a_ptr)(void));

void Timer0_DeInit(void);

#endif /* TIMER0_H_ */








