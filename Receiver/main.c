#define F_CPU 8000000UL    // clock
#include <avr/io.h>
#include <avr/interrupt.h>	// interrupts
#include <util/delay.h>		//delays

#define  Trigger_pin_1	PB7	/* Trigger pins */
#define  Trigger_pin_3	PB3

#define  Echo_pin_1	PB6
#define  Echo_pin_3	PB2

volatile int TimerOverflow = 0;

ISR(TIMER0_OVF_vect)
{
	TimerOverflow++;

}

int main(void){

	double distance_1;
	double distance_3;
	
	/* Make trigger pins as output and echo pins as inputs :33-->40 10101000 */
	DDRB =0xA8;
	DDRC =0x00;
	DDRA =0xff;
	DDRD =0xff;

	/*Calculate width of Echo by Getting TCNT1 Timer values */

	while (1){	
	
		distance_1 = distance_cal(Trigger_pin_1,Echo_pin_1);
		distance_3 = distance_cal(Trigger_pin_3,Echo_pin_3);
		
		//take the distance measurement to account
		
		if( distance_1<=100){
			
			PORTA = 1<<1;
			_delay_ms(1000);
			PORTA =0<<1;
			_delay_ms(1000);
			PORTA = 1<<5;
			_delay_ms(1000);
			PORTA =0<<1;
			_delay_ms(5000);
			
			
		}
		
		if((distance_3<180)&&(distance_3>170)){
			
			PORTA =0<<6;
			_delay_ms(2000);
		}
		
		else if(distance_3>=180){
			
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(100);
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(100);
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(100);
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(5000);
			
		}
		
		else if((distance_3<=170)&&(distance_3>=150)){
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(100);
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(5000);
		}
		
		else if(distance_3<150){
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(100);
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(100);
			PORTA = 1<<6;
			_delay_ms(100);
			PORTA =0<<6;
			_delay_ms(5000);
		}
		
		
		if(PINC & (1<<2)){
			
			PORTD = 1<<2;
			_delay_ms(2000);
			PORTD = 0<<2;
			_delay_ms(5000);
			
			
			
		}else{
		
			PORTD = 0<<2;
			_delay_ms(5000);
		}	
		
		
	}
	return 0;
}

int distance_cal(int trig_pin, int echo_pin){

		int count_1=0;
		int overflow =0;
		double distance_value =0;
			
		//setting up TIMER0 For time measurements
		TCNT0 = 0;
		TCCR0 |= (1<<CS01); // PRESCALER 8
		TIMSK = (1<<TOIE0);// enable TIMER overflow Interrupt
		sei();// enable global Interrupts
		_delay_ms(50);

		/* Make a 10us trigger pulse on trig pin_1 to HC-SR04 */
		PORTB =0x00;
		_delay_us(10);
		PORTB |= 1<<trig_pin;
		_delay_us(10);
		PORTB &= ~(1<<trig_pin);

		/* Wait for rising edge */
		while(1){
			if(!(PINB & (1<<echo_pin))){continue;}
			else{
				TCNT0 =0;
				TimerOverflow=0;
				break;}
			}
		
		/* Wait for falling edge */
		while (1){
			if(PINB & (1<<echo_pin)){continue;}
			else{
				count_1 = TCNT0 ;
				overflow = TimerOverflow;
				break;}
			}

		cli();//disable global Interrupts

		count_1 = count_1+ 256*(overflow);

		distance_value = count_1/58;// distance calculation  : distance in Centimeters

		return distance_value;
}


