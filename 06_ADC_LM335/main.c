/*
 * Измерение температуры (Atmega32 + LM335)
 * Вывод кода АЦП и температуры на LCD
 * Частота МК: 8 МГц
 */

#include <avr/io.h>
#include "lcd1602wire4.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>

float cod, volt, temp;
char str[12];

// Инициализация АЦП
void ADC_ini(void)
{
    ADCSRA |= (1 << ADEN);                 // включить АЦП
    ADCSRA |= (1 << ADPS1) | (1 << ADPS2); // делитель /64 (125 кГц при 8 МГц)
    ADMUX |= (1 << REFS0);                 // опорное AVCC (5 В)
    ADMUX |= (1 << MUX0) | (1 << MUX1);    // канал ADC3 (PA3)
}

// Запуск преобразования, результат в cod
void ADCconvert(void)
{
    ADCSRA |= (1 << ADSC);                 // старт
    while (ADCSRA & (1 << ADSC));          // ждать окончания
    cod = ADC;                             // 10-битный результат
}

int main(void)
{
    LCD_ini();
    ADC_ini();

    setpos_to_LCD(0, 0);
    string_to_LCD("Cod:");
    setpos_to_LCD(0, 1);
    string_to_LCD("Voltage:");

    while (1)
    {
        ADCconvert();

        // вывод кода АЦП
        setpos_to_LCD(5, 0);
        sprintf(str, "%.0f", cod);
        string_to_LCD(str);
        string_to_LCD("     ");

        // преобразование в напряжение (5 В / 1024 = 0.00489)
        volt = cod * 0.00489;

        // вывод напряжения
        setpos_to_LCD(9, 1);
        sprintf(str, "%.2f", volt);
        string_to_LCD(str);
        string_to_LCD("     ");
    }
}