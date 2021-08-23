/*
 * twi.c
 *
 * Created: 2021-08-21 오후 10:50:38
 *  Author: ekj
 */ 


#include "twi.h"

void _twi_wait()
{
	while(!(TWCR & (1<<TWINT)));
}

void twi_init(unsigned long scl_freq, char mode)
{
	// set the Bit Rate
	TWBR = F_CPU / 2 / scl_freq - 16;
	TWSR = 0x00;
	TWCR = (1<<TWEN);
}

unsigned char twi_send_byte(unsigned char addr, unsigned char data)
{
	char error = 0;
	char sreg = SREG;
	cli();
	
	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for SEND
	_twi_wait();
	// check value for ERROR
	if((TWSR & TWI_STATUS) != TWI_START && (TWSR & TWI_STATUS) != TWI_REPEAT)
		error = 1;
	else 
	{
		// load address and write signal
		// ADDR | R=1(W=0)
		TWDR = addr << 1;
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wait for SEND
		_twi_wait();
		// check value for ERROR
		if((TWSR & TWI_STATUS) != TWI_W_ACK)
			error = (TWSR & TWI_STATUS);
		else
		{
			// load data
			TWDR = data;
			TWCR = (1<<TWINT) | (1<<TWEN);
			// wait for SEND
			_twi_wait();
			// check value for ERROR
			if((TWSR & TWI_STATUS) != TWI_WD_ACK)
				error = 3;
			else
				// send STOP condition
				TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
		}
		
	}
	
	SREG = sreg;
	
	return error;
}