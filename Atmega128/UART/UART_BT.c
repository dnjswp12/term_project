/*
 * UART_0.c
 *
 * Created: 2019-09-20 오후 2:50:08
 *  Author: wonjechung
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "UART0.h"
#include "UART1.h" // BT와 연결, 초기 baud 38400
#include <string.h>


int main(void)
{
	DDRG = 0xff;
	USART0_init(MYUBRR);
	USART1_init(MYUBRR1);
	USART0_Transmit_String("dddddd123\n");
	//unsigned char input ;
	char BT[20];
    while(1)
    {
		PORTG = 0x00;
		_delay_ms(10);
		PORTG = 0xff;
		_delay_ms(10);
		USART0_Transmit_String("BT_Phon: \n");
		//USART1_Transmit_String("AT+PSWD=1234\n\r"); // PUTTY 사용할때 \n\r
		rx1_string(BT); // BT 모듈로 부터 문자열 받아옴
		tx0_string(BT); // BT로 부터 받아온 문자열 유아트 0번 시리얼로 뿌림
		USART0_Transmit_NewLine();
		_delay_ms(1000);
		if (!strcmp(BT,"1"))
		{
			PORTG = 0xff;
			_delay_ms(1000);
		}
		else
		{
			PORTG = 0x00;
		}
		
    }
}
//ABCDEFGHIJKLMNOPQRST
