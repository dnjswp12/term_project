
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "/Users/wonjechung/Documents/Atmel Studio/6.2/Final/Final/header/USART1.h"

char m_flag=0;
char m_flag2=0;
ISR(TIMER0_COMP_vect)
{
	++m_flag;
	if (m_flag == 200)
	{
		m_flag = 0;
		m_flag2+=1;
	}
	if (m_flag2 == 9)
	{
		m_flag2 = 0;
	}
}
void Motor_init()
{
	PORTC |= 0x01; // MODE1 1/4step
	PORTD |= 0xC0;
	//timer0, CTC
	TCCR0 |= (0<<FOC0)
	|(0<<WGM00)|(1<<WGM01)
	|(1<<COM00)|(0<<COM01)
	|(0<<CS02)|(1<<CS01)|(1<<CS00);
	//CTC, N=32, Toggle
	OCR0 = 125;
	TIMSK |= (1<<OCIE0);
	sei(); // output compare match
}

void Motor_up()
{
	
	m_flag = 0;
	m_flag2 = 0;
	while(m_flag2<10)
	{
		PORTD &= 0x80;
		if (m_flag2==8)
		{
			m_flag2=0;
			break;
		}
	}
	PORTD |=0xC0;
}
void Motor_down()
{
	m_flag = 0;
	m_flag2 = 0;
	while(m_flag2<10)
	{
		PORTD &= 0x00;
		if (m_flag2==8)
		{
			m_flag2=0;
			break;
		}
	}
	PORTD |=0xC0;
}
void Motor_stop()
{
	USART1_Transmit('s');//motor stop
	PORTD = 0xCf;
}
