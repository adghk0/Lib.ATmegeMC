/*
 * adc.h
 *
 * Created: 2021-08-22 오후 4:55:42
 *  Author: ekj
 */ 

#ifndef ADC_H_
#define ADC_H_

/*** DEFINES ***/

#define ADC_CHANNEL0	0
#define ADC_CHANNEL1	1
#define ADC_CHANNEL2	2
#define ADC_CHANNEL3	3
#define ADC_CHANNEL4	4
#define ADC_CHANNEL5	5
#define ADC_CHANNEL6	6
#define ADC_CHANNEL7	7
#define ADC_CHANNEL8	8

#define ADC_ALIGN_LSB	0
#define ADC_ALIGN_MSB	1

#define ADC_REF_AREF	0
#define ADC_REF_AVCC	1
#define ADC_REF_INTER	3

#define ADC_PRE_2	1
#define ADC_PRE_4	2
#define ADC_PRE_8	3
#define ADC_PRE_16	4
#define ADC_PRE_32	5
#define ADC_PRE_64	6
#define ADC_PRE_128	7

/*** INCLUDES ***/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>

/*** TYPES ***/
typedef struct adc_info {
	unsigned char channel;
	unsigned char adlar;
	unsigned char refer;
	unsigned char eoc;
	int result;
} adc_info;

/*** FUNCTIONS ***/

// create new info data for adc
adc_info* adc_new(unsigned char channel, unsigned char adlar, unsigned char reference);

// is enabled
char adc_enabled();
// can convert
char adc_is_ready();
// initializing ADC
void adc_init(unsigned char prescaler);
// request starting conversion
void adc_request(adc_info *info);
// read data (not ended = -1)
int adc_read(adc_info *info);

#endif /* ADC_H_ */