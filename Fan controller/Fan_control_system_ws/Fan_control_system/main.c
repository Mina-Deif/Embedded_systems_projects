/*
 * main.c
 *
 *  	Created on: 12 Oct 2021
 *      Author: Mina Sameh
 *      Description : Source file for fan controller system based on temperature value
 */

/* Include hardware abstraction layer drivers */
#include "lcd.h"
#include "lm35_sensor.h"
#include "motor.h"




int main(){


	LCD_init();			/* Initialize LCD driver*/
	LM35_init();		/* Initialize LM35 sensor driver*/
	DcMotor_Init();		/* Initialize DC motor driver*/

	uint8 temperature;		/* to store the temperature value to run the control algorithm*/
	const char *fan_state;
	/* Print the permanent strings*/
	LCD_displayStringRowColumn(0,4,"Fan is ");
	LCD_displayStringRowColumn(1,4,"Temp = ");

	while(1){

		temperature=LM35_getTemperature();	/* Read sensor reading*/

		/*
		 * If the temperature is less than 30C turn off the fan.
		 * If the temperature is greater than or equal 30C turn on the fan with 25% of its maximum speed.
		 * If the temperature is greater than or equal 60C turn on the fan with 50% of its maximum speed.
		 * If the temperature is greater than or equal 90C turn on the fan with 75% of its maximum speed.
		 * If the temperature is greater than or equal 120C turn on the fan with 100% of its maximum speed
		 */

		if (temperature<30){
			DcMotor_Rotate (OFF,0);
			fan_state = "OFF";
		}
		else if (temperature>=30 && temperature<60){
			DcMotor_Rotate (Counter_Clockwise,25);
			fan_state = "ON ";
		}
		else if (temperature>=60 && temperature<90){
			DcMotor_Rotate (Counter_Clockwise,50);
			fan_state = "ON ";
		}
		else if (temperature>=90 && temperature<120){
				DcMotor_Rotate (Counter_Clockwise,75);
				fan_state = "ON ";
			}
		else if (temperature>=120){
			DcMotor_Rotate (Counter_Clockwise,100);
			fan_state = "ON ";
		}

		/* Display fan state on LCD*/
		LCD_displayStringRowColumn(0,11,fan_state);
		LCD_moveCursor(1,11);

		/* Conditions to control Displayed number of characters and blanks on LCD */
		if (temperature>=0 && temperature<10){
					LCD_intgerToString(temperature);
					LCD_displayStringRowColumn(1,12," ");
				}
		else if (temperature>=10 && temperature<100){
			LCD_intgerToString(temperature);
			LCD_displayStringRowColumn(1,13," ");
		}
		else if (temperature>=100){
			LCD_intgerToString(temperature);
		}
	}
}
