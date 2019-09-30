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
#include "UART0.h"
INT8 ch=0;
INT8 cnt=0;

ISR(USART0_RX_vect)
{
	ch = UDR0;
}
void port_init();
void timer_init();
int main(void)
{
	port_init();
	timer_init();
	while(1)
    {
		PORTD = 0x88;
		ch = USART0_Receive();
		USART0_Transmit(ch);
		USART0_Transmit_NewLine();
		
		if (ch == 'u')
		{
			PORTD = 0x80;
			USART0_Transmit_String("UP");
			USART0_Transmit_NewLine();
			/*if (cnt == 200)
			{
				cnt = 0; 
			}
			for (int i=0;i<200;i++)
			{
				PORTA = 0xff;
				_delay_ms(2);
				PORTA = 0x00;
				_delay_ms(2);
			}*/
		}
		else if (ch == 'd')
		{
			PORTD = 0x00;
			USART0_Transmit_String("DOWN");
			USART0_Transmit_NewLine();
			/*if (cnt == 200)
			{
				cnt = 0;
			}
			for (int a=0;a<200;a++)
			{
				PORTA = 0xff;
				_delay_ms(2);
				PORTA = 0x00;
				_delay_ms(2);
			}*/
		}
		else if (ch == '0')
		{
			PORTD = 0x88;
			USART0_Transmit_String("STOP");
			USART0_Transmit_NewLine();
		}
		//TODO:: Please write your application code 
    }
}
void port_init()
{
	USART0_init(MYUBRR);
	
	DDRD = 0x88; // dir, EN(PD3)
	DDRG = 0xff;
	DDRB |= 0x10; // step
	USART0_Transmit_String("Motor test");
	USART0_Transmit_NewLine();
	_delay_ms(100);
}
void timer_init()
{
	TCCR0 |= (0<<FOC0)
		     |(0<<WGM00)|(1<<WGM01)
			 |(1<<COM00)|(0<<COM01)
			 |(0<<CS02)|(1<<CS01)|(1<<CS00);
			 //CTC, N=32, Toggle
	OCR0 = 125;
}
