/*
 * uart.c
 * UART communication
 * Created: 2021-08-23 오후 7:46:47
 *  Author: Eom Kyoungjun
 */ 


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
					0, 128, 128);
}


// is buffer can be written
char uart_buf_writeble(uart_buffer *buf)
{
	return uart_buf_next_index(buf->w_pos) != buf->r_pos;
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
		r = rx_w - rx_r;
	else
		r = rx_w + buf->size - rx_r;
		
	return r;
}

// read data from buffer
char uart_buf_read(uart_buffer *buf)
{
	char re = buf->data[buf->r_pos];
	buf->r_pos = uart_buf_next_index(buf->r_pos);
	return re;
}


// UART0
#ifdef UCSR0A
uart_info *uart0_info;

// initializing
void uart0_init(uart_info *info)
{
	cli();
	
	uart0_info = info;
	
	// set baudrate
	if(info->u2x)
		UBRR0 = F_CPU / 8 / info->baudrate - 1;
	else
		UBRR0 = F_CPU / 16 / info->baudrate - 1; 
	
	// set UCSR0A
	UCSR0A = (info->u2x<<U2X0);
	// set UCSR0B
	UCSR0B = (1<<RXCIE0) | (info->rx_en<<RXEN0)
			 | (info->tx_en<<TXEN0) | ((info->datasize&4));
	// set UCSR0C
	UCSR0C = (info->mode<<UMSEL00) | (info->parity<<UPM00)
			 | (info->stopbit<<USBS0) | (info->datasize&3<<UCSZ00)
			 | (info->polarity<<UCPOL0);
	
	sei();
}

// write a byte
void uart0_write(char data)
{
	char sreg = SREG;
	cli();
	
	uart0_info->
	
	SREG = sreg;
}
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
#endif /* UART0 */