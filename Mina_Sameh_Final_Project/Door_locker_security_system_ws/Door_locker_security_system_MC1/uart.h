 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	ONE_BIT, TWO_BITS=8
}UART_StopBitsENUM;

typedef enum
{
	FIVE_BIT, SIX_BIT=2, SEVEN_BIT=4, EIGHT_BIT=6, NINE_BIT
}UART_CharacterSizeENUM;

typedef enum
{
	DISABLED, EVEN_PARITY=32, ODD_PARITY=48
}UART_ParityENUM;

/* structure for dynamic configuration of UART operation mode, prescaler, initial value and compare value*/
typedef struct{
	uint32					UART_BaudRate;
	UART_StopBitsENUM 		UART_StopBits;
	UART_ParityENUM			UART_Parity;
	UART_CharacterSizeENUM  UART_CharacterSize;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_Init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
