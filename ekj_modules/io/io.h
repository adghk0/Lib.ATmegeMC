/*
 * io.h
 * easy normal port controls
 * Created: 2021-08-22 오후 10:47:11
 *  Author: ekj
 */ 


#ifndef IO_H_
#define IO_H_

#define F_CPU 16000000UL

#define IO_INPUT	0
#define IO_OUTPUT	1

#include <stdint.h>
#include <util/delay.h>

void io_dir(volatile uint8_t *ddr, unsigned char bit, unsigned char dir);
void io_set(volatile uint8_t *port, unsigned char bit);
void io_clear(volatile uint8_t *port, unsigned char bit);
void io_write(volatile uint8_t *port, unsigned char bit, unsigned char val);
char io_read(volatile uint8_t *pin, unsigned char bit);
int io_wait(volatile uint8_t *pin, unsigned char bit, unsigned char obj, int timeout);

#endif /* IO_H_ */