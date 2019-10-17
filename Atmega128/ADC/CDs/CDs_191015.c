/*
 * Cds.c
 *
 * Created: 2019-09-29 오후 2:41:37
 *  Author: wonjechung
 */ 
typedef unsigned char INT8;
typedef unsigned int INT16;
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART0.h"

#define ADC_CH 0 // 입력채널
#define REF_VOLTAGE 5.0 //기준전압
#define RESOLUTION 1024.0 // 해상도 10bit
#define R1 10000 // 10kohm
volatile INT16 read_adc;
volatile INT8 adcflag = 0;
volatile INT8 flag = 0;
volatile INT16 ledflag;

float Rc = 0;
INT8 test = 0;

void adc_init(INT8 channel);
float convert_adc(void);
void ledbright(float Rcc);
void LED_init();

ISR(TIMER2_COMP_vect)
{
	OCR2= ledflag ;
}
ISR(USART0_RX_vect)
{
	test = UDR0;
}
/*ISR(ADC_vect)
{
	read_adc = ADCL;
	read_adc += (ADCH<<8);
	adcflag =1;
}*/
int main(void)
{
	USART0_init(MYUBRR);
	_delay_ms(100);
	
	DDRF = 0x00;
	DDRG = 0xff;
	PORTG = 0x00;
	
	adc_init(ADC_CH);
	LED_init();
	float a=0;
    while(1)
    {
		test = USART0_Receive();
	    USART0_Transmit(test);
		USART0_Transmit_NewLine();
		if (test == 'a')
		{
			ADCSRA |= (1<<ADFR);
			while(1)
			{
				a = convert_adc();
				ledbright(a);
			}
		}
		
		
	}
}
							
		


void adc_init(INT8 channel)
{
	asm("CLI");
	ADMUX = (0<<REFS1)|(1<<REFS0); // 외부 avcc 5v 사용
	ADMUX |= (0<<ADLAR); // 10비트 사용
	ADMUX |= channel;
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(0<<ADFR)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	// ADC의 모든 동작 허용, 1로 SET되었을때 ADC시작 , 
	asm("SEI"); // SREG|= 0x80
}
void LED_init()
{
	DDRB |= 0x80; // PB7(OC2)
	// FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
	//timer2 - PB7
	//TCCR2 = 0b0 1 11 11011; //f-pwm, n=32, oc set
	TCCR2 |= (0<<FOC2)
	|(1<<WGM20)|(1<<WGM21)
	|(0<<COM20)|(1<<COM21)
	|(0<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2 = 0xFF;
	TIMSK = 0x80;
	sei();
}
float convert_adc(void)
{
	read_adc = ADCL;
	read_adc += (ADCH<<8);
	//float adc_volt = 0;
	adcflag = 0;
	//adc_volt = (float)read_adc*(REF_VOLTAGE/RESOLUTION);
	//Rc = (R1*adc_volt) / (REF_VOLTAGE - adc_volt);
	Rc = (float)read_adc*240/RESOLUTION;
	
	USART0_Transmit_String("read: ");
	UART0_print32bitNumber(Rc);
	USART0_Transmit_NewLine();
	return Rc;
}
void ledbright(float Rcc)
{
	//char str[2];
	//sprintf(str,"adc volt: %4.2f",Rcc);
	USART0_Transmit_String("Rc: ");
	UART0_print32bitNumber(Rcc);
	USART0_Transmit_NewLine();
	ledflag = Rcc;
	if (Rcc>= 240)
	{
		OCR2 = 0xff;
	}
	else if (Rcc <= 10)
	{
		OCR2 = 0x00;
	}
		/*if (Rcc < 10000) // 밝을때
		{
			OCR2 -=20;
			PORTG = 0x00;
			if (OCR2 == 240)
			{
				OCR2 = 0xff;
			}
		}
		else if (Rcc >15000) // 어두울때
		{
			OCR2 +=20;
			PORTG = 0xff;
			if (OCR2 == 10)
			{
				OCR2 = 0;
			}
		}*/
		ADCSRA |=(1<<ADSC);
		_delay_ms(1000);
}

/*
float read_light(void)
{
	int adc_resol;
	float vout,rcds,lux;
	adc_resol=read_adc;
	vout=(float)((float)adc_resol * 0.0048875855); //OUTPUT VOLTAGE
	rcds = (23500/vout) - 4700;
	lux=(float)pow(10,1-(float)(log10(rcds)-log10(40000))/0.8);
	return lux;
	if(mode==’3′)
	{
		if(pwm<led)
		PORTA=0×00; // free mode OFF
		else 
		PORTA=0xFF; // free mode ON
		if(++pwm>127)
		{ pwm=0; delay_us(250); // 변환시간동안 딜레이
			ADC_I=ADCW; 
			ADC_F=(float)ADC_I*100.0/1023.0;
		led=(char)ADC_F;
		}
	}
}
}*/
