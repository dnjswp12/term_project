/*
 * PWM.c
 *
 * Created: 2019-09-23 오후 3:22:25
 *  Author: wonjechung
 */ 

typedef unsigned char INT8; 
typedef unsigned int INT16;

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= 0x10; //PB4
	TCCR0 = 0b01111100; //f-pwm,OC set, N = 64.. 
	OCR0 = 0xFF;
    while(1)
    {
		OCR0 +=5 ;
		_delay_ms(100);
        //TODO:: Please write your application code 
    }
}
