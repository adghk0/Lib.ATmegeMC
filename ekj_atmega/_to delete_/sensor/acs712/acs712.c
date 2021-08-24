/*
 * acs712.c
 *
 * Created: 2021-08-22 오후 6:41:09
 *  Author: ekj
 */ 

#include "acs712.h"

int offset = 512; 

// convert digital raw data to currents*MULTIFLY
int acs712_conv(int raw, unsigned char max)
{
	int level;
	long long result;
	
	if(max == ACS712_MAX5)
		level = 185;
	else if(max == ACS712_MAX20)
		level = 100;
	else // 30A
		level = 66;
		
	result = ((long)(raw) - offset) * 5 * ACS712_MULTIFLY * 1000 / level / 1024;
	
	return result;
}