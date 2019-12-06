
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include "/Users/wonjechung/Documents/Atmel Studio/6.2/Final/Final/header/USART1.h"

int m_flag = 0;
int y; 
int sum=0;
ISR(TIMER0_COMP_vect)
{
	m_flag+=1;
}

// 모터 1/4 step : 200펄스에 90도임. 0.45*800 = 360;
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
	//OCR0 = 12; //10ms 10khz
	OCR0 = 124; // 1ms(1000hz) 
	TIMSK |= (1<<OCIE0);
	sei(); // output compare match
}

int average_adc(int avetemp, int r)
{
	int result = 0;
	sum+=avetemp;
	if (r == 10)
	{
		result = sum/10;
		sum = 0;
	}
	return result;
}

void Motor_up()
{
	_delay_ms(100);
	m_flag =0;
	while(m_flag<1600)
	{
		PORTD &= 0x80;
		if (m_flag>=1600)
		{
			PORTD |=0xC0;
		}
	}
}

void Motor_down()
{
	m_flag =0;
	
	while(m_flag<1600)
	{
		PORTD &= 0x00;
		if (m_flag>=1600)
		{
			PORTD |=0xC0;
		}
	}
}
void Motor_stop()
{
	USART1_Transmit('s');//motor stop
	PORTD = 0xCf;
}
void Motor_auto(char m_adc)
{
	if (m_adc <= 60)
	{
		m_flag = 0;
		for (; m_flag<=4800;)
		{
			PORTD &= 0x80;
			if (m_flag >= 4800)
			{
				y=1;
				
				PORTD |=0xC0;
			}
			if (y == 1)
			{
				y = 0;
				PORTD |=0xC0;
				break;
			}
		}
			
	}
	
	else if (m_adc >= 210)
	{
		for (m_flag=0; m_flag<=4800;)
		{
			PORTD &= 0x00;
			if (m_flag >= 4800)
			{
				y=1;
				PORTD |=0xC0;
			}
			if (y == 1)
			{
				y = 0;
				PORTD |=0xC0;
			}
		}
	 }
	
	else
	{
		PORTD = 0xCf;
	}
}
