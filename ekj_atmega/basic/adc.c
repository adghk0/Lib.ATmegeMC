/*
 * adc.c
 *
 * Created: 2021-08-22 오후 5:17:39
 *  Author: ekj
 */ 

#include "adc.h"

adc_info *adc_for;
char adc_on;
char adc_ready;

ISR(ADC_vect)
{
	adc_for->result = ADCW;
	adc_for->eoc = 1;
	adc_ready = 1;
}

// create new info data for adc
adc_info* adc_new(unsigned char channel, unsigned char adlar, unsigned char reference)
{
	adc_info *info;
	info = malloc(sizeof(adc_info));
	
	info->channel = channel;
	info->adlar = adlar;
	info->refer = reference;
	
	info->eoc = 0;
	info->result = 0;
	
	return info;
}

// is enabled
char adc_enabled()
{
	return adc_on;
}

// can convert
char adc_is_ready()
{
	return adc_ready;
}

// initializing ADC
void adc_init(unsigned char prescaler)
{
	// ADC Enable, ADC Interrupt Enable, ADC set Prescaler 
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (prescaler<<ADPS0);
	adc_on = 1;
	adc_ready = 1;
}

// request starting conversion
void adc_request(adc_info *info)
{
	adc_ready = 0;
	adc_for = info;
	
	// set info
	ADMUX = (info->refer<<REFS0) | (info->adlar<<ADLAR) | info->channel;
	
	// start conversion
	ADCSRA |= (1<<ADSC);
}

// read data (not ended = -1)
int adc_read(adc_info *info)
{
	if(info->eoc)
		return info->result;
	else
		return -1;	
}
