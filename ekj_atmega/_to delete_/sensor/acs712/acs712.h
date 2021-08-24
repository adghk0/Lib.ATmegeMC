/*
 * acs712.h
 *
 * Created: 2021-08-22 오후 6:32:53
 *  Author: ekj
 */ 


#ifndef ACS712_H_
#define ACS712_H_

#define ACS712_MAX5		5
#define ACS712_MAX20	20
#define ACS712_MAX30	30

#define ACS712_MULTIFLY 10

#include "../../adc/adc.h"

// convert digital raw data to currents*MULTIFLY
int acs712_conv(int raw, unsigned char max);

#endif /* ACS712_H_ */