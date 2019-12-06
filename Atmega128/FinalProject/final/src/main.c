/*
 * Final.c
 *
 * Created: 2019-10-18 오후 5:43:48
 *  Author: wonjechung
 */ 
/************************************************************************/
/*								PORT Initialize   
							     pin Assignment
							a4988		    Atmega128		
							dir					PD7
							EN					PD6
							step				PB4
							MS2(1/4)			PC0
							RESET - SLEEP		 - 
  
							LED					PB7
  
							HC05				UART0
							  TX				PE0(RX)
							  RX				PE1(TX)
  
						    ESP12				UART1
							 TX					PD2(RX)
							 RX					PD3(TX)
							 GND

							CDs 				PF0(ADC0) */
/************************************************************************/
typedef unsigned char INT8;
typedef unsigned int INT16;

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "/Users/wonjechung/Documents/Atmel Studio/6.2/Final/Final/header/UART0.h"
#include "/Users/wonjechung/Documents/Atmel Studio/6.2/Final/Final/header/USART1.h"

#define ADC_CH 0 // 입력채널


INT8 mode = 0;
volatile INT16 read_adc = 0;
INT8 adcflag = 0;

ISR(USART0_RX_vect)
{
	mode = UDR0;
	if (mode == '7')
	{
		adcflag = 0;
	}
}
ISR(TIMER2_COMP_vect)
{
	OCR2;
}
ISR(ADC_vect)
{
	read_adc = ADCL;
	read_adc += (ADCH<<8);
	adcflag = 1;
}
//init
void Motor_init();
void LED_init();
void port_init();
void adc_init(INT8 channel);

//etc
int average_adc(int avetemp,int r);

//func
void Motor_up();
void Motor_down();
void Motor_stop();
char LED_bright(char lf);
char LED_dark(char lf);
char auto_led(char led_adc);
float convert_adc(INT16 adc);
void Motor_auto(char m_adc);
int uartflag(char mooode);

int main()
{
	port_init();
	Motor_init();
	LED_init();
	adc_init(ADC_CH);
	
	USART0_init(MYUBRR);
	USART1_init(MYUBRR1);
	
	char adctemp=0;
	int adctemp2 = 0;
	int stateflag = 0;
	int test2 = 0; // 평균값을 내기 위한 카운트
	
	while(1)
	{
		stateflag = uartflag(mode);
		if (stateflag == 1) // 어둡게
		{
			OCR2=LED_bright(OCR2);
		}
		else if (stateflag == 2) // 밝게  
		{
			OCR2=LED_dark(OCR2);
		}
		else if (stateflag == 4)
		{
			Motor_up();
		}
		else if (stateflag == 8)
		{
			Motor_down();
		}
		else if (stateflag == 16)
		{
			USART1_Transmit('a');
			for (;adcflag<2;)
				{
					++test2;
					adctemp = convert_adc(read_adc);
					adctemp2=average_adc(adctemp,test2);
					adcflag = 1;
					
					OCR2=auto_led(adctemp);
					if (test2 == 10 )
					{
						Motor_auto(adctemp2);
						test2 = 0;
					}
					if (adcflag == 0)
					{
						//USART1_Transmit_String("close");
						break;
					}
					//USART1_Transmit_String("main: ");
					//UART1_print16bitNumber(adctemp);
					//USART1_Transmit_NewLine();
				}
				
				
		}

			// auto mode
		}
}



void port_init()
{
	//ADC
	DDRF = 0xFE; // PF0 (ADC0)
	
	//MOTOR
	DDRD = 0xCF; // dir(PD7), EN(PD6)
	DDRB = 0x90; // OC1(PB4), OC2(PB7)
	DDRC |= 0x01; // MS2 (1/4step)
	
	//LED
	DDRB |= 0x80; // PB7(OC2)
	
	DDRG = 0xff;
	USART0_init(MYUBRR);
	USART1_init(MYUBRR1);
	
	USART0_Transmit_String("BT-CONNECTED!");
	USART0_Transmit_NewLine();
	_delay_ms(100);
}
int uartflag(char mooode)
{
	int U_flag = 0;
	
	if (mooode == '1') // 어둡게
	{
		USART1_Transmit('d');// led darkness
		U_flag |= 0x01; //1
	}
	else if (mooode == '2') // 밝게
	{
		USART1_Transmit('b');//led bright
		U_flag |= 0x02; // 2
	}
	else if (mooode == '3') // 올리기
	{
		USART1_Transmit('u');// motor up
		U_flag |= 0x04; // 4
	}
	else if (mooode == '4') // 내리기
	{
		USART1_Transmit('d'); // control motor down
		U_flag |= 0x08; // 8
	}
	else if (mooode == '5') // 자동모드
	{
		U_flag |= 0x10; // 16
	}
	else if (mooode == '6')
	{
		U_flag |= 0x20; // 32
	}
	else
	{
		U_flag = 0;
	}
	
	 return U_flag;
}
