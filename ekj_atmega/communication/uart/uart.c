/*
 * uart.c
 * UART communication
 * Created: 2021-08-23 오후 7:46:47
 *  Author: Eom Kyoungjun
 */ 

#include "uart.h"

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
					unsigned char rx_buffer_size)
{
	// make new info structure
	uart_info *info;
	uart_buffer *tx_b, *rx_b;
	uart_status *sta;
	
	// allocate
	info = malloc(sizeof(uart_info));
	tx_b = malloc(sizeof(uart_buffer));
	rx_b = malloc(sizeof(uart_buffer));
	sta = malloc(sizeof(uart_status));
	
	// set datas
	info->baudrate = baudrate;
	info->u2x = uart2x;
	info->tx_en = tx_enable;
	info->rx_en = rx_enable;
	info->mode = mode;
	info->stopbit = stop_bit;
	info->datasize = data_size;
	info->polarity = polarity;
	info->parity = parity;
	
	tx_b->size = tx_buffer_size;
	tx_b->r_pos = 0;
	tx_b->w_pos = 0;
	info->tx_buf = tx_b;
	
	rx_b->size = rx_buffer_size;
	rx_b->r_pos = 0;
	rx_b->w_pos = 0;
	info->rx_buf = rx_b;
	
	sta->error = 0;
	sta->overflow = 0;
	info->status = sta;
	
	return info;
}

// make new uart infomation simply
uart_info* uart_new_simple(unsigned long baudrate)
{
	// default set
	// u2x, tx.rx enable, asynronize, stop-1, 
	// data-8, 128bytes buffers
	return uart_new(baudrate,
				    1, 1, 1, UART_MODE_ASC,
					UART_STOP_1BIT,
					UART_DATA_8BIT,
					UART_PARITY_DISABLE,
					0, 128, 128);
}



// is buffer can be written
char uart_buf_writeble(uart_buffer *buf)
{
	return uart_buf_next_index(buf, buf->w_pos) != buf->r_pos;
}

// next index
unsigned char uart_buf_next_index(uart_buffer *buf, unsigned char index)
{
	return (index + 1) % buf->size;
}

// write data to buffer
void uart_buf_write(uart_buffer *buf, char data)
{
	buf->data[buf->w_pos] = data;
	buf->w_pos = uart_buf_next_index(buf, buf->w_pos);
}

// has data in buffer
char uart_buf_available(uart_buffer *buf)
{
	char r;
	
	if(buf->r_pos >= buf->w_pos)
		r = buf->w_pos - buf->r_pos;
	else
		r = buf->w_pos + buf->size - buf->r_pos;
		
	return r;
}

// read data from buffer
char uart_buf_read(uart_buffer *buf)
{
	char re = buf->data[buf->r_pos];
	buf->r_pos = uart_buf_next_index(buf, buf->r_pos);
	return re;
}


// UART0
#ifdef UCSR0A
uart_info *uart0_info;

// UDR Empty Interrupt
ISR(USART_UDRE_vect)
{
	char sreg = SREG;
	cli();
	
	uart_buffer *tx_b = uart0_info->tx_buf;
	
	// has data in buffer
	if(uart_buf_available(tx_b))
		// send
		UDR0 = uart_buf_read(tx_b);
	else
		UCSR0B &= ~(1<<UDRIE0);
	
	SREG = sreg;
}
// RX Complete Interrupt
ISR(USART_RX_vect)
{
	char r;
	char sreg = SREG;
	cli();
	
	uart_buffer *rx_b = uart0_info->rx_buf;
	
	// get data
	r = UDR0;
	uart0_info->status->error = UCSR0A & UART_ERROR_MSK;
	
	// write
	if(uart_buf_writeble(rx_b))
		uart_buf_write(rx_b, r);
	
	SREG = sreg;
}

// initializing
void uart0_init(uart_info *info)
{
	cli();
	
	uart0_info = info;
	
	uart0_info->tx_buf->data = malloc(uart0_info->tx_buf->size);
	uart0_info->rx_buf->data = malloc(uart0_info->rx_buf->size);
	
	// set baudrate
	if(info->u2x)
		UBRR0 = F_CPU / info->baudrate / 8 - 1;
	else
		UBRR0 = F_CPU / info->baudrate / 16 - 1; 
	
	
	// set UCSR0A
	UCSR0A = (info->u2x<<U2X0);
	
	// set UCSR0B
	UCSR0B = (1<<RXCIE0) | (info->rx_en<<RXEN0)
			 | (info->tx_en<<TXEN0) | ((info->datasize&4));
	
	// set UCSR0C
	UCSR0C = (info->mode<<UMSEL00) | (info->parity<<UPM00)
			 | (info->stopbit<<USBS0) | ((info->datasize&3)<<UCSZ00)
			 | (info->polarity<<UCPOL0);
	
	sei();
}

// write a byte
void uart0_write(char data)
{
	char sreg = SREG;
	cli();
	
	// clear last overflow signal
	uart0_info->status->overflow = 0;
	
	// get tx buffer
	uart_buffer *tx_b = uart0_info->tx_buf;
	
	// check overflow
	if (uart_buf_writeble(tx_b))
	{
		uart_buf_write(tx_b, data);
		// set UDR interrupt
		UCSR0B |= (1<<UDRIE0);
	}
	else
		uart0_info->status->overflow = 1;
	
	SREG = sreg;
}

// write a string
void uart0_write_string(char *str)
{
	unsigned char i = 0, rem = 0;
	char sreg = SREG;
	cli();
	
	// clear last overflow signal
	uart0_info->status->overflow = 0;
	
	// get tx buffer
	uart_buffer *tx_b = uart0_info->tx_buf;
	
	// check overflow
	while (uart_buf_writeble(tx_b) && str[i])
	{
		uart_buf_write(tx_b, str[i++]);
		// set UDR interrupt
		UCSR0B |= (1<<UDRIE0);
	}
	
	if(str[i])
	{
		// check remains
		while(str[i++]) rem++;
		uart0_info->status->overflow = rem;
	}
	
	SREG = sreg;
}

// has data in rx buffer
char uart0_available()
{
	return uart_buf_available(uart0_info->rx_buf);	
}

// read a byte
char uart0_read()
{
	if(uart0_available())
		return uart_buf_read(uart0_info->rx_buf);
	else
		return 0;
}

// read a string and put in buf
void uart0_read_string(char *buf, char size)
{
	unsigned char i = 0;
	while(uart0_available() && i < size - 1)
	{
		buf[i++] = uart_buf_read(uart0_info->rx_buf); 
	}
	buf[i] = '\0';
}

// tx buffer flush
void uart0_tx_flush()
{
	uart_buffer *tx_b = uart0_info->tx_buf;
	tx_b->r_pos = 0;
	tx_b->w_pos = 0;
}

// rx buffer flush
void uart0_rx_flush()
{
	uart_buffer *rx_b = uart0_info->rx_buf;
	rx_b->r_pos = 0;
	rx_b->w_pos = 0;
}

// get error signal
char uart0_get_error()
{
	return uart0_info->status->error;
}

// get info structure
uart_info *uart0_get_info()
{
	return uart0_info;
}

#endif /* UART0 */