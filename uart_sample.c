
#include <avr/io.h>

void usart_init(void){
	UCSR0B = (1 << TXEN0)|(1 << RXEN0);  // enable transmit
	UCSR0C = (1 << UCSZ01)| (1 << UCSZ00);  // 8 bit data
	UBRR0L  = 103;
}

void usart_send(unsigned char ch ){
	
	while (! (UCSR0A & (1 << UDRE0))); // wait until UDR is empty
	UDR0 = ch;
	
}


int main(void)
{
	unsigned char ch,current;
	usart_init();
	
    while(1)
    {
        //TODO:: Please write your application code 
		
		
		while(!(UCSR0A & (1 << RXC0))){
			usart_send('a');		
		};			
		ch = UDR0;
		current = ch;
		for(int i = 0 ; i < 10 ; i++)
			usart_send(ch);		
			
		do {
			while(!(UCSR0A & (1 << RXC0)));
			ch = UDR0;	
						
			}while(ch != current);
				
		
		
	
    }
}