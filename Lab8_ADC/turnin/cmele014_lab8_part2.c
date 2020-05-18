/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	Demonstration Link: https://drive.google.com/open?id=1IjN-zO24XKUTxK9qaobsjmYSXPMT7NYX
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() { 
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);

	//ADEN: Setting this bit enables analog-to-digital conversion
	//ADSC: Setting this bit starts the first conversion
	//ADATE: Setting this bit enables auto-triggering
}

void main() {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0xFF; //Configure port B's 8 pins as outputs
    DDRD = 0xFF; PORTD = 0x00; //Configure port D's 8 pins as outputs

    ADC_init(); //Initializes the ADC
    
    PORTB = 0x00; //Initial value of PORTB
    PORTD = 0x00; //Initial value of PORTD	

    unsigned short MAX = 0x00; //Variable for maximum value of ADC
    unsigned short MIN = 0xFFFF; //Variable for minimum value of ADC

    while (1) { 
	//User Code
	unsigned short input = ADC; //Value of ADC register
	PORTB = (char)input & 0xFF; //Lower 8 bits of ADC
	PORTD = (char)input >> 8; //Upper 2 bits of ADC
	
	if(ADC > MAX) { //Write MAX
		MAX = ADC;
	}
	
	if(ADC < MIN) { //Write MIN
		MIN = ADC;
	}
	
    }    
}
