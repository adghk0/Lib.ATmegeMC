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

/*** TYPES ***/
typedef struct acs712 {
	unsigned int mutifly;
	int result;
} acs712;

/*** FUNCTIONS ***/

void acs712_init(unsigned char mutiply, unsigned char channel);
// convert digital raw data to currents*mutiply
void acs712_conv(unsigned char max);


#endif /* ACS712_H_ */