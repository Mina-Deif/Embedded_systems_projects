/*
 * File name: Door_locker_security_system_MC1.c
 *
 * Created on: 2 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description : Source file for door locker security system HMI microcontroller.
 *
 *********************************************************************************************************/

/* Define the microcontroller frequency as 8MHz */
#define F_CPU  8000000UL

#include <util/delay.h>

/* Include hardware abstraction layer drivers */
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer0.h"
#include "Door_locker_security_system_MC1.h"


/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

/* global variable contain the ticks count of the timer */
uint8 g_tick = 0;

/* global variable flag to indicate finish of counting desired number of seconds */
uint8 g_FinshedCounting = 0;

int main(){


	LCD_init();			/* Initialize LCD driver*/

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec, 1 stop bit, disabled parity and 8 bit character */
	UART_ConfigType UART_Structure={9600,ONE_BIT,DISABLED,EIGHT_BIT};
	UART_Init(&UART_Structure);


	uint8 Decision;		/*Variable to store the received decision from MC2 */

	/*To set a password for the system at first use */
	UART_sendByte(SetFirstPasswordFn);
	Decision = UART_recieveByte();
	if (Decision)
	{

	}
	else{
	SetNewPassword ();
	}


	while(1){
		GetOptions();
		Decision = UART_recieveByte();
		switch (Decision){
		case OpenDoorFn:

			InformMC2(EnterPasswordFn);
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Enter password:");
			GetPassword();

			if(CheckDecision())
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Opening the door");
				CountByTimer0(33);
			}
			else
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Wrong password");
				_delay_ms(1000);
			}
			break;

		case ChangePasswordFn:
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Enter password:");
			GetPassword();
			if(CheckDecision())
			{
				SetNewPassword ();
			}
			else
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Wrong password");
				_delay_ms(1000);
			}
		}

	}
}
/********************************************************************************************************/

/* Description:
 * Function used for displaying the steps of changing the password on the LCD
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */

void ChangePassword (void)
{

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Enter new pass:");
	GetPassword();
	_delay_ms(500);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Renter new pass:");
	GetPassword();

}
/********************************************************************************************************/


/* Description:
 * Function used for:
 *  Getting the entered password from the keypad
 * 	Sending the entered password to MC2
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */

void GetPassword(void)
{
	uint8 Password[17];
	uint8 key;
	uint8 counter = 0;
	do
	{
		key = KEYPAD_getPressedKey();

		if((key >= 0) && (key <= 9))
		{
			Password[counter]=key+48;
			LCD_moveCursor(1,counter);
			LCD_intgerToString(key);
			counter++;
			_delay_ms(500);
		}
	}
	while(key != '=');
	Password[counter]= '#';
	Password[counter+1]= NULL_PTR;

	/* Wait until MC2 is ready to receive the string */
	while(UART_recieveByte() != MC2_READY){}
	UART_sendString(Password);
}
/********************************************************************************************************/

/* Description:
 * Function used for displaying the main menu on the LCD
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */

void GetOptions (void)
{
	UART_sendByte(GetOptionsFn);	/* Inform MC2 of selected function */

	uint8 key;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open door");
	LCD_displayStringRowColumn(1,0,"- : Change pass");
	do{
	key = KEYPAD_getPressedKey();
	if((key == '+') || (key == '-'))
	{

		UART_sendByte(key);
	}
	}while((key != '+') && (key != '-'));
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Displaying the passwords entry screens
 *  Checking the returned decision from comparing both entries at MC2
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */

void SetNewPassword (void)
{
	uint8 Decision;
	do{
		InformMC2(SetNewPasswordFn);
		ChangePassword();
		Decision = CheckDecision();
		if (!Decision){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Error: mismatch");
			_delay_ms(1000);
		}
	}while(!Decision );
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 * 	Receiving the decision returned from MC2
 * INPUTS:	N/A
 * OUTPUTS:
 * 		uint8 Decision: the returned decision value from MC2
 */


uint8 CheckDecision(void)
{
	uint8 Decision;
	Decision = UART_recieveByte();
	return Decision;
}
/********************************************************************************************************/

void InformMC2(uint8 SelectedFn)
{
	UART_sendByte(SelectedFn);
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Interrupt Service Routine for timer0 compare mode
 *  Set g_FinshedCounting flag to 1 every 1 second
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */

void TickCounter(void)
{
	g_tick++;
	if(g_tick == NUMBER_OF_COMPARE_MTACHES_PER_SECOND)
	{
		g_FinshedCounting=1;
		g_tick = 0; //clear the tick counter again to count a new second
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Initializing Timer0 with the desired values
 *  Loop until reaching the desired number of delayed seconds
 * INPUTS:
 * 		uint8 Seconds: Number of desired seconds to delay
 * OUTPUTS:	N/A
 */
void CountByTimer0(uint8 Seconds)
{
	Timer0_DeInit();
	Timer0_ConfigType Timer0_Structure={COMPARE,F_CPU_1024,0,250};
	Timer0_Init(&Timer0_Structure);
	Timer0_setCallBack(TickCounter);
	uint8 SecondsCounter=0;
	while(1)
    {

		if(g_FinshedCounting==1){
			SecondsCounter++;
			g_FinshedCounting=0;
		}

		if(SecondsCounter==Seconds){
			SecondsCounter=0;
			Timer0_DeInit();
			return;
		}
    }
}
