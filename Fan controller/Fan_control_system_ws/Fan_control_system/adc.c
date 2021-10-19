 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ATmega16 ADC driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "avr/io.h" /* To use the ADC Registers */
#include "adc.h"
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType * Config_Ptr)
{
	ADMUX =0;  /* clear ADMUX register for new initialization*/

	/* ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */
	/* cases for dynamic configuration of reference voltage*/
	switch(Config_Ptr->ADCref_Voltage){
		case AREF:
			ADMUX &= ~((1<<REFS0)|(1<<REFS1));
			break;
		case AVCC:
			ADMUX &= ~(1<<REFS1);
			ADMUX |=  (1<<REFS0);
			break;
		case Internal_2:
			ADMUX &= ~(1<<REFS1);
			ADMUX |=  (1<<REFS0);
			break;
	}

	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADATE   = 0 Disable Auto Trigger
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	ADCSRA = 0;			/* clear ADCSRA register for new initialization*/
	ADCSRA = (1<<ADEN);
	/* cases for dynamic configuration of prescaler*/
	switch(Config_Ptr->ADCprescaler){
		case FCPU_by_2:
			SET_BIT(ADCSRA,ADPS0);
			break;
		case FCPU_by_4:
			SET_BIT(ADCSRA,ADPS1);
			break;
		case FCPU_by_8:
			SET_BIT(ADCSRA,ADPS0);
			SET_BIT(ADCSRA,ADPS1);
			break;
		case FCPU_by_16:
			SET_BIT(ADCSRA,ADPS2);
			break;
		case FCPU_by_32:
			SET_BIT(ADCSRA,ADPS0);
			SET_BIT(ADCSRA,ADPS2);
			break;
		case FCPU_by_64:
			SET_BIT(ADCSRA,ADPS1);
			SET_BIT(ADCSRA,ADPS2);
			break;
		case FCPU_by_128:
			SET_BIT(ADCSRA,ADPS0);
			SET_BIT(ADCSRA,ADPS1);
			SET_BIT(ADCSRA,ADPS2);
			break;
	}
}

uint16 ADC_readChannel(uint8 channel_num)
{
	channel_num &= 0x07; /* Input channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* Clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* Choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* Wait for conversion to complete, ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF); /* Clear ADIF by write '1' to it :) */
	return ADC; /* Read the digital value from the data register */
}
