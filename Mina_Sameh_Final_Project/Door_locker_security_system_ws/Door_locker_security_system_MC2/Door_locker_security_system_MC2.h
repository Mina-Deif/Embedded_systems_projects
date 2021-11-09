/*
 * File name: Door_locker_security_system_MC2.h
 *
 * Created on: 2 Nov 2021
 *
 * Author: Mina Sameh
 *
 * Description : header file for door locker security system decision making microcontroller.
 *
 *********************************************************************************************************/

#ifndef DOOR_LOCKER_SECURITY_SYSTEM_MC2_H_
#define DOOR_LOCKER_SECURITY_SYSTEM_MC2_H_

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
 * Function used for the main menu decisions made by the user.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is/will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is/will be saved
 *
 * OUTPUTS:N/A
 */
void UserChoice(uint8 * PassPtr1, uint8 * PassPtr2);

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
void OpenDoor(void);

/* Description:
 * Function used to read the entered password from MC1 and save it in a string.
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the entered password will be saved
 *
 * OUTPUTS:N/A
 */
void ReadEnteredPassword(uint8 * PassPtr);

/* Description:
 * Function used to check if the two entered passwords are equal and if yes save them in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */

void SavePassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used to read and store the two entered passwords and save them.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */
void ChangePassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used to compare the two passwords and set the g_PasswordCorrectFlag accordingly and inform MC1 of the decision.
 *
 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password is saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password is saved
 *
 * OUTPUTS:N/A
 */
void CheckPassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used for checking the flag in the EEPROM to determine if previous password is saved at first use
 * Used after power cuts to prevent creating a new password

 * INPUTS:
 * 		uint8 * PassPtr1: pointer to the string where the first entered password will be saved
 * 		uint8 * PassPtr2: pointer to the string where the second entered password will be saved
 *
 * OUTPUTS:	N/A
 */
void CheckForPreviouslySavedPassword(uint8 * PassPtr1, uint8 * PassPtr2);

/* Description:
 * Function used for saving the new password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password is saved
 *
 * OUTPUTS:	N/A
 */
void EEPROMStorePassword(uint8 * PassPtr);

/* Description:
 * Function used for reading the saved password in the EEPROM
 *
 * INPUTS:
 * 		uint8 * PassPtr: pointer to the string where the password will be saved
 *
 * OUTPUTS:	N/A
 */
void EEPROMRetrivePassword(uint8 * PassPtr);

/* Description:
 * Function used for:
 *  Interrupt Service Routine for timer0 compare mode
 *  Set g_FinshedCounting flag to 1 every 1 second
 *
 * INPUTS:	N/A
 *
 * OUTPUTS:	N/A
 */
void TickCounter(void);

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
void CountByTimer0(uint8 Seconds);

#endif /* DOOR_LOCKER_SECURITY_SYSTEM_MC2_H_ */
