#include <avr/io.h>
#include <avr/interrupt.h>

int state1 = 1, state2 = 0,count = 0,count2;

int main(void)
{
	DDRB = 0b00111111;
	DDRC = 0b00110001;
	DDRD = 0;
	
	PORTC = 0;
	PORTB = 0;
	
	
	OCR1A = 0x0C34;  // 0.2s
	TCCR1A = 0x00;
	TCCR1B = 0x0D;	// ctc, 1024
	TIMSK1 = (1 << OCIE1A);		// enable counter1 interrupt
	
	PORTD = (1 << INT0);	// high impedence
	EICRA = ( 1 << ISC01);		// INT0, falling 
	EIMSK = (1 << INT0);		// enable external interrupt INT0
	
	sei();
	while(1){}
}

ISR(INT0_vect){
	
	if(count2  == 3 ){
		PORTC ^= (1 << 0);
		count2 = 0;
	}
	else
	count2++;
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