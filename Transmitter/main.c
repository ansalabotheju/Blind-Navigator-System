#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>


unsigned int val;

int main(void){

		DDRB = 0xff;
		ADCSRA =1<<ADEN|1<<ADPS1|1<<ADPS0;
		ADMUX = 1<<REFS0;
		
    while(1) {
	
		ADCSRA = ADCSRA|(1<<ADSC); //start convertion
		while((ADCSRA & (1<<ADSC))==0);//wait for finish
		val = ADCL|ADCH<<8;
		
		if(val<500){
			
			PORTB = 0xff;
			
		}else{
			
			PORTB = 0x00;
			
		}	
		
    }
	
    return 0;
	
}
