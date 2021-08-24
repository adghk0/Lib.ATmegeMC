/*
 * dht11.c
 *
 * Created: 2021-08-22 오후 11:20:14
 *  Author: ekj
 */ 

#include "dht11.h"

volatile uint8_t *dht_ddr, *dht_port, *dht_pin;
volatile unsigned char dht_bit;
unsigned char dht_data[5];

void dht_init(volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *pin, unsigned char bit)
{
	dht_ddr = ddr;
	dht_port = port;
	dht_bit = bit;
	dht_pin = pin;
	
	io_dir(dht_ddr, dht_bit, IO_OUTPUT);
	io_set(dht_port, dht_bit);
}
unsigned char dht_read_byte()
{
	int i = 0;
	unsigned char result = 0;
	
	for(i = 0; i < 8; i++)
	{
		// wait for 1
		io_wait(dht_pin, dht_bit, 1, 100);
		_delay_us(40);
		
		if(io_read(dht_pin, dht_bit))
		{
			result = (result << 1) | 1;
			io_wait(dht_pin, dht_bit, 0, 50);
		}
		else
			result = result << 1;
	}
	
	return result;
}
char dht_read()
{
	unsigned char result[5], i = 0;
	int ti;
	
	io_dir(dht_ddr, dht_bit, IO_OUTPUT);
	io_set(dht_port, dht_bit);
	
	// send start signal
	io_clear(dht_port, dht_bit);	
	_delay_ms(18);
	io_set(dht_port, dht_bit);
	
	// set input
	io_dir(dht_ddr, dht_bit, IO_INPUT);
	io_wait(dht_pin, dht_bit, 0, 100);
	
	// wait for response
	ti = io_wait(dht_pin, dht_bit, 1, 100);
	if(ti == -1)
		return -1;
	ti = io_wait(dht_pin, dht_bit, 0, 100);
	if(ti == -1)
		return -2;
	
	// read bytes
	for(i = 0; i < 5; i++)
	{
		result[i] = dht_read_byte();
	}
	if(result[0] + result[1] + result[2] + result[3] != result[4])
		return -3;
	
	memcpy(dht_data, result, 5);
	return 0;
}
unsigned char dht_get_temp()
{
	return dht_data[2];
}
unsigned char dht_get_humi()
{
	return dht_data[0];
}

unsigned char * dht_get_data()
{
	return dht_data;
}