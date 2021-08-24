/*
 * uart.h
 * UART communication
 * Created: 2021-08-23 오후 7:27:42
 *  Author: Eom Kyoungjun
 */ 

#ifndef UART_H_
#define UART_H_

/*** DEFINES ***/

#define UART_FRAME_ERROR	8
#define UART_OVERRUN_ERROR	4
#define UART_PARITY_ERROR	2
#define UART_ERROR_MSK		14

#define UART_MODE_ASC		0
#define UART_MODE_SYN		1
// #define UART_MODE_SPI		3		// not support
// #define UART_MODE_MPC		4		// not support

#define UART_PARITY_DISABLE	0
#define UART_PARITY_EVEN	2
#define UART_PARITY_ODD		3

#define UART_STOP_1BIT		0
#define UART_STOP_2BIT		1

#define UART_DATA_5BIT		0
#define UART_DATA_6BIT		1
#define UART_DATA_7BIT		2
#define UART_DATA_8BIT		3
// #define UART_DATA_9BIT		7		// not support


/*** INCLUDES ***/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <stdlib.h>

/*** TYPES ***/
typedef struct uart_status
{
	unsigned char error;
	unsigned char overflow;
} uart_status;

typedef struct uart_buffer
{
	unsigned char size;
	char* data;
	unsigned char w_pos;
	unsigned char r_pos;
} uart_buffer;

typedef struct uart_info
{
	unsigned long baudrate;
	unsigned char u2x;
	unsigned char tx_en;
	unsigned char rx_en;
	unsigned char datasize;
	unsigned char mode;
	unsigned char parity;
	unsigned char stopbit;
	unsigned char polarity;
	uart_status *status;
	uart_buffer *tx_buf;
	uart_buffer *rx_buf;
} uart_info;

/*** FUNCTIONS ***/

// make new uart infomation
uart_info* uart_new(unsigned long baudrate,
					unsigned char uart2x,
					unsigned char tx_enable,
					unsigned char rx_enable,
					unsigned char mode,
					unsigned char stop_bit,
					unsigned char data_size,
					unsigned char parity,
					unsigned char polarity,
					unsigned char tx_buffer_size,
					unsigned char rx_buffer_size);
// make new uart infomation simply
uart_info* uart_new_simple(unsigned long baudrate);


// is buffer can be written
char uart_buf_writeble(uart_buffer *buf);
// next index
unsigned char uart_buf_next_index(uart_buffer *buf, unsigned char index);
// write data to buffer
void uart_buf_write(uart_buffer *buf, char data);
// has data in buffer
char uart_buf_available(uart_buffer *buf);
// read data from buffer
char uart_buf_read(uart_buffer *buf);


// UART0
#ifdef UCSR0A
// initializing
void uart0_init(uart_info *info);
// write a byte
void uart0_write(char data);
// write a string
void uart0_write_string(char *str);
// has data in rx buffer
char uart0_available();
// read a byte
char uart0_read();
// read a string and put in buf
void uart0_read_string(char *buf, char size);
// tx buffer flush
void uart0_tx_flush();
// rx buffer flush
void uart0_rx_flush();
// get error signal
char uart0_get_error();
// get info structure
uart_info *uart0_get_info();
#endif /* UART0 */

#endif /* UART_H_ */