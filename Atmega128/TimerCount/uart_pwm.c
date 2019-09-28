/*
 * LED_PWM.c
 *
 * Created: 2019-09-25 오후 6:17:59
 *  Author: wonjechung
 */ 
typedef unsigned char INT8;
typedef unsigned int INT16;

#define F_CPU 8000000UL
#define BAUD 9600
#define U2X_S 2 //U2X --> 1 or 2
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1)

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART0.h"
 


char recieve = 0;
INT16 flag=0;

ISR(TIMER0_COMP_vect)
{
	OCR0 = flag;
	if (flag == 255)
	{
		flag = 255;
		rx0_string("MAX");
		USART0_Transmit_NewLine();
	}
	else if (flag == 0)
	{
		flag =0;
		rx0_string("MIN");
		USART0_Transmit_NewLine();
	}
}
ISR(USART0_RX_vect)
{
	recieve = UDR0;
}
int main(void)
{
	
	//INT8 length=0;
	
	DDRB |= 0x10; //PB4
	DDRG = 0xff;
	//uart
	USART0_init(MYUBRR);
	sbi(UCSR0B, RXCIE); // RX
	//timer 0
	TCCR0 = 0b011111011; //f-pwm, n=32, oc set
	OCR0 = 0xff;
	TIMSK = 0x02;
	sei();
	
    while(1)
    {
		recieve = USART0_Receive();
		if (recieve == '1') // 어두워짐
		{
			flag+=5;
		}
		else if (recieve == '2')
		{
			flag-=5;
			
		}
        //TODO:: Please write your application code 
    }
}

