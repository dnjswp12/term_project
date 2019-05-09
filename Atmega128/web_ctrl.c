typedef unsigned char INT8;
typedef unsigned int INT16;

#define F_CPU 8000000UL
#define BAUD  9600
#define U2X_S  2     // U2X --> 1 or 2
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Usartheader.h"

INT8 rx_buf;
INT16 flag;
//int str1;

ISR(USART0_RX_vect)
{
   flag=1;
   rx_buf = UDR0;
   if (flag==0)
   {
      rx_buf=0;
      //OCR3BL = 0;
   }
}

ISR(TIMER0_COMP_vect)
{
   OCR0 = OCR0;
}

void led_on_off() // 전등 on/off
{
   if (rx_buf == '1') // 웹서버에서 ESP-12로 보낸 신호를 받아 LED ON
   {
      PORTC = 0x00;
   }
   else if (rx_buf == '3') // LED OFF
   {
      PORTC = 0xff;
   }
}

void led_pwm() // 전등 밝기 조절
{
   if (rx_buf == '0') // LED 밝기 UP
   {
      if (flag==1)
      {
         OCR3BL = 10;
         //OCR0 = 10;
         flag=0;
      }      
   }
   else if(rx_buf == '2') // led가 on 되었을때만 ocr값이 변하여 led 밝아짐
   {
      if (flag==1)
      {
         OCR3BL = 80;
         //OCR0 = 80;
         flag=0;
      }
   }
   
   else if(rx_buf == '4')
   {
      if (flag==1)
      {
         OCR3BL = 150;
         //OCR0 = 150;
         flag=0;
      }
      
   }
   
   else if(rx_buf == '6')
   {
      if (flag==1)
      {
         OCR3BL = 255;
         //OCR0 = 255;
         flag=0;
      }
      
   }
}

void motor() // 블라인드 제어
{
   if (rx_buf == 'R') // 내림
   {
      TCCR0 = 0b01111100; // F-PWM, OC Set, N=256
      OCR0 = 190;
      sbi(PORTB,4);
      cbi(PORTB,7);
   }
   else if (rx_buf == 'F') // 올림
   {
      TCCR0 = 0b01101100; // F-PWM, OC Set, N=256
      OCR0 = 190;
      cbi(PORTB,4);
      sbi(PORTB,7);
   }
   else if (rx_buf == 'S') // 정지
   {
      TCCR0 = 0;
      cbi(PORTB,4);
      cbi(PORTB,7);
   }
}

void port_init() // port 초기화 
{
   DDRC = 0xff;
   DDRF = 0xcf;
   DDRB |= 0x90;
   DDRE = 0x10;
} 

void timer_init() // Timer 초기화
{
   //TCCR0 = 0b01111100; // F-PWM, OC Set, N=256
   //OCR0 =0x00;
   //cbi(PORTB,7); // PB7=0
   TCCR3A |= (0<<COM3A1)|(0<<COM3A1)
         |(1<<COM3B1)|(1<<COM3B0)
         |(0<<COM3C1)|(0<<COM3C0)
         |(0<<WGM31)|(1<<WGM30);         //WGM11=1, WGM10=0 F-PWM
   TCCR3B |= (1<<ICNC1)|(0<<ICES1)
         |(0<<WGM33)|(1<<WGM32)         //WGM13=1, WGM12=1 F-PWM
         |(0<<CS32)|(1<<CS31)|(0<<CS30); // N=256
   TCCR3C |= 0x0000;
   OCR3BL = 0x0000;
   //ICR1 = 199;
   
   TIMSK |= 0x02;
   sei();
}

int main(void)
{
   port_init();
   timer_init();
   
   USART0_init(MYUBRR);
   _delay_ms(100);
   
   PORTC = 0xff;
   
    while(1)
    {
      led_on_off();
      led_pwm();
      motor();
    }
}
