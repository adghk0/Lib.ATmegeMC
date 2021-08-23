/*
 * io.c
 *
 * Created: 2021-08-22 오후 11:21:54
 *  Author: ekj
 */ 

#include "io.h"

void io_dir(volatile uint8_t *ddr, unsigned char bit, unsigned char dir)
{
	*ddr = (*ddr&~(1<<bit)) | (dir*(1<<bit));
}
void io_set(volatile uint8_t *port, unsigned char bit)
{
	*port |= (1<<bit);
}
void io_clear(volatile uint8_t *port, unsigned char bit)
{
	*port &= ~(1<<bit);
}
void io_write(volatile uint8_t *port, unsigned char bit, unsigned char val)
{
	*port = (*port&~(1<<bit)) | (val*(1<<bit));
}
char io_read(volatile uint8_t *pin, unsigned char bit)
{
	return (*pin >> bit) & 1;
}
int io_wait(volatile uint8_t *pin, unsigned char bit, unsigned char obj, int timeout)
{
	int time_us = 0;
	
	while(time_us++ < timeout && io_read(pin, bit) != obj) _delay_us(1);
	
	if(time_us < timeout)
		return time_us;
	else
		return -1; 
}