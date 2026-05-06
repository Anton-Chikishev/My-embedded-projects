/*
 * Управление светодиодами с помощью кнопок (Atmega32)
 * Частота МК: 1 МГц
 */

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

int main(void)
{
    DDRC |= (1 << 2);   // PC2 выход (зелёный)
    DDRD |= (1 << 4);   // PD4 выход (синий)
    DDRB |= (1 << 6);   // PB6 выход (красный)

    DDRB &= ~(1 << 2);  // PB2 вход (K1)
    PORTB |= (1 << 2);  // подтяжка
    DDRA &= ~(1 << 4);  // PA4 вход (K2)
    PORTA |= (1 << 4);  // подтяжка
    DDRC &= ~(1 << 4);  // PC4 вход (K3)
    PORTC |= (1 << 4);  // подтяжка

    while (1)
    {
        if (PINA & (1 << 4))           // K2 отжата
        {
            if (~PINB & (1 << 2))      // K1 нажата
            {
                if (~PINC & (1 << 4))  // K3 нажата
                {
                    PORTD = 0b00010000;
                    PORTC = 0b00000100;
                    PORTB = 0b01000000;
                    _delay_ms(200);

                    PORTD = 0b00000000;
                    PORTC = 0b00000000;
                    PORTB = 0b00000000;
                    _delay_ms(200);
                }
            }
        }
    }
}