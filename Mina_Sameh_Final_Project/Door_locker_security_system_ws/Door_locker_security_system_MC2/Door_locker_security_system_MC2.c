/*
 * File name: Door_locker_security_system_MC2.c
 *
 * Created on: 2 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description : Source file for door locker security system decision making microcontroller.
 *
 *********************************************************************************************************/

/* Define the microcontroller frequency as 8MHz */
#define F_CPU  8000000UL


#include <util/delay.h>

/* Include hardware abstraction layer drivers */
#include "motor.h"
#include "buzzer.h"
#include "uart.h"
#include "lcd.h"
#include "external_eeprom.h"
#include "twi.h"
#include "timer0.h"
#include "Door_locker_security_system_MC2.h"

/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

//uint8 First_Password_Flag=0;
//EEPROM_readByte( 0x0311 , &First_Password_Flag );

/* global variable contain the ticks count of the timer */
uint8 g_tick = 0;

/* global variable flag to indicate finish of counting desired number of seconds */
uint8 g_FinshedCounting = 0;

/* global variable flag to indicate the state of the password comparison */
uint8 g_PasswordCorrectFlag = 0;

int main()
{

	/* For debugging and showing that program works fine only */
	LCD_init();

	/* Initialize the TWI/I2C Driver with dynamic configuration */
	/*TWI_ConfigType TWI_Structure={P_1,FAST_MODE,0b00000010};*/
	/*TWI_init(&TWI_Structure);*/

	TWI_init();				/* Initialize the TWI/I2C Driver */
	DcMotor_Init();			/* Initialize DC motor driver*/
	Buzzer_Init();			/* Initialize buzzer driver*/

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec, 1 stop bit, disabled parity and 8 bit character */
	UART_ConfigType UART_Structure={9600,ONE_BIT,DISABLED,EIGHT_BIT};
	UART_Init(&UART_Structure);

	/*Array of characters to store the two received passwords, size is 16 due to LCD limit and 1 place for the null */
	uint8 Password_1[17];
	uint8 Password_2[17];


	while(1)
	{

		/*Receive the chosen function from MC1 */
		uint8 ChosenFunction;
		ChosenFunction = UART_recieveByte();

		switch(ChosenFunction)
		{
		case SetFirstPasswordFn:
			CheckForPreviouslySavedPassword(Password_1, Password_2);
			_delay_ms(500);
			break;

		case SetNewPasswordFn:
			ChangePassword(Password_1, Password_2);
			break;

		case GetOptionsFn:
			UserChoice(Password_1, Password_2);
			break;

		case EnterPasswordFn:
			CheckPassword(Password_1, Password_2);
			break;

		}


		/*For debugging and displaying that code works propperly only */
		LCD_displayStringRowColumn(0,0,Password_1);
		LCD_displayStringRowColumn(1,0,Password_2);
		LCD_displayStringRowColumn(0,8,"inEEPROM");
		LCD_displayStringRowColumn(1,8,"Debugger");
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for the main menu decisions made by the user.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is/will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is/will be saved
 *
 * OUTPUTS:N/A
 */

void UserChoice(uint8 * PassPtr1, uint8 * PassPtr2)
{
	uint8 choice;
	static uint8 FailureCounter=0;
	choice = UART_recieveByte();
	switch(choice){
	case '+':
		UART_sendByte(OpenDoorFn);	/* Inform MC1 about the selected choice*/
		UART_recieveByte(); //dummy
		CheckPassword(PassPtr1, PassPtr2);
		if(g_PasswordCorrectFlag)
		{
			FailureCounter = 0;
			OpenDoor();
		}
		else
		{
			FailureCounter++;
			if (FailureCounter==3)
			{
				Buzzer_ON();
				CountByTimer0(60);
				Buzzer_OFF();
				FailureCounter = 0;
			}
		}
		break;
	case '-':
		InformMC1(ChangePasswordFn);
		CheckPassword(PassPtr1, PassPtr2);
		if(g_PasswordCorrectFlag)
			{
				FailureCounter = 0;
				UART_recieveByte(); //dummy
				ChangePassword(PassPtr1, PassPtr2);
			}
			else
			{
				FailureCounter++;
				if (FailureCounter==3)
				{
					Buzzer_ON();
					CountByTimer0(60);
					Buzzer_OFF();
					FailureCounter = 0;
				}
			}
		break;
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Opening the door for 15 seconds
 *  waiting 3 seconds
 *  closing the door for 15 seconds
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */

void OpenDoor(void)
{
	DcMotor_Rotate (Clockwise,100);
	CountByTimer0(15);
	DcMotor_Rotate (OFF,0);
	CountByTimer0(10);
	DcMotor_Rotate (Counter_Clockwise,100);
	CountByTimer0(15);
	DcMotor_Rotate (OFF,0);
}
/********************************************************************************************************/

/* Description:
 * Function used to read the entered password from MC1 and save it in a string.
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the entered password will be saved
 *
 * OUTPUTS:N/A
 */

void ReadEnteredPassword(uint8 * PassPtr)
{
	uint8 RecievedCharacter, counter=0;

	UART_sendByte(MC2_READY);	/* Send MC2_READY byte to MC1 to ask it to send the string */
	UART_receiveString(PassPtr);
	_delay_ms(1000);

}
/********************************************************************************************************/

/* Description:
 * Function used to check if the two entered passwords are equal and if yes save them in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */


void SavePassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	if (!(strcmp(PassPtr1,PassPtr2))){

		UART_sendByte(TRUE);
		EEPROMStorePassword(PassPtr2);

	}
	else{
		UART_sendByte(FALSE);
	}
}
/********************************************************************************************************/

/* Description:
 * Function used to read and store the two entered passwords and save them.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */

void ChangePassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	//UART_sendByte(ChangePasswordFn);	/* Inform MC1 about the selected choice*/
	// error function decision byte read as password character ====================================
	ReadEnteredPassword(PassPtr1);
	ReadEnteredPassword(PassPtr2);

	SavePassword(PassPtr1, PassPtr2);

}
/********************************************************************************************************/

/* Description:
 * Function used to compare the two passwords and set the g_PasswordCorrectFlag accordingly and inform MC1 of the decision.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */

void CheckPassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	EEPROMRetrivePassword(PassPtr1);
	ReadEnteredPassword(PassPtr2);
	if (!(strcmp(PassPtr1,PassPtr2))){
		UART_sendByte(TRUE);
		g_PasswordCorrectFlag=1;
	}
	else{
		UART_sendByte(FALSE);
		g_PasswordCorrectFlag=0;
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for checking the flag in the EEPROM to determine if previous password is saved at first use
 * Used after power cuts to prevent creating a new password

 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password will be saved
 *
 * OUTPUTS:	N/A
 */

void CheckForPreviouslySavedPassword(uint8 * PassPtr1, uint8 * PassPtr2)
{
	uint8 FirstSystemPassword_flag;
	EEPROM_readByte( 0x311, &FirstSystemPassword_flag ); /* Read current character in the external EEPROM*/
	if (FirstSystemPassword_flag)
	{
		EEPROMRetrivePassword(PassPtr1);
		UART_sendByte(TRUE);
	}
	else
	{
	EEPROM_writeByte( 0x311 , 1); /* Write current character in the external EEPROM */
	_delay_ms(10);
	UART_sendByte(FALSE);
	ChangePassword(PassPtr1, PassPtr2);
	}
}
/********************************************************************************************************/
void InformMC1(uint8 SelectedFn)
{
	UART_sendByte(SelectedFn);
}

/********************************************************************************************************/

/* Description:
 * Function used for saving the new password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password is saved
 *
 * OUTPUTS:	N/A
 */

void EEPROMStorePassword(uint8 * PassPtr)
{
	uint8 Character ,Counter= 0;
	uint16 FirstAddress = 0x0312;
	do
	{
		Character = PassPtr[Counter];
		EEPROM_writeByte( FirstAddress + Counter , Character); /* Write current character in the external EEPROM */
		_delay_ms(10);
		Counter++;
	}
	while(Character != NULL_PTR);
}
/********************************************************************************************************/

/* Description:
 * Function used for reading the saved password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password will be saved
 *
 * OUTPUTS:	N/A
 */

void EEPROMRetrivePassword(uint8 * PassPtr)
{
	uint8 Counter= 0;
	uint16 FirstAddress = 0x0312;
	do
	{
	//_delay_ms(10);
	EEPROM_readByte( FirstAddress + Counter , PassPtr + Counter ); /* Write current character in the external EEPROM */
	Counter++;
	}
	while(PassPtr[Counter-1] != NULL_PTR);
	//PassPtr[Counter] = NULL_PTR;
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Interrupt Service Routine for timer0 compare mode
 *  Set g_FinshedCounting flag to 1 every 1 second
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */

void TickCounter(void)
{
	g_tick++;
	if(g_tick == NUMBER_OF_COMPARE_MTACHES_PER_SECOND)
	{
		g_FinshedCounting=1;
		g_tick = 0; //clear the tick counter again to count a new 0.5 second
	}
}
/********************************************************************************************************/

/* Description:
 * Function used for:
 *  Initializing Timer0 with the desired values
 *  Loop until reaching the desired number of delayed seconds
 *
 * INPUTS:
 * 		uint8 Seconds: Number of desired seconds to delay
 *
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

		if(g_FinshedCounting==1)
		{
			SecondsCounter++;
			g_FinshedCounting=0;
		}

		if(SecondsCounter==Seconds)
		{
			SecondsCounter=0;
			Timer0_DeInit();
			return;
		}
    }
}

