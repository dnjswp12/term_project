
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "/Users/wonjechung/Documents/Atmel Studio/6.2/Final/Final/header/USART1.h"

void LED_init()
{
	// FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
	//timer2 - PB7
	//TCCR2 = 0b0 1 11 11011; //f-pwm, n=32, oc clear
	TCCR2 |= (0<<FOC2)
	|(1<<WGM20)|(1<<WGM21)
	|(0<<COM20)|(1<<COM21)
	|(0<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2 = 0x00;
	TIMSK |= 0x80;
	sei();
}
char LED_bright(char lf)
{
	if (lf <= 10)
	{
		lf = 0;
		OCR2 = 0x00;
	}
	else
	{
		lf-=10;
	}
	return lf;
}
char LED_dark(char lf)
{
	if(lf >= 240)
	{
		OCR2 = 255;
	}
	else
	{
		lf+=10;
	}
	return lf;
}
char auto_led(char led_adc)
{
	//char str[2];
	//sprintf(str,"adc volt: %4.2f",Rcc);
	//USART1_Transmit_String("Rc: ");
	//UART1_print16bitNumber(led_adc);
	//USART1_Transmit_NewLine();
	if (led_adc>= 240)
	{
		OCR2 = 0xff;
	}
	else if (led_adc <= 10)
	{
		OCR2 = 0x00;
	}
	//adcflag = 0;
	ADCSRA |=(1<<ADSC);
	_delay_ms(1000);
	return led_adc;
}
