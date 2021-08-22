/*
 * lcd1602_twi.c
 *
 * Created: 2021-08-21 오후 11:30:01
 *  Author: ekj
 */ 

#include "lcd1602_twi.h"

unsigned char lcd_addr = 0;

void lcd_move(unsigned char x, unsigned char y)
{
	lcd_send_cmd(0x80 | 0x40*y | x);
}
void lcd_clear()
{
	lcd_send_cmd(0x01);
}
void lcd_cursor_home()
{
	lcd_send_cmd(0x02);
}
void lcd_display(unsigned char display)
{
	lcd_send_cmd(0x08 | 0x04*display);
}
void lcd_backlight_on()
{
	lcd_backlight = 1;
}
void lcd_backlight_off()
{
	lcd_backlight = 0;
}

void lcd_send_cmd_4bit(unsigned char cmd_half)
{
	twi_send_byte(lcd_addr, (lcd_backlight << LCD_BL));
	twi_send_byte(lcd_addr, (1<<LCD_EN) | (lcd_backlight << LCD_BL));
	twi_send_byte(lcd_addr, cmd_half | (1<<LCD_EN) | (lcd_backlight << LCD_BL));
	twi_send_byte(lcd_addr, cmd_half | (lcd_backlight << LCD_BL));
	_delay_ms(2);
}
void lcd_send_cmd(unsigned char cmd)
{
	lcd_send_cmd_4bit(cmd & 0xf0);
	lcd_send_cmd_4bit(cmd << 4);
}
void lcd_send_data_4bit(unsigned char data_half)
{
	twi_send_byte(lcd_addr, (1<<LCD_RS) | (lcd_backlight << LCD_BL));
	twi_send_byte(lcd_addr, (1<<LCD_EN) | (1<<LCD_RS) | (lcd_backlight << LCD_BL));
	twi_send_byte(lcd_addr, data_half | (1<<LCD_EN) | (1<<LCD_RS) | (lcd_backlight << LCD_BL));
	twi_send_byte(lcd_addr, data_half | (1<<LCD_RS) | (lcd_backlight << LCD_BL));
	_delay_ms(2);
}
void lcd_send_data(char data)
{
	lcd_send_data_4bit(data & 0xf0);
	lcd_send_data_4bit(data << 4);
}
void lcd_send_string(char* str)
{
	while(*str) lcd_send_data(*str++);
}

void lcd_init(unsigned char addr)
{
	twi_init(100000UL, TWI_MASTER_MODE);
	
	lcd_addr = addr;
	
	// 4bit Power On
	lcd_backlight = 0;
	lcd_send_cmd_4bit(0x30);
	_delay_ms(15);
	lcd_send_cmd_4bit(0x30);
	_delay_ms(5);
	lcd_send_cmd_4bit(0x30);
	_delay_us(100);
	lcd_send_cmd_4bit(0x20);
	_delay_ms(5);
	
	// 4bit Initialize
	lcd_send_cmd(0x28); // Function set
	lcd_send_cmd(0x01); // Clear
	lcd_send_cmd(0x06); // Entry set
	lcd_backlight = 1;
	lcd_send_cmd(0x0c); // Display
}