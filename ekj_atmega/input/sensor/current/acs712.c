/*
 * acs712.c
 * sense the currents
 * Created: 2021-08-24 오후 8:35:12
 *  Author: Eom Kyoungjun
 */ 

#include "acs712.h"

/*** FUNCTIONS ***/

unsigned long muti;

void acs712_init(unsigned int channel, unsigned int mutiply)
{
	muti = mutiply;
	
	if(adc_enabled())
		adc_init(ADC_ALIGN_LSB, ADC_REF_AREF, ADC_PRE_8);
}
// convert digital raw data to currents*mutiply
long acs712_conv(unsigned char max)
{
	int level;
	long result;
	
	if(max == ACS712_MAX5)
	level = 185;
	else if(max == ACS712_MAX20)
	level = 100;
	else // 30A
	level = 66;
	
	adc_request()
}