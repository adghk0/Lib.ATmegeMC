/*
 * uart.h
 *
 * Created: 2021-08-21 오후 5:20:30
 *  Author: ekj
 */ 

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <stdlib.h>

// Functions
// initializing UART
void uart_init(unsigned long f_cpu, unsigned long baudrate, unsigned char tx_q_s, unsigned char rx_q_s);
// write a char
// return: success=0 fail=1
char uart_write(char byte);
// write a string
// return: success=0 fail=the number of fail bytes
char uart_write_string(char *str);
// can read
// return: the number of bytes in queue
char uart_read_available();
// read a char
// return: data
char uart_read();
// read string from queue to buf
void uart_read_string(char *buf, char size);
// clear tx queue
void uart_tx_flush();
// clear rx queue
void uart_rx_flush();


unsigned char uart_tx_q_size();
unsigned char uart_rx_q_size();

#endif /* UART_H_ */