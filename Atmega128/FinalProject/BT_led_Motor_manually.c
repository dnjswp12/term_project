/*
 * steppermotor.c
 *
 * Created: 2019-09-28 오후 4:01:13
 *  Author: wonjechung
 */ 
/************************************************************************/
/*			DRV8255 
			---------
		  EN|O		| 12Vcc
		  M0|		| GND
		  M1|		| B2 (BLACK)
		  M2|		| B1 (RED)
   RESET(5v)|		| A1 (BLUE)
   SLEEP(5v)|		| A2 (GREEN)
		STEP|		|
		DIR |		| GND(Vdd-GND)
			---------            
			모터는 검은색-초록색 한 코일, 빨-파 한 코일                                    
                     */
/************************************************************************/
typedef unsigned char INT8;
typedef unsigned int INT16;

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART0.h"  // esp12
#include "USART1.h" // HC-05


char recieve = 0;
INT8 flag=0;
INT8 l_flag=0;
INT8 m_flag=0;
INT8 m_flag2 = 0;
void port_init();
void LED_init();
void Motor_init();
void led_bright();
void motor();

ISR(TIMER2_COMP_vect)
{
	OCR2 = flag;
	
}
ISR(USART1_RX_vect)
{
	recieve = UDR1;
}
ISR(TIMER0_COMP_vect)
{
	++m_flag;
	if (m_flag == 200 )
	{
		m_flag = 0;
		m_flag2+=1;
	}
	if (m_flag2 == 9)
	{
		m_flag2 = 0;
	}
}
int main(void)
{
	port_init();
	LED_init();
	Motor_init();
	while(1)
    {
		motor();
		led_bright(); 
    }
}
void port_init()
{
	DDRD = 0xCF; // dir(PD7), EN(PD6)
	DDRB = 0x90; // OC1(PB4), OC2(PB7)
	DDRC |= 0x01;
	DDRG = 0xff;
	USART0_init(MYUBRR);
	USART1_init(MYUBRR1);
	
	
	USART1_Transmit_String("BT-CONNECTED!");
	USART1_Transmit_NewLine();
	_delay_ms(100);
}
void LED_init()
{
	// FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
	//timer2 - PB7
	//TCCR2 = 0b0 1 11 11011; //f-pwm, n=32, oc set
	TCCR2 |= (0<<FOC2)
	|(1<<WGM20)|(1<<WGM21)
	|(1<<COM20)|(1<<COM21)
	|(0<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2 = 0xFF;
	TIMSK = 0x80;
	sei();
}
void Motor_init()
{
	PORTC |= 0x01; // MODE1 1/4step
	PORTD = 0xCF;
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
void led_bright()
{
	if (recieve == '1') // 어두워짐
	{
		
		if(flag >= 240)
		{
			OCR2 = 0xff;
		}
		else
		{
			flag+=20;
		}
		USART1_Transmit_NewLine();
		UART1_print16bitNumber(flag);
	}
	else if (recieve == '2')
	{
		
		if (flag <= 20)
		{
			flag = 0;
			OCR2 = 0x00;
		}
		else
		{
			flag-=20;
		}
		USART1_Transmit_NewLine();
		UART1_print16bitNumber(flag);
	}
}
void motor()
{
	if (recieve == 'u')
	{
		m_flag = 0;
		m_flag2=0;
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
	else if (recieve == 'd')
	{
		m_flag = 0;
		m_flag2=0;
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
	else if (recieve == 's')
	{
		PORTD = 0xCf;
		USART0_Transmit_String("STOP");
	    USART0_Transmit_NewLine();
	}
}
