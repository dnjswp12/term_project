/*
 * Cds.c
 *
 * Created: 2019-09-29 오후 2:41:37
 *  Author: wonjechung
 */ 
typedef unsigned char INT8;
typedef unsigned int INT16;
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART0.h"

#define ADC_CH 0 // 입력채널
#define REF_VOLTAGE 5 //기준전압
#define RESOLUTION 1024.0 // 해상도 10bit

volatile INT16 read_adc = 0;
volatile INT8 adcflag = 0;
void adc_init(INT8 channel);


int main(void)
{
	USART0_init(MYUBRR);
	_delay_ms(100);
	DDRF = 0x00;
	DDRG = 0xff;
	
	adc_init(ADC_CH);
	INT8 adc_volt = 0;
	INT8 cnt = 0;
    while(1)
    {
		
		PORTG = 0x00;
		_delay_ms(100);
		
		for (int i=0;i<10;i++)
		{
			read_adc = ADCL;
			read_adc +=  (ADCH<<8);
			adc_volt = (float)read_adc*(REF_VOLTAGE/RESOLUTION);
			
			PORTG = 0xff;
			_delay_ms(100);
			USART0_Transmit_String("adc: ");
			UART0_print32bitNumber(read_adc);
			USART0_Transmit_NewLine();
			
			USART0_Transmit_String("volt: ");
			UART0_print32bitNumber(adc_volt);
			USART0_Transmit_NewLine();
			_delay_ms(300);
			cnt += 1;
		}
		if (cnt == 10)
		{
			ADCSRA |= (1<<ADSC);
			cnt = 0;
		}
		//TODO:: Please write your application code 
    }
}
void adc_init(INT8 channel)
{
	asm("CLI");
	ADMUX = (0<<REFS1)|(1<<REFS0); // 외부 avcc 5v 사용
	ADMUX |= (0<<ADLAR); // 10비트 사용
	ADMUX |= channel;
	ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADFR)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	asm("SEI"); // SREG|= 0x80
}
