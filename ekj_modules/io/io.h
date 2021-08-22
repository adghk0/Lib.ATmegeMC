/*
 * io.h
 * easy normal port controls
 * Created: 2021-08-22 오후 10:47:11
 *  Author: ekj
 */ 


#ifndef IO_H_
#define IO_H_

#define IO_INPUT	0
#define IO_OUTPUT	1

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
	return (*pin & (1<<bit)) != 0;
}

#endif /* IO_H_ */