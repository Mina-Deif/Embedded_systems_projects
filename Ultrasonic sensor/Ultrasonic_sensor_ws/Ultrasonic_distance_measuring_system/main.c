/*
 * File name: main.c
 *
 *  Created on: 12 Oct 2021
 *
 *  Author: Mina Sameh
 *
 *  Description: Source file for the distance measuring system using an ultrasonic sensor
 */


/* Include the hardware abstraction layer drivers */
#include "lcd.h"
#include "ultrasonic.h"


int main(){

	/* Used for storing the measured distance */
	uint16 distance =0;
	/* Initialize the lCD driver */
	LCD_init();
	/* Initialize the ultrasonic sensor driver */
	Ultrasonic_init();


	/* Display the permanent strings */
	LCD_displayStringRowColumn(0,0,"Distance =  ");
	LCD_displayStringRowColumn(0,14,"cm");

	while(1){
		/* Read measured distance from ultrasonic sensor */
		distance = Ultrasonic_readDistance();

		/*
		 * Display the distance on the LCD
		 * with conditions to control number of displayed characters and blanks on LCD to always cover 3 places*/
		LCD_moveCursor(0,11);
		if (distance>=0 && distance<10){
			LCD_intgerToString(distance);
			LCD_displayStringRowColumn(0,12," ");
		}
		else if (distance>=10 && distance<100){
			LCD_intgerToString(distance);
			LCD_displayStringRowColumn(0,13," ");
		}
		else if (distance>=100){
			LCD_intgerToString(distance);
		}

	}
}
