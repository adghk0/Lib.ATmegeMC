/*
 * acs712.c
 * sense the currents
 * Created: 2021-08-24 오후 8:35:12
 *  Author: Eom Kyoungjun
 */ 

#include "acs712.h"

int offset = 511;

/*** FUNCTIONS ***/

acs712* acs712_init(unsigned int channel,
					unsigned int mutiply,
					unsigned char average,
					unsigned char max)
{
	unsigned long offset = 0;
	int temp, i;
	acs712 *acs;
	acs = malloc(sizeof(acs712));
	
	acs->channel = adc_new(channel, ADC_ALIGN_LSB, ADC_REF_AVCC);
	acs->mutiply = mutiply;
	acs->avg = average;
	
	// set level [A/mV]
	if(max == ACS712_MAX5)
		acs->level = 185;
	else if(max == ACS712_MAX20)
		acs->level = 100;
	else // 30A
		acs->level = 66;
	
	// enable acs
	if(adc_enabled())
		adc_init(ADC_PRE_8);
	
	for(i = 0; i < acs->avg; i++)
	{
		while(!adc_is_ready());
		adc_request(acs->channel);

		do {
			temp = adc_read(acs->channel);
		} while(temp == -1);
		offset += temp;
	}
	acs->offset = offset / acs->avg;
	
	return acs;
}

// convert digital raw data to currents*mutiply
long acs712_conv(acs712* acs)
{
	int temp;
	long result;
	char i;
	
	// sum adc datas to make a value exactly
	result = 0;
	for(i = 0; i < acs->avg; i++)
	{
		while(!adc_is_ready());
		adc_request(acs->channel);
		
		do {	
			temp = adc_read(acs->channel);
		} while(temp == -1);
		result += temp;	
	}
	result /= acs->avg;
	
	// calculate currents
	result = (result - acs->offset) * 5 * acs->mutiply * 1000 / acs->level / 1024;
	
	return result;
}