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
	return uart_new(baudrate,
				    1, 1, 1, UART_MODE_ASC,
					UART_STOP_1BIT,
					UART_DATA_8BIT,
					0, 128, 128);
}