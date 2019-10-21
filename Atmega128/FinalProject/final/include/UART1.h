/*
 * UART_1.h
 *
 * Created: 2019-09-19 오후 6:36:37
 *  Author: wonjechung
 */ 


#ifndef UART1_H_
#define UART_1H_
typedef unsigned char  INT8;
typedef unsigned int   INT16;

#define sbi(reg,bit)    reg |= (1<<(bit))	  // Set "bit"th bit of Register "reg"
#define cbi(reg,bit)    reg &= ~(1<<(bit))

#define BAUD1 9600
#define U2X_S 2 //U2X --> 1 or 2
#define MYUBRR1 ((F_CPU*U2X_S)/(16L*BAUD1)-1)

void USART1_init(INT16 ubrr)
{
	// Set baud rate
	UBRR1H=(INT8)(ubrr>>8);
	UBRR1L=(INT8)ubrr;       
	// Enable U2X
	if(U2X_S==2)
		sbi(UCSR1A,U2X);
	else
		cbi(UCSR1A,U2X);
		
	//Enable Tx & Rx Port
	sbi(UCSR1B,RXEN); // RX핀 사용 PD2
	sbi(UCSR1B,TXEN); // TX핀 사용 PD3
	
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
	
	//Enable interrupt
	sbi(UCSR1B,RXCIE); //RX
	//sbi(UCSR0B,TXCIE); //TX
	//sbi(UCSR0B,UDRIE); //UDR Empty
}


void USART1_Transmit(char data1)
{
	//wait for empty transmit buffer
	while (!(UCSR1A & 0x20)) // UDRE bit -> data resist empty : 1
	{
		;
	}
	//Put "data" into buffer,transmit the data
	UDR1 = data1;
}

void USART1_Transmit_NewLine(void)
{
	USART1_Transmit(0x0A); // LF
	USART1_Transmit(0x0D); // CR
}

void USART1_Transmit_String(char *str1)
{
	while(*str1 != '\0')
	{
		if(*str1 == '\n')
		USART1_Transmit_NewLine();
		else
		USART1_Transmit(*str1);
		str1++;
	}
}

char USART1_Receive(void)
{
	//Wait for data to be received
	while(!(UCSR1A&0x80)) //RXC bit
	{
		;	
	}
	//Get "data" from buffer, return the data
	return UDR1;
	
}
void rx1_string(char *str1)
{
	int i=0;
	while ((str1[i] = USART1_Receive()) != '\n')
	{
		i ++;
	}
	str1[i] = '\0';
}
void tx1_string(char *str1)
{
	int i=0;
	while (str1[i] != '\0')
	{
		USART1_Transmit(str1[i]);
		i++;
	}
}
void UART1_print16bitNumber(uint16_t no)               // 숫자를 문자열로 변환하여 송신, 8비트 int형
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
		USART1_Transmit(numString[i]);
	}
}
void UART1_print32bitNumber(uint32_t no)               // 숫자를 문자열로 변환하여 송신, 8비트
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
		USART1_Transmit(numString[i]);
	}
}



#endif /* UART1_H_ */
