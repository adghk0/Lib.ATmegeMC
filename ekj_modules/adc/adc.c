/*
 * adc.c
 *
 * Created: 2021-08-22 오후 5:17:39
 *  Author: ekj
 */ 

#include "adc.h"

unsigned int buf = 0;
unsigned char finish = 0;

ISR(ADC_vect)
{
	buf = ADCW;
	finish = 1;
}

// initializing ADC
void adc_init(unsigned char adlar, unsigned char ref, unsigned char prescaler)
{
	// ADC Enable, ADC Interrupt Enable, ADC Prescaler 128
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (prescaler<<ADPS0);
	// set reference voltage and left adjust result bit
	ADMUX = (ref<<REFS0) | (adlar<<ADLAR);
}

// request starting conversion
void adc_request(unsigned char channel)
{
	finish = 0;
	
	// set channel
	ADMUX = (ADMUX & 0xf0) | channel;
	
	// start conversion
	ADCSRA |= (1<<ADSC);
}

// can read the data
// Returns : exist(1), no-exist(0)
char adc_available()
{
	return finish;
}

// read data
// Returns : data(16bit)
int adc_read()
{
	finish = 1;
	return buf;
}