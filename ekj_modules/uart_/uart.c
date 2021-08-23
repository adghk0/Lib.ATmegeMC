/*
 * uart.c
 *
 * Created: 2021-08-21 오후 5:20:54
 *  Author: ekj
 */ 

#include "uart.h"

// tx_q, rx_q ring buffer
volatile unsigned char tx_q_size, rx_q_size;
volatile char *tx_q, *rx_q;
volatile unsigned char tx_w, tx_r;
volatile unsigned char rx_w, rx_r;

// ISR
// UDR Empty Interrupt
ISR(USART_UDRE_vect)
{
	// save a flag
	char sreg = SREG;
	cli();
	
	// is data exist in queue
	if(tx_r != tx_w) {
		// send data
		UDR0 = tx_q[tx_r];
		tx_r = (tx_r + 1) & (tx_q_size - 1);
	}
	else	
		// UDR Interrupt Disable
		UCSR0B &= ~(1<<UDRIE0);
		
	// rerun
	SREG = sreg;
}
// RX Complete Interrupt
ISR(USART_RX_vect)
{
	char r;
	// save a flag
	char sreg = SREG;
	cli();
	
	r = UDR0;
	if(((rx_w + 1) & (rx_q_size - 1)) != rx_r)
	{
		rx_q[rx_w] = r;
		rx_w = (rx_w + 1) & (rx_q_size - 1);
	}
	
	// rerun
	SREG = sreg;
}

// Functions
// initializing UART
void uart_init(unsigned long f_cpu, unsigned long baudrate, unsigned char tx_q_s, unsigned char rx_q_s)
{
	cli();

	// set baudrate
	UBRR0 = (f_cpu / 4 / baudrate - 1) / 2;
	
	// 2x mode enable
	UCSR0A = (1<<U2X0);
	// RX, TX enable, RX complete interrupt enable
	UCSR0B = ((1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0));
	// 8bit mode
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

	tx_q_size = tx_q_s;
	rx_q_size = rx_q_s;
	tx_q = (char*)malloc(tx_q_s);
	rx_q = (char*)malloc(rx_q_s);
	tx_w = tx_r = 0;
	rx_w = rx_r = 0;

	sei();
}

// write a char
char uart_write(char byte)
{
	char error = 0;
	// save a flag
	char sreg = SREG;
	cli();
	
	// check overflow
	if(((tx_w + 1) & (tx_q_size - 1)) == tx_r)
		error = 1;
	
	tx_q[tx_w] = byte;
	tx_w =  (tx_w + 1) & (tx_q_size - 1);
	
	// UDR Interrupt Enable
	UCSR0B |= (1<<UDRIE0);
	
	// rerun
	SREG = sreg;
	return error;
}
// write a string
char uart_write_string(char *str)
{
	unsigned char i = 0, error = 0;
	
	// save a flag
	char sreg = SREG;
	cli();
	
	while(str[i])
		// check overflow
		if(((tx_w + 1) & (tx_q_size - 1)) == tx_r)
			break;
		else {
			tx_q[tx_w] = str[i++];
			tx_w = (tx_w + 1) & (tx_q_size - 1);
		}
		
	// if overflow
	if(str[i])
	{
		// count unsent
		while(str[i++])
			error++;
	}
	else
		// UDR Interrupt Enable
		UCSR0B |= (1<<UDRIE0);
		
	// rerun
	SREG = sreg;
	return error;
}
// can read
char uart_read_available()
{
	char r;
	// save a flag
	char sreg = SREG;
	cli();
	
	if(rx_w >= rx_r)
		r = rx_w - rx_r;
	else
		r = rx_w + rx_q_size - rx_r;
		
	SREG = sreg;
	return r;
}
// read a char
char uart_read()
{
	if(rx_w == rx_r) return 0;
	
	// save a flag
	char c;
	char sreg = SREG;
	cli();
	
	c = rx_q[rx_r];
	rx_r = (rx_r + 1) & (rx_q_size - 1);
	
	SREG = sreg;
	return c;
}
// read string from queue to buf
void uart_read_string(char *buf, char size)
{
	if(rx_w == rx_r) { buf[0] = '\0'; return; }
	
	// save a flag
	unsigned char i = 0;
	char sreg = SREG;
	cli();
	
	while(rx_w != rx_r && i < size - 1)
	{
		buf[i++] = rx_q[rx_r];
		rx_r = (rx_r + 1) & (rx_q_size - 1);
	}
	buf[i] = '\0';
	
	SREG = sreg;
}
// clear TX queue
void uart_tx_flush()
{
	tx_r = tx_w = 0;
}
// clear RX queue
void uart_rx_flush()
{
	rx_r = rx_w = 0;
}


unsigned char uart_tx_q_size()
{
	return tx_q_size;
}
unsigned char uart_rx_q_size()
{
	return rx_q_size;
}