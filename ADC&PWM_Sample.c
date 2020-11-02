
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>     /* atoi,itoa */

unsigned int state = 0,delaystate = 0;
char result[10];
int count = 0;
unsigned char ch;
unsigned char nline[4] = "\n\r";


void usart_init(void){
	
	UCSR0B =(1 << TXEN0)|(1 << RXEN0)| (1 << UDRIE0)|(1 << RXCIE0)|(1 < TXCIE0);  // enable transmit, Receive
	//UCSR0C = (1 << UCSZ01)| (1 << UCSZ00);  // 8 bit data

	UCSR0C = 0x06;
	UBRR0L  = 0x67; // 9600
}



int main(void)
{
	
	usart_init();

	//ADC		
	// input for ADC	
	DDRC = 0;		

	//	ck/128 = 16e6/128 = 125000, less than 200k
	ADCSRA	|= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);

	
	// Internal 1.1V Voltage Reference
	// with external capacitor at AREF pin
	// enable ADC0
	// ADLAR = 1
	// 0xE0
	ADMUX |= (1 << REFS1)| (1 << REFS0)|(1 << ADLAR)|( 0 << MUX1)|( 0 << MUX0); 

	// ADC enable 
	// ADC interrupt enable
	// 0b10001000 ,0x88
	ADCSRA	|=	(1 << ADEN)| (1 << ADIE) ;
	
	//enable interrupt
	sei();		
	
	// start	
	//0b010000000,0x40
	ADCSRA |= (1 << ADSC);
	
	// Overall ADCSRA = 0xCF;

	
	
	
	//Timer 1 0.5s interrupt
	OCR1A = 0x1E84; // 0.5s
	TCCR1A = 0;
	TCCR1B = 0x0D; // ctc,1024
	TIMSK1 = 0x02; // start

	
	
	//OCR0A = Frequency (16M)/( generated wave * N) -1 
	//duty cycle = (OCR0B + 1 )*100 / (OCR0A +1)
	
		
	// Timer 0
	//Clear OC0A on compare match,set OC0A at bottom
	//Clear OC0B on Compare Match, set OC0B at BOTTOM,
	//(non-inverting mode)
	//PWM
	//TCCR0A = 0b10100011, 0xA3
	TCCR0A = ( 1 << COM0A1)|  ( 0 << COM0A0)|
	( 1 << COM0B1)|  ( 0 << COM0B0)|
	( 1 << WGM01)|   ( 1 << WGM00);   // Fast PWM,
	
	// 256p
	// TCCR0B= 0b00001100, 0x0C
	// 250 Hz
	// 25% duty cycle 
	TCCR0B = ( 1 << WGM02) | ( 1 << CS02 )|( 0 << CS01 )|( 0 << CS00 ); // 256 p
	OCR0A = 249;	// 250 Hz
	OCR0B = 62;		//
	
	
	
	
	// Timer 2
	//Clear OC2A on compare match,set OC2A at bottom
	//Clear OC2B on Compare Match, set OC2B at BOTTOM,
	//(non-inverting mode)
	//PWM
	//TCCR2A = 0b10100011, 0xA3
	TCCR2A = (1 << COM2A1) | ( 0 << COM2A0)|
	(1 << COM2B1) | ( 0 << COM2B0)|				//Clear OC2B on Compare Match, set OC2B at BOTTOM,
	( 1 << WGM21)|   ( 1 << WGM20);								 //(non-inverting mode).
	
	// 256p
	// TCCR2B= 0b00001110, 0x0E
	// 250 Hz
	// 25% duty cycle
	TCCR2B = ( 1 << WGM22)|(1 << CS22) | (1 << CS21)| (0 << CS20);		// Fast PWM ,OCR2A	
	OCR2A = 249;		// 250 Hz
	OCR2B = 62;			//
	
	
	// Button
	PORTD = (1 << INT0);
	//  0x02 INT0 falling, 0x03 rising
	//  0x08  INT1 falling,0x0C rising 
	EICRA = ( 1 << ISC01);		// INT0 falling
	
	// 0x02 int1
	// 0x01 int0
	EIMSK = (1 << INT0);		// enable INT0
	
	sei();
	while(1)
	{}
}

ISR(ADC_vect)
{
	unsigned int RADC=0;
	
	// get the value (10 bit) of ADC register
	RADC |= ADCL >> 6 | ADCH << 2;		
	
	// convert the value of RADC to decimal base char
	itoa(RADC,result,10);						

	// start
	ADCSRA |= (1 << ADSC);
	
}

ISR(USART_UDRE_vect){
	if (delaystate == 1 ){
		
		if (state == 0){
			if (count < 10){
				UDR0 = result[count];
				UCSR0A = (1 << TXC0);
				count++;
				}else{
				count = 0;
				state = 2;
				delaystate = 0;
			}
		}
		
		if (state == 2){
			if(count < 4){
				UDR0 = nline[count];
				UCSR0A = (1 << TXC0);		// set TXC0 to generate a transmit complete interrupt
				count ++;
				} else {
				count = 0;
				state  = 0;
				delaystate = 0;
			}
		}
		
	}
}


ISR(TIMER1_COMPA_vect){
		delaystate = 1;
}