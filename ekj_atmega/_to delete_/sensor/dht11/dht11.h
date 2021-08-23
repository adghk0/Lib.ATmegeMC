/*
 * dht11.h
 *
 * Created: 2021-08-22 오후 11:16:26
 *  Author: ekj
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU	16000000UL

#include "../../io_/io.h"
#include <util/delay.h>
#include <string.h>

void dht_init(volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *pin, unsigned char bit);
char dht_read();
unsigned char dht_get_temp();
unsigned char dht_get_humi();
unsigned char * dht_get_data();

#endif /* DHT11_H_ */