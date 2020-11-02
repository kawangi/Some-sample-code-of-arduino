
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char ch;
unsigned char Current;
unsigned int Counter = 0;
unsigned int State = 0;
unsigned int State2 = 0;

void usart_init(void){
	UCSR0B = (1 << TXEN0)|(1 << RXEN0)| (1 << UDRIE0)|
	(1 << RXCIE0)|(1 < TXCIE0);  // enable transmit, Receive
	UCSR0C = (1 << UCSZ01)| (1 << UCSZ00);  // 8 bit data
	UBRR0L  = 103;
}



int main(void)
{
	
	usart_init();	
	sei();
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}


ISR(USART_RX_vect){
	ch = UDR0;
	if (State2 == 0)
	{
		Current = ch;
		State = 1;
		State2 = 1;
	}else if (State2 == 1){
		if (ch == Current){
			State = 0;	
			State2 = 0;		
		}
	}
	
	
}


ISR(USART_UDRE_vect){
	
	if (State == 0){
		UDR0 = 'a';	
	}else if (State == 1){
		if (Counter < 10){
			UDR0 = ch;
			Counter ++;
		}else{
			Counter = 0;
			State = 2;
		}
	}
}