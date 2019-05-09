*
 Header file for USART #0 
*/ 


#define sbi(reg, bit)    reg |= (1<<(bit))    
#define cbi(reg, bit)    reg &= ~(1<<(bit))

void USART0_Init( INT16 ubrr );
void USART0_Transmit( char data );
void USART0_Transmit_String(char *str);
void USART0_Transmit_NewLine(void);
char USART0_Receive( void );


void USART0_Init( INT16 ubrr )
{
	// Set baud rate
	UBRR0H = (INT8)(ubrr>>8);
	UBRR0L = (INT8)ubrr;
	// Enable U2X
	if(U2X_S == 2)
		sbi(UCSR0A, U2X);
	else
		cbi(UCSR0A, U2X);
	// Enable receiver and transmitter
	sbi(UCSR0B, RXEN);
	sbi(UCSR0B, TXEN);

	// Set frame format: 8data, 1stop bit
	cbi(UCSR0C, UMSEL);  // asynch
	cbi(UCSR0C, USBS);   // 1 Stop bit
	cbi(UCSR0C, UPM01);  // No parity
	cbi(UCSR0C, UPM00);
	cbi(UCSR0B, UCSZ02); // 8-bit
	sbi(UCSR0C, UCSZ01);
	sbi(UCSR0C, UCSZ00);

	// Enable interrupt
//	sbi(UCSR0B, RXCIE);  // RX
//	sbi(UCSR0B, TXCIE);  // TX
//	sbi(UCSR0B, UDRIE);  // UDR Empty

//	sei();  // Enable all interrupt
}

void USART0_Transmit( char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & 0x20 ) )   // (1<<UDRE)
	;
	// Put data into buffer, sends the data
	UDR0 = data;
}

void USART0_Transmit_String(char *str)
{
	while(*str != '\0')
	{
		if(*str == '\n')
		{
			USART0_Transmit_NewLine();
		}
		else
		{
			USART0_Transmit(*str);
		}
		str++;
	}
}

void USART0_Transmit_NewLine(void)
{
	USART0_Transmit(0x0A); // LF
	USART0_Transmit(0x0D); // CR
}


char USART0_Receive( void )
{
	// Wait for data to be received
	while ( !(UCSR0A & 0x80) )  //(1<<RXC)
	{
		;
	}
	// Get and return received data from buffer
	return UDR0;
}
