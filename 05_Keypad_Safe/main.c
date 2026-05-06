/*
 * Пароль для сейфа (Atmega32)
 * Ввод пароля 666 с кейпада, маскировка звёздочками, обратная связь на LCD и светодиоде
 * Частота МК: 1 МГц
 */

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd1602wire4.h"

char n, i, protect;   // protect — накопление введённых символов
char flag;

void port_ini(void)
{
    // столбцы кейпада на выход (PD0..PD2)
    DDRD |= (1 << 0) | (1 << 1) | (1 << 2);
    // строки кейпада на вход с подтяжкой (PD3..PD6)
    DDRD &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6));
    PORTD |= (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6);

    DDRC |= (1 << 0);   // светодиод на PC0
}

int Keypad_scan(void)
{
    n = 10;  // 10 — "нет нажатия"

    // столбец 1 (PD0 = 0)
    PORTD &= ~(1 << 0);
    PORTD |= (1 << 1) | (1 << 2);
    if (~PIND & (1 << 3)) n = 1;
    if (~PIND & (1 << 4)) n = 4;
    if (~PIND & (1 << 5)) n = 7;
    if (~PIND & (1 << 6)) n = 0x2A - 0x30;  // '*' -> 0x2A

    // столбец 2 (PD1 = 0)
    PORTD &= ~(1 << 1);
    PORTD |= (1 << 0) | (1 << 2);
    if (~PIND & (1 << 3)) n = 2;
    if (~PIND & (1 << 4)) n = 5;
    if (~PIND & (1 << 5)) n = 8;
    if (~PIND & (1 << 6)) n = 0;

    // столбец 3 (PD2 = 0)
    PORTD &= ~(1 << 2);
    PORTD |= (1 << 0) | (1 << 1);
    if (~PIND & (1 << 3)) n = 3;
    if (~PIND & (1 << 4)) n = 6;
    if (~PIND & (1 << 5)) n = 9;
    if (~PIND & (1 << 6)) n = 0x23 - 0x30; // '#' -> 0x23

    return n;
}

int main(void)
{
    port_ini();
    LCD_ini();
    visible_cursor();
    clear_LCD();

    setpos_to_LCD(0, 0);
    string_to_LCD("Enter password:");
    setpos_to_LCD(5, 1);

    while (1)
    {
        n = Keypad_scan();

        if (n != 10)
        {
            if (flag == 1)
            {
                PORTC &= ~(1 << 0);
                flag = 0;
            }

            i++;
            sendbyte_to_LCD('*', 1);   // выводим '*' вместо цифры
            _delay_ms(300);

            // проверка пароля 666
            if (n == 6 && i == 1 && protect == 0) protect = 1;
            else if (n == 6 && i == 2 && protect == 1) protect = 2;
            else if (n == 6 && i == 3 && protect == 2)
            {
                protect = 3;
                for (int j = 0; j < 5; j++)
                {
                    PORTC |= (1 << 0);
                    _delay_ms(200);
                    PORTC &= ~(1 << 0);
                    _delay_ms(200);
                }
            }

            // после ввода трёх символов
            if (i == 3)
            {
                i = 0;
                setpos_to_LCD(5, 1);
                if (protect == 3)
                {
                    string_to_LCD("TRUE!   ");
                    flag = 1;
                }
                else
                {
                    string_to_LCD("FALSE!   ");
                    PORTC &= ~(1 << 0);
                }
                protect = 0;
                _delay_ms(1000);

                clear_LCD();
                setpos_to_LCD(0, 0);
                string_to_LCD("Enter password:");
                setpos_to_LCD(5, 1);
            }
        }
    }
}