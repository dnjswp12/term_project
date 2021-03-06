
#define F_CPU 8000000UL
#define BAUD  9600 
#define U2X_S  2     // U2X --> 1 or 2 
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1) 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Usartheader.h"


int main(void)
{
   INT8 rx_buf;
   DDRG = 0xff;
   USART0_init(MYUBRR);
   _delay_ms(100);
   PORTG = 0xff;

    while(1)
    {
      rx_buf = USART0_Receive();
      if (rx_buf == 9)
      {
         PORTG = 0x00;
         _delay_ms(100);
      }
      if (rx_buf == 0)
      {
         PORTG = 0xff;
         _delay_ms(100);
      }
        //TODO:: Please write your application code 
    }
}

