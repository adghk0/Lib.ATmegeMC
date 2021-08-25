/*
 * acs712.h
 * sense the currents
 * Created: 2021-08-24 오후 8:34:31
 *  Author: Eom Kyoungjun
 */ 


#ifndef ACS712_H_
#define ACS712_H_

/*** DEFINES ***/

#define ACS712_MAX5		5
#define ACS712_MAX20	20
#define ACS712_MAX30	30


/*** INCLUDES ***/
#include "../../../basic/adc.h"
#include <stdlib.h>

/*** TYPES ***/
typedef struct acs712 {
	adc_info *channel;
	unsigned int mutiply;
	unsigned char avg;
	unsigned int level;
	unsigned int offset;
	int result;
} acs712;

/*** FUNCTIONS ***/

acs712* acs712_init(unsigned int channel,
					unsigned int mutiply,
					unsigned char average,
					unsigned char max);
					
// convert digital raw data to currents*mutiply
long acs712_conv(acs712* acs);


#endif /* ACS712_H_ */