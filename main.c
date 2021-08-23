/*
 * Test.c
 *
 * Created: 2021-08-21 오후 5:16:14
 * Author : ekj
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "ekj_atmega/communication/uart/uart.h"
#include "ekj_atmega/basic/io.h"

int main(void)
{
	//uart_init(F_CPU, 9600, 128, 128);
	
	io_pin *pin = io_new(&PORTB, 0);
	io_dir(pin, IO_OUTPUT);
	//uart_write(DDRB);
	
	while(1)
	{
		_delay_ms(100);
		io_write(pin, io_get(pin) ^ 1);
		//uart_write(PORTB);
	}
}

/*
#include "ekj_modules/uart/uart.h"
#include "ekj_modules/io/io.h"
#include "ekj_modules/sensor/dht11/dht11.h"

int main(void)
{
	char str[100], error;
	
	unsigned char *data;
	unsigned int temp, humi;
	
	uart_init(F_CPU, 9600, 128, 128);
	
	dht_init(&DDRB, &PORTB, &PINB, 0);
	
	while(1)
	{	
		_delay_ms(5);
		error = dht_read();
		
		
		uart_write('0' - error);
		if(error == 0)
		{
		_delay_ms(1000);
		data = dht_get_data();
		humi = data[0] << 8 | data[1];
		temp = (data[2] & 0x7f) << 8 | data[3];
		
		uart_write_string("humi: ");
		sprintf(str, "%2d", humi / 10);
		uart_write_string(str);
		uart_write('.');
		sprintf(str, "%2d", humi % 10);
		uart_write_string(str);
		uart_write_string("%, temp: ");
		if(data[2] & 0x80)
			uart_write('-');
		sprintf(str, "%2d", temp / 10);
		uart_write_string(str);
		uart_write('.');
		sprintf(str, "%2d", temp % 10);
		uart_write_string(str);
		uart_write_string("C\n\r");
		*/
		
		/*
		_delay_ms(500);
		uart_write_string("humi: ");
		sprintf(str, "%2d", dht_get_humi());
		uart_write_string(str);
		uart_write_string("%, temp: ");
		sprintf(str, "%2d", dht_get_temp());
		uart_write_string(str);
		uart_write_string("C\n\r");
	}
}*/

/*
// ACS712 TEST
#include "ekj_modules/uart/uart.h"
#include "ekj_modules/sensor/acs712/acs712.h"
#include "ekj_modules/adc/adc.h"
#include <math.h>

#define AVG 50

int main(void)
{
	long result;
	char str[100];
	uart_init(F_CPU, 9600, 128, 128);
	
	adc_init(ADC_ALIGN_LSB, ADC_REF_AREF, ADC_PRE_16);
	
	while (1)
	{
		_delay_ms(500);
		
		result = 0;
		
		for(int i = 0; i < AVG; i++)
		{
			adc_request(ADC_CHANNEL0);
			while(!adc_available());
			result += adc_read();
		}
		result /= AVG;
		
		sprintf(str, "%4d", result);
		uart_write_string(str);
		uart_write_string(" - ");
		
		sprintf(str, "%2d", acs712_conv(result, ACS712_MAX20) / ACS712_MULTIFLY);
		uart_write_string(str);
		uart_write('.');
		sprintf(str, "%1d", abs(acs712_conv(result, ACS712_MAX20) % ACS712_MULTIFLY));
		uart_write_string(str);
		uart_write_string("A\n\r");
	}
}
*/
/*
// ADC, UART, LCD_TWI TEST PROGRAM
#include "ekj_modules/uart/uart.h"
#include "ekj_modules/twi/twi.h"
#include "ekj_modules/lcd/lcd1602/lcd1602_twi.h"
#include "ekj_modules/adc/adc.h"

int main(void)
{	
	int result;
	char error;
	char str[100];
	uart_init(F_CPU, 9600, 128, 128);
	
	lcd_init(0x27);
	
	adc_init(F_CPU, ADC_ALIGN_LSB, ADC_REF_AREF);
	
    while (1) 
    {
		_delay_ms(500);
		//uart_read_string(buf, RX_Q_SIZE);
		//uart_write_string(buf);
		
		adc_request(ADC_CHANNEL0);
		
		while(!adc_available());
		result = adc_read();
		
		sprintf(str, "%4d", result);
		lcd_move(0, 0);
		lcd_send_string(str);
		
		if(uart_read_available()) {
			lcd_move(0, 1);
			uart_read_string(str, 100);
			lcd_send_string(str);
			
			lcd_move(6, 0);
			error = uart_write_string(str);
			sprintf(str, "%4d", error);
			lcd_send_string(str);
		}
    }
}
*/