/*
 * miniProject2.c
 *
 *  Created on: Sep 18, 2021
 *  Author: Mina Sameh
 *  Description: Stop watch system
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Time counter variables
unsigned char seconds_units	= 0;
unsigned char seconds_tenth	= 0;
unsigned char minutes_units	= 0;
unsigned char minutes_tenth	= 0;
unsigned char hours_units	= 0;
unsigned char hours_tenth	= 0;

//=========================================================================================

ISR(TIMER1_COMPA_vect)
{
	//Increment timer counters
	seconds_units++;
	if (seconds_units == 10) {
		seconds_tenth++;
		seconds_units=0;
	}
	if (seconds_tenth == 6) {
			minutes_units++;
			seconds_tenth=0;
	}
	if (minutes_units == 10) {
				minutes_tenth++;
				minutes_units=0;
	}
	if (minutes_tenth == 6) {
				hours_units++;
				minutes_tenth=0;
	}
	if (hours_units == 10) {
				hours_tenth++;
				hours_units=0;
	}
	if (hours_tenth == 10) {
				hours_tenth=0;
	}
}
//==========================================================================================

/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	// reset time
	seconds_units	= 0;
	seconds_tenth	= 0;
	minutes_units	= 0;
	minutes_tenth	= 0;
	hours_units	= 0;
	hours_tenth	= 0;
	TCNT1		 = 0;    			// Set Timer initial value to 0
}
//==========================================================================================

/* External INT0 enable and configuration function */
void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));               		// Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2);     				// Enable the internal pull up resistor at PD2 pin
	GICR  |= (1<<INT0);              		// Enable external interrupt pin INT0
	// Trigger INT0 with the falling edge
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
}
//==========================================================================================

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	// Pause time counting
	//TIMSK &= ~(1<<OCIE1A); 			// Disable Timer1 Compare A Interrupt
	TCCR1B &= ~(1<<CS10 | (1<<CS12));		// Disable Timer1 by setting clock to zero
}
//==========================================================================================

/* External INT1 enable and configuration function */
void INT1_Init(void)
{
	DDRD  &= (~(1<<PD3));				// Configure INT1/PD3 as input pin
	GICR  |= (1<<INT1);				// Enable external interrupt pin INT1
	MCUCR |= (1<<ISC11) | (1<<ISC10);		// Trigger INT1 with the rising edge
}
//==========================================================================================

/* External INT2 Interrupt Service Routine */
ISR(INT2_vect)
{
	// Resume time counting
	//TIMSK |= (1<<OCIE1A); 			// Enable Timer1 Compare A Interrupt
	TCCR1B |= (1<<CS10) | (1<<CS12);		// Enable Timer1 by setting back the prescaler to 1024
}
//==========================================================================================

/* External INT2 enable and configuration function */
void INT2_Init(void)
{
	DDRB   &= (~(1<<PB2));				// Configure INT2/PB2 as input pin
	PORTB  |= (1<<PB2);				// Enable the internal pull up resistor at PB2 pin
	GICR   |= (1<<INT2);				// Enable external interrupt pin INT2
	MCUCSR &= ~(1<<ISC2);				// Trigger INT2 with the falling edge
}
//==========================================================================================

void Timer1_Init_CTC_Mode(void)
{
	TCNT1 = 0;    					// Set Timer initial value to 0
	OCR1A  = 976; 					// Set Compare Value to 976 to trigger interrupt every 1 sec as each compare match at 1.024 msec
	TIMSK |= (1<<OCIE1A);	 			// Enable Timer1 Compare A Interrupt

	/* Configure timer control register TCCR1A
	     * 1. Disconnect OC1A  COM1A1=0 COM1A0=0
	     * 2. Disconnect OC1B  COM1B0=0 COM1B1=0
	     * 3. FOC1A=1 FOC1B=1 for non-pwm mode
	     * 4. CTC1 Mode WGM10=0 & WGM11=0 (Mode Number 4)
	*/
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	/* Configure timer control register TCCR1B
		 * 1. CTC1 Mode WGM12=1 & WGM13=0 (Mode Number 4)
		 * 2. CS10=1 CS12=1 for prescaler = F_CPU/1024 to get interrupt every 1ms
	*/
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
}
//==========================================================================================

void Display(void){

	PORTA 	= 0x0001;
	PORTC = (PORTC & 0xF0) | (seconds_units & 0x0F);
	_delay_ms(3);

	PORTA 	= 0x0002;
	PORTC = (PORTC & 0xF0) | (seconds_tenth & 0x0F);
	_delay_ms(3);

	PORTA 	= 0x0004;
	PORTC = (PORTC & 0xF0) | (minutes_units & 0x0F);
	_delay_ms(3);

	PORTA 	= 0x0008;
	PORTC = (PORTC & 0xF0) | (minutes_tenth & 0x0F);
	_delay_ms(3);

	PORTA 	= 0x0010;
	PORTC = (PORTC & 0xF0) | (hours_units & 0x0F);
	_delay_ms(3);

	PORTA 	= 0x0020;
	PORTC = (PORTC & 0xF0) | (hours_tenth & 0x0F);
	_delay_ms(3);
}
//==========================================================================================

int main(void)
{
	SREG  |= (1<<7);			// Enable global interrupts in MC.
	INT0_Init();				// Initialize interrupt 0
	INT1_Init();				// Initialize interrupt 1
	INT2_Init();				// Initialize interrupt 2
	Timer1_Init_CTC_Mode();			// Initialize timer1
	DDRA	|= 	0x003F;			// set first 6 pins of port A as output
	PORTA 	&= 	0xC0;			// write zero on first 6 pins of port A
	DDRC 	|=	0x000F;			// set first 4 pins of port C as output
	PORTC 	&=	0xF0;			// write zero on first 4 pins of port C
	SFIOR	&=	~(1<<PUD);		// Enable internal pull-up

    while(1)
    {
    	Display();
    }
}
