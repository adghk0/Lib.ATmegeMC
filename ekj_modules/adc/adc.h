/*
 * adc.h
 *
 * Created: 2021-08-22 오후 4:55:42
 *  Author: ekj
 */ 

#ifndef ADC_H_
#define ADC_H_

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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>

// initializing ADC
void adc_init(unsigned long f_cpu, unsigned char adlar, unsigned char ref);
// request starting conversion
void adc_request(unsigned char channel);
// can read the data
// Returns : exist(1), no-exist(0)
char adc_available();
// read data
// Returns : data(16bit)
int adc_read();


#endif /* ADC_H_ */