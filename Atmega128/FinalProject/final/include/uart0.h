/*
 * UART0.h
 *
 * Created: 2019-09-20 오후 2:51:36
 *  Author: wonjechung
 */ 


#ifndef UART0_H_
#define UART0_H_

typedef unsigned char  INT8;
typedef unsigned int   INT16;

#define sbi(reg,bit)    reg |= (1<<(bit))	  // Set "bit"th bit of Register "reg"
#define cbi(reg,bit)    reg &= ~(1<<(bit))

#define BAUD 9600
#define U2X_S 2 //U2X --> 1 or 2
#define MYUBRR ((F_CPU*U2X_S)/(16L*BAUD)-1)

void USART0_init(INT16 ubrr)
{
	// Set baud rate
	UBRR0H=(INT8)(ubrr>>8);
	UBRR0L=(INT8)ubrr;       
	// Enable U2X
	if(U2X_S==2)
		sbi(UCSR0A,U2X);
	else
		cbi(UCSR0A,U2X);
		
	//Enable Tx & Rx Port
	sbi(UCSR0B,RXEN); // RX핀 사용 PE0
	sbi(UCSR0B,TXEN); // TX핀 사용 PE1
	
	
	//Enable interrupt
	sbi(UCSR0B,RXCIE); //RX
	sbi(UCSR0B,TXCIE); //TX
	//sbi(UCSR0B,UDRIE); //UDR Empty
	
	//Set frame format : default값으로 세팅되어 있음
	/*
	cbi(UCSR0C,UMSEL); // 0: 비동기 1: 동기
	cbi(UCSR0C,USBS); // 스톱비트 0: 1bit 1: 2bit
	cbi(UCSR0C,UPM01); // NO parity 패러티 설정
	cbi(UCSR0C,UPM00);
	cbi(UCSR0B,UCSZ02);//8bit 데이터 비트 설정
	sbi(UCSR0C,UCSZ01);
	sbi(UCSR0C,UCSZ00);
	*/
}


void USART0_Transmit(char data)
{
	//wait for empty transmit buffer
	while (!(UCSR0A & 0x20)) // UDRE bit -> data resist empty : 1
	{
		;
	}
	//Put "data" into buffer,transmit the data
	UDR0 = data;
}

void USART0_Transmit_NewLine(void)
{
	USART0_Transmit(0x0A); // LF
	USART0_Transmit(0x0D); // CR
}

void USART0_Transmit_String(char *str)
{
	while(*str != '\0')
	{
		if(*str == '\n')
		USART0_Transmit_NewLine();
		else
		USART0_Transmit(*str);
		str++;
	}
}

char USART0_Receive(void)
{
	//Wait for data to be received
	while(!(UCSR0A&0x80)) //RXC bit
	{
		;	
	}
	//Get "data" from buffer, return the data
	return UDR0;
	
}
void rx0_string(char *str)
{
	int i=0;
	while ((str[i] = USART0_Receive()) != '\n')
	{
		i ++;
	}
	str[i] = '\0';
}
void tx0_string(char *str)
{
	int i=0;
	while (str[i] != '\0')
	{
		USART0_Transmit(str[i]);
		i++;
	}
}
void UART0_print16bitNumber(uint16_t no)               // 숫자를 문자열로 변환하여 송신, 8비트 int형
{
	char numString[6] = "0";
	int i, index = 0;
	if(no > 0)// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)
	{                           // 변환된 문자열 출력
		USART0_Transmit(numString[i]);
	}
}
void UART0_print32bitNumber(uint32_t no)               // 숫자를 문자열로 변환하여 송신, 8비트 
{
	char numString[11] = "0";
	int i, index = 0;
	if(no > 0)// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		} 
			numString[i] ='\0';
			index = i-1;
	} 
		for(i=index; i>=0; i--)// 변환된 문자열 출력 
		{
			USART0_Transmit(numString[i]);
		}
}

#endif /* UART0_H_ */
