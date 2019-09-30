/*
 * steppermotor.c
 *
 * Created: 2019-09-28 오후 4:01:13
 *  Author: wonjechung
 */ 
/************************************************************************/
/*			DRV8255 
		   	   ---------
		      EN|O	 | 12Vcc
		      M0|		 | GND
		      M1|		 | B2 (BLACK)
		      M2|		 | B1 (RED)
   RESET(5v)|		 | A1 (BLUE)
   SLEEP(5v)|		 | A2 (GREEN)
		    STEP|		 |
		    DIR |	   | GND(Vdd-GND)
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
ISR(USART0_RX_vect)
{
	ch = UDR0;
}
int main(void)
{
	USART0_init(MYUBRR);
	
	DDRD = 0x84; // dir, en
	DDRG = 0xff;
	DDRA = 0xff; // step
	PORTD = 0x80;
	USART0_Transmit_String("Motor test");
	USART0_Transmit_NewLine();
	_delay_ms(100);
	while(1)
    {
		ch = USART0_Receive();
		USART0_Transmit(ch);
		USART0_Transmit_NewLine();
		
		if (ch == 'u')
		{
			PORTD = 0x80;
			USART0_Transmit_String("UP");
			USART0_Transmit_NewLine();
			for (int i=0;i<200;i++)
			{
				PORTA = 0xff;
				_delay_ms(2);
				PORTA = 0x00;
				_delay_ms(2);
			}
		}
		else if (ch == 'd')
		{
			PORTD = 0x00;
			USART0_Transmit_String("DOWN");
			USART0_Transmit_NewLine();
			for (int a=0;a<200;a++)
			{
				PORTA = 0xff;
				_delay_ms(2);
				PORTA = 0x00;
				_delay_ms(2);
			}
		}
		else if (ch == '0')
		{
			USART0_Transmit_String("STOP");
			USART0_Transmit_NewLine();
			PORTD = 0x08;
		}
			
					
				
		}
			

		
		
		
		
        //TODO:: Please write your application code 
    }
