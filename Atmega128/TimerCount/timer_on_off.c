/*
 * GccApplication2.c
 *
 * Created: 2019-10-30 오전 10:05:03
 *  Author: user
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "uart1.h"
#include <avr/interrupt.h>

volatile int m_flag=0;

volatile char test=0;

ISR(TIMER0_COMP_vect)
{
	m_flag++;
	
	//USART1_Transmit_String("timer:");
	//UART1_print16bitNumber(m_flag);
	//USART1_Transmit_NewLine();
	// ocr=124>>interrupt 발생 ctc이므로 int 2번 1개의 펄스 (현재 1ms, 1000hz)
}
ISR(USART1_RX_vect)
{
	test = UDR1;	
}
int main(void)
{
	DDRB |=0x10;
	USART1_init(MYUBRR1);
	_delay_ms(100);
	//timer0, CTC
	TCCR0 |= (0<<FOC0)
	|(0<<WGM00)|(1<<WGM01)
	|(0<<COM00)|(0<<COM01)
	|(0<<CS02)|(1<<CS01)|(1<<CS00);
	//CTC, N=32, Toggle
	//OCR0 = 12; //10ms 10khz
	OCR0 = 124; // 1ms(1000hz)
	TIMSK |= (1<<OCIE0);
	sei(); // output compare match
	USART1_Transmit_String("TEST!");
	USART1_Transmit_NewLine();
    while(1)
    {
		test = USART1_Receive();
		if (test == 'a')
		{
			TCCR0 |=(1<<COM00)|(0<<COM01);
			USART1_Transmit_String("timer on: ");
			UART1_print16bitNumber(TCNT0);
			USART1_Transmit_NewLine();
		}
		else if (test == 'b')
		{
			TCCR0 &=(0<<COM00)|(0<<COM01);
			USART1_Transmit_String("timer off: ");
			UART1_print16bitNumber(TCNT0);
			USART1_Transmit_NewLine();
		}
		else
		{
			test = 0;
		
		}
        //TODO:: Please write your application code 
    }
}
