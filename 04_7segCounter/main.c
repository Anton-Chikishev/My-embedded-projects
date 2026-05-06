/*
 * 7-сегментный счётчик с длинным нажатием (Atmega32)
 * Короткое нажатие PA2 +1, длительное (>2 сек) PA2 -1
 * Частота МК: 1 МГц
 */

#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
    // Массив цифр для семисегментного индикатора (общий анод)
    char led[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    signed char n = 0;     // текущее значение счета (0-9)

    // Инициализация портов
    DDRD = 0b11111111;      // порт D — выход на индикатор
    PORTD = ~led[0];        // вывести 0 на индикатор

    DDRA &= ~((1 << 2) | (1 << 3));
    PORTA |= (1 << 2) | (1 << 3);   // подтяжка

    while (1)
    {
        // проверка кнопки на PA2
        if (!(PINA & (1 << 2)))
        {
            _delay_ms(50);  // антидребезг
            if (!(PINA & (1 << 2)))
            {
                unsigned int counter = 0;
                while (!(PINA & (1 << 2)))
                {
                    _delay_ms(50);
                    counter++;
                    if (counter >= 40)          // > 2 секунд
                    {
                        while (!(PINA & (1 << 2)));
                        if (n > 0) n--;
                        else n = 9;
                        PORTD = ~led[n];
                        break;
                    }
                }
                if (counter < 40 && counter > 0) // короткое нажатие
                {
                    n++;
                    if (n > 9) n = 0;
                    PORTD = ~led[n];
                }
            }
        }
    }
}