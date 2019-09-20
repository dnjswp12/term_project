/*
 * UART_INT.c
 *
 * Created: 2019-09-20 오후 5:26:55
 *  Author: wonjechung
 */ 

typedef unsigned char INT8;
typedef unsigned int INT16;

#define F_CPU 8000000
#define BAUD 9600
#define U2X_S 2 //U2X --> 1 or 2
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1)


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART0.h"

INT8 keyscan,rx;
ISR(USART0_RX_vect)
{
	keyscan = UDR0;
}

int main(void)
{
	USART0_init(MYUBRR);
	DDRG = 0xff;
	PORTG = 0xff;
	_delay_ms(100);
	PORTG = 0x00;
	_delay_ms(100);
	 sbi(UCSR0B, RXCIE); // RX
	 sei( );
	 _delay_ms(100);
	USART0_Transmit_String("INT_test\n");
	
    while(1)
    {
		if (keyscan == '0')
		{
			USART0_Transmit_String("LED OFF!\n");
			PORTG = 0xff;
			keyscan = 0xff;
		}
		else if(keyscan == '1')
		{
			USART0_Transmit_String("LED ON!\n");
			PORTG = 0x00;
			keyscan = 0xff;
		}
        //TODO:: Please write your application code 
    }
}
