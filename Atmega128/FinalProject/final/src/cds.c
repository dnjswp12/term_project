/*
 * Cds.c
 *
 * Created: 2019-10-19 오후 4:55:50
 *  Author: wonjechung
 */ 
typedef unsigned char INT8;
typedef unsigned int INT16;

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ADC_CH 0 // 입력채널
#define REF_VOLTAGE 5.0 //기준전압
#define RESOLUTION 1023.0 // 해상도 10bit

void adc_init(INT8 channel)
{
	asm("CLI");
	ADMUX = (0<<REFS1)|(1<<REFS0); // 외부 avcc 5v 사용
	ADMUX |= (0<<ADLAR); // 10비트 사용(0)
	ADMUX |= channel;
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(0<<ADFR)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	// ADC의 모든 동작 허용, 1로 SET되었을때 ADC시작 , 프리러닝 , 인터럽트 허용비트 , 분주비 
	asm("SEI"); // SREG|= 0x80
}
float convert_adc(INT16 adc)
{
	float Rc=0;
	Rc = (float)adc*240/RESOLUTION;
	//USART1_Transmit_String("after: ");
	//UART1_print16bitNumber(Rc);
	//USART1_Transmit_NewLine();
	return (char)Rc;
}
