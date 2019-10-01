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
INT16 flag=0;

ISR(TIMER2_COMP_vect)
{
	OCR2 = flag;
	if (flag == 255)
	{
		flag=0;
	}
	else if (flag == 0)
	{
		flag=0;
	}
	
}
ISR(USART1_RX_vect)
{
	recieve = UDR1;
}
void port_init();
void LED_init();
void Motor_init();
void led_bright();
void motor();
int main(void)
{
	port_init();
	LED_init();
	Motor_init();
	while(1)
    {
		led_bright();
		motor();
		
		//TODO:: Please write your application code 
    }
}
void port_init()
{
	DDRD = 0xCF; // dir(PD7), EN(PD6)
	DDRB |= 0x90; // OC1(PB4), OC2(PB7)
	DDRC |= 0x01;
	DDRG = 0xff;
	USART0_init(MYUBRR);
	USART1_init(MYUBRR1);
	
	
	USART0_Transmit_String("BT-CONNECTED!");
	USART0_Transmit_NewLine();
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
	OCR2 = 0x00;
	TIMSK = 0x80;
	sei();
}
void Motor_init()
{
	PORTC |= 0x01; // MODE1 1/4step
	//timer0, CTC
	TCCR0 |= (0<<FOC0)
	|(0<<WGM00)|(1<<WGM01)
	|(1<<COM00)|(0<<COM01)
	|(0<<CS02)|(1<<CS01)|(1<<CS00);
	//CTC, N=32, Toggle
	OCR0 = 125;
}
void led_bright()
{
	recieve = USART1_Receive();
	if (recieve == '1') // 어두워짐
	{
		flag+=50;
	}
	else if (recieve == '2')
	{
		flag-=50;
	}
}
void motor()
{
		PORTD = 0xCF;
		recieve = USART1_Receive();
		USART0_Transmit(recieve);
		USART0_Transmit_NewLine();
		
		if (recieve == 'u')
		{
			PORTD = 0x80; //dir NonClk, EN - LOW
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
		else if (recieve == 'd')
		{
			PORTD = 0x0f; // EN-LOW, clk-w
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
		else if (recieve == 's')
		{
			PORTD = 0xCf;
			USART0_Transmit_String("STOP");
			USART0_Transmit_NewLine();
		}
}
