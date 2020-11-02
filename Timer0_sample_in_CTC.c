
#include <avr/io.h>
void delayNormal(int);
void delayCTC(int);

int main(void)
{
	DDRB = 0b00111111;
	DDRC = 0b00110000;
	
	while(1)
	{
		//TODO:: Please write your application code
		PORTB = 0b00100001;		//1
		PORTC = 0b00010000;
		delayCTC(5000);
		PORTB = 0b00100001;		//2
		PORTC = 0b00010000;
		delayCTC(1000);
		PORTB = 0b00001001;		//3
		PORTC = 0b00010000;
		delayCTC(1000);
		PORTB = 0b00001011;		//4
		PORTC = 0b00010000;
		delayCTC(1000);
		PORTB = 0b00001100;		//5
		PORTC = 0b00100000;
		delayCTC(5000);
		
		for(int i = 1; i < 10; i ++){
			PORTB = 0b00001010;		//6
			
			if(i % 2 == 0)
			PORTC =0;
			else
			PORTC = 0b00100000;
			
			delayCTC(200);
		}
		
		PORTB = 0b00001001;		//7
		PORTC = 0b00010000;
		delayCTC(1000);
		PORTB = 0b00011001;		//8
		PORTC = 0b00010000;
		delayCTC(1000);
	}
}

void delayCTC(int times)	// 0.001
{
	OCR0A	= 0x3F;		//T= 1/16Mhz = 6.25e-8
	//Desired time ( 0.001)/(6.25e-8*256 prescaler)=62.5 ~ 63=0x3F
	while(times > 0){
		TCCR0A = 0x02;		// CTC mode
		TCCR0B = 0x04;		// ck/256 prescaler
		while((TIFR0 & (1 << OCF0A)) == 0 );
		TCCR0A = 0;
		TCCR0B = 0;
		TIFR0 = 0x02;		// clear flag
		
		times--;
	}
	
}