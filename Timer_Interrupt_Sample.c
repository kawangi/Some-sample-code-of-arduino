
#include <avr/io.h>
#include <avr/interrupt.h>

int state1 = 1, state2 = 0,count = 0,count2;

void counter(int);

int main(void)
{
	DDRB = 0b00111111;
	DDRC = 0b00110001;
	PORTC = 0;
	PORTB = 0;
	
	OCR1A = 0x0C34;  // 0.2s
	TCCR1A = 0x00;
	TCCR1B = 0x0D;	// ctc, 1024
	TIMSK1 = (1 << OCIE1A);
	
	
	OCR0A = 2;
	TCCR0A = 0x02;// ctc
	TCCR0B = 0x06;	//	External clock source on T0 pin.Clock on falling edge.
	TIMSK0 = (1 << OCIE0A);
	
	sei();
    while(1){
	
	}
}


ISR(TIMER0_COMPA_vect){
	
		PORTC ^= (1 << 0);
}

ISR(TIMER1_COMPA_vect){
	
	if( state1 == 1 && count == 0){
		
		PORTB = 0b00100001;				//1
		PORTC |= (1 << 4);
		state1 = 2;
		count = 0;
		
	}
	else if( state1 == 2 && count == 25){
		
		PORTB = 0b00010001;				//2
		PORTC |= (1 << 4);
		state1 = 3;
		count = 0;
		
		}else if( state1 == 3 && count == 5){
		
		PORTB = 0b00001001;				//3
		PORTC |= (1 << 4);
		state1 = 4;
		count = 0;
		
		}else if ( state1 == 4 && count == 5){
		
		PORTB = 0b00001011;				//4
		PORTC |= (1 << 4);
		state1 = 5;
		count = 0;
		}else if ( state1 == 5 && count == 5){
		PORTB = 0b00001100;				//5
		PORTC &= ~(1 << 4);
		PORTC |= (1 << 5);
		state1 = 6;
		count = -20;
		
		}else if (state1 == 6 && count >= 0 && count < 5 ){
		
		PORTB = 0b00001010;				//6   blinking
		if(state2 == 0)
		{
			PORTC &= ~(1 << 5);
			state2 = 1;
			}else{
			PORTC |= (1 << 5);
			state2 = 0;
		}
		if(count == 4){
			state1 = 7;
			count = 0;
		}
		
		}else if( state1 == 7 ){
		
		PORTB = 0b00001001;				//7
		PORTC &= ~(1 << 5);
		PORTC |= (1 << 4);
		state1 =8;
		count = 0;
		
		}else if( state1 == 8 && count == 5){
		
		PORTB = 0b00011001;				//8
		PORTC |= (1 << 4);
		state1 = 1;
		count = -5;
	}

	count++;
	
}