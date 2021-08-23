/*
 * io.h
 * the basic one pin controls
 * Created: 2021-08-23 오후 5:18:01
 *  Author: Eom Kyoungjun
 */ 

#ifndef IO_H_
#define IO_H_

/*** DEFINES ***/

#define IO_INPUT	0
#define IO_OUTPUT	1

#define IO_FALSE	0
#define IO_TRUE		1

/*** INCLUDES ***/
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

/*** TYPES ***/

// the type describing a pin
typedef struct io_pin
{
	volatile uint8_t *ddr;
	volatile uint8_t *port;
	volatile uint8_t *pin;
	unsigned char bit;
} io_pin;

/*** FUNCTIONS ***/

// make a new pin structure
io_pin* io_new(volatile uint8_t *port, unsigned char bit);
// set direction
void io_dir(volatile io_pin *pin, char direction);
// set the pin (1)
void io_set(volatile io_pin *pin);
// clear the pin (0)
void io_clear(volatile io_pin *pin);
// write the value to pin
void io_write(volatile io_pin *pin, char value);
// get the output value at pin
char io_get(volatile io_pin *pin);
// read the value from pin
char io_read(volatile io_pin *pin);
// wait until pin be target in timeout[us]
int io_wait_us(volatile io_pin *pin, char target, int timeout);
// wait until pin be target in timeout[ms]
int io_wait_ms(volatile io_pin *pin, char target, int timeout);

#endif /* IO_H_ */