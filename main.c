/*
 * ADC_PWM.c
   This project is derived from ME370 smart car project 
 
 * Main function: 
		Read analog input from ADC0, ADC1 and ADC2 and convert them to PWM wave for motor control
		pin connection: PC0(ADC0) - PB1(OC1A), PC1(ADC1) - PB2(OC1B), PC2(ADC2) - PD6(OC0A)
		
 * Created: 24/02/2021 4:54:01 PM
 * Last update: 15/05/2021 12:32 PM
 * Author : Lily Zhang
 
 */ 

#ifndef F_CPU 
#define F_CPU 2000000UL //2Mhz according to the fuse settings
#endif

// set & clear bit macros
#define SET_BIT(byte, bit) (byte |= (1 << bit)) //set bit
#define CLEAR_BIT(byte, bit) (byte &= ~(1<<bit)) //clear bit
#define BIT_IS_SET(byte, bit) (byte & (1 << bit)) //check if bit is set
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit))) //check if bit is clear

#include <avr/io.h>
#include <avr/interrupt.h>

void init_ADC(void){
	// initialize ADC registers
	ADMUX = 0b01100000; // select ADC0 initially, left adjust
	ADCSRA = 0b10001011; // enable ADC, enable interrupt, division factor 8, single conversion mode
	ADCSRB = 0x00; 
}

void init_PWM(void){
	// initialize PWM registers
	SET_BIT(DDRB, PB1); //set PB1 as output (OC1A)
	SET_BIT(DDRB, PB2); //set PB2 as output (OC1B)
	SET_BIT(DDRD, PD6);  //OC0A 
	
	TCCR0A = 0b10100011; // fast PWM on OC0
	TCCR0B = 0b00000010; // pre-scaler /8
	/*Important: TC0 registers and TC1 registers have different set up!*/
	TCCR1A = 0b10100001; // fast PWM on OC1
	TCCR1B = 0b00001010; // pre-scaler /8
}


int main(void)
{
	init_ADC(); // initialize ADC
	init_PWM(); // initialize PWM
	
	sei(); // enable interrupt
	
	SET_BIT(ADCSRA, ADSC); //  start first conversion
	
    while (1) {} // infinite loop
}

ISR(ADC_vect) {
	switch(ADMUX){
		case 0b01100000: // ADC0 to OCR1A
			OCR1A = ADCH; // use ADC reading to define PWM duty cycle
			ADMUX = 0b01100001; // enable ADC1
			SET_BIT(ADCSRA, ADSC); // start next conversion
		break;
		case 0b01100001: //ADC1 to OCR1B
			OCR1B = ADCH; // use ADC reading to define PWM duty cycle
			ADMUX = 0b01100010; // enable ADC2
			SET_BIT(ADCSRA, ADSC); // start next conversion
		break;
		case 0b01100010: //ADC2 to OCR0A
			OCR0A = ADCH;
			ADMUX = 0b01100000; //return to ADC0
			SET_BIT(ADCSRA, ADSC); // start next conversion
		break;
	}

}

