/*
 * lcd1602wire4.h
 *
 * Created: 01.12.2016 0:24:04
 *  Author: 1
 */ 


#ifndef LCD1602WIRE4_H_
#define LCD1602WIRE4_H_

//Прототипы функций
void sendbyte_to_LCD(unsigned char byte, unsigned char RS);
void LCD_ini(void);
void setpos_to_LCD(unsigned char X, unsigned char Y);
void sendsymbol_to_LCD(unsigned char symbol);
void string_to_LCD(char str1[]);
void move_screen_right(void);
void move_screen_left(void);
void move_cursor_right(void);
void move_cursor_left(void);
void clear_LCD(void);
void visible_cursor(void);
#endif /* LCD1602WIRE4_H_ */