/*
 * lcd1602_twi.h
 *
 * Created: 2021-08-21 오후 11:28:14
 *  Author: ekj
 */ 


#ifndef LCD1602_H_
#define LCD1602_H_

// TWI GP Set
#define LCD_RS	0
#define LCD_RW	1
#define LCD_EN	2
#define LCD_BL	3
#define LCD_D4	4
#define LCD_D5	5
#define LCD_D6	6
#define LCD_D7	7

#include <avr/io.h>
#include <util/delay.h>

#include "../../twi/twi.h"

unsigned char lcd_backlight;

void lcd_move(unsigned char x, unsigned char y);
void lcd_clear();
void lcd_cursor_home();
void lcd_display(unsigned char display);
void lcd_backlight_on();
void lcd_backlight_off();

void lcd_send_cmd(unsigned char cmd);
void lcd_send_data(char data);
void lcd_send_string(char* str);

void lcd_init(unsigned char addr);



#endif /* LCD1602_H_ */