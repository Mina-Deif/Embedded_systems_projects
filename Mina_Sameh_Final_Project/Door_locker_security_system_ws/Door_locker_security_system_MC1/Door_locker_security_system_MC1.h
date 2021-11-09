/*
 * File name: Door_locker_security_system_MC1.h
 *
 * Created on: 2 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description : Header file for door locker security system HMI microcontroller.
 *
 *********************************************************************************************************/

#ifndef DOOR_LOCKER_SECURITY_SYSTEM_MC1_H_
#define DOOR_LOCKER_SECURITY_SYSTEM_MC1_H_

/* Handshaking definitions*/
#define MC1_READY 6
#define MC2_READY 7

/* MC1 functions */
#define SetNewPasswordFn	9
#define GetOptionsFn		1
#define EnterPasswordFn		2
#define SetFirstPasswordFn	5

/* MC2 functions */
#define ChangePasswordFn	3
#define OpenDoorFn			4


/* Defined for timer number of compares */
#define NUMBER_OF_COMPARE_MTACHES_PER_SECOND 31

/*******************************************************************************
 *                               Functions' prototypes                         *
 *******************************************************************************/

/* Description:
 * Function used for displaying the steps of changing the password on the LCD
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */
void ChangePassword (void);

/* Description:
 * Function used for:
 *  Getting the entered password from the keypad
 * 	Sending the entered password to MC2
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */
void GetPassword(void);

/* Description:
 * Function used for displaying the main menu on the LCD
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */
void GetOptions (void);

/* Description:
 * Function used for:
 *  Displaying the passwords entry screens
 *  Checking the returned decision from comparing both entries at MC2
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */
void SetNewPassword (void);

/* Description:
 * Function used for:
 * 	Receiving the decision returned from MC2
 * INPUTS:	N/A
 * OUTPUTS:
 * 		uint8 Decision: the returned decision value from MC2
 */
uint8 CheckDecision(void);

void InformMC2(uint8 SelectedFn);

/* Description:
 * Function used for:
 *  Interrupt Service Routine for timer0 compare mode
 *  Set g_FinshedCounting flag to 1 every 1 second
 * INPUTS:	N/A
 * OUTPUTS:	N/A
 */
void TickCounter(void);

/* Description:
 * Function used for:
 *  Initializing Timer0 with the desired values
 *  Loop until reaching the desired number of delayed seconds
 * INPUTS:
 * 		uint8 Seconds: Number of desired seconds to delay
 * OUTPUTS:	N/A
 */
void CountByTimer0(uint8 Seconds);


#endif /* DOOR_LOCKER_SECURITY_SYSTEM_MC1_H_ */
