/*
 * io.c
 * the basic one pin controls
 * Created: 2021-08-23 오후 6:31:20
 *  Author: Eom Kyoungjun
 */ 

#include "io.h"

/*** FUNCTIONS ***/

// make a new pin structure
io_pin* io_new(volatile uint8_t *port, unsigned char bit)
{
	io_pin *pin;
	pin = malloc(sizeof(io_pin));
	
	pin->port = port;
	pin->ddr = port - 1;
	pin->pin = port - 2;
	pin->bit = bit;
	
	return pin;
}

// set direction
void io_dir(volatile io_pin *pin, char direction)
{
	*pin->ddr = (*pin->ddr&~(1<<pin->bit)) | (direction*(1<<pin->bit));
}

// set the pin (1)
void io_set(volatile io_pin *pin)
{
	*pin->port |= (1<<pin->bit);
}

// clear the pin (0)
void io_clear(volatile io_pin *pin)
{
	*pin->port &= ~(1<<pin->bit);	
}

// write the value to pin
void io_write(volatile io_pin *pin, char value)
{
	*pin->port = (*pin->port&~(1<<pin->bit)) | (value*(1<<pin->bit));
}

// get the output value at pin
char io_get(volatile io_pin *pin)
{
	return (*pin->port >> pin->bit) & 1;
}

// read the value from pin
char io_read(volatile io_pin *pin)
{
	return (*pin->pin >> pin->bit) & 1;
}
// wait until pin be target in timeout[us]
int io_wait_us(volatile io_pin *pin, char target, int timeout)
{
	int t = 0;
	
	while(t++ < timeout && io_read(pin) != target) _delay_us(1);
	
	if(t < timeout)
		return t;
	else
		return -1;
}

// wait until pin be target in timeout[ms]
int io_wait_ms(volatile io_pin *pin, char target, int timeout)
{
	int t = 0;
	
	while(t++ < timeout && io_read(pin) != target) _delay_ms(1);
	
	if(t < timeout)
	return t;
	else
	return -1;
}
