

typedef unsigned char INT8;
typedef unsigned int INT16;
#define F_CPU 8000000
#define BAUD 9600
#define U2X_S 2 // U2X à 1 or 2
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART_ch0.h"

INT8 data = 0;

ISR(USART0_RX_vect)
{
   data = UDR0;
   
}

int main(void)
{
   DDRD = 0xff;
   USART0_Init ( MYUBRR );
   // Enable interrupt
   sbi(UCSR0B, RXCIE); // RX
   sei( );
   _delay_ms(100);
   
   while(1)
   {
      
      if (data == '1' || data == 1)
      {
         PORTD = 0b01111111;
         data = 0xff;
      }
      
      else if (data == '3')
      {
         PORTD = 0b10111111;
         data = 0xff;
      }
      
      else if (data == '0' || data == 0)
      {
         PORTD = 0b11011111;
         data = 0xff;
      }
      
      else if (data == '2')
      {
         PORTD = 0b11001111;
         data = 0xff;
      }
      
      else if (data == '4')
      {
         PORTD = 0b11000111;
         data = 0xff;
      }
      
      else if (data == '6')
      {
         PORTD = 0b11000011;
         data = 0xff;
      }
      
      else if (data == 'F')
      {
         PORTD = 0b11111101;
         data = 0xff;
      }
      
      else if (data == 'S')
      {
         PORTD = 0b11111110;
         data = 0xff;
      }
      
      else if (data == 'R')
      {
         PORTD = 0b00000000;
         data = 0xff;
      }
      
      
      
      else
      {
         ;
      }
      
      
   }
}
