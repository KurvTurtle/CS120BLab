/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab 8  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *	Demonstration Link: https://drive.google.com/open?id=1_m8u2U1tdl6EILg4UOA3MF8lo_Os7kW6 
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

    ADC_init(); //Initializes the ADC
    
    PORTB = 0x00; //Initial value of PORTB

    unsigned short MAX = 0x00; //Variable for maximum value of ADC
    unsigned short MIN = 0xFFFF; //Variable for minimum value of ADC

    while (1) { 
	//User Code
	unsigned short input = ADC; //Value of ADC register

	if(ADC > MAX) { //Write MAX
		MAX = ADC;
	}
	
	if(ADC < MIN) { //Write MIN
		MIN = ADC;
	}
	
	unsigned short threshold = (MAX)/ 8; //Each level of LED
	//unsigned short remainder = MIN/8;

	if (input < threshold) {PORTB = 0x00;}
	else if (input < (2 * threshold)) {PORTB = 0x01;}
	else if (input < (3 * threshold)) {PORTB = 0x03;}
        else if (input < (4 * threshold)) {PORTB = 0x07;}
        else if (input < (5 * threshold)) {PORTB = 0x0F;}
        else if (input < (6 * threshold)) {PORTB = 0x1F;}
        else if (input < (7 * threshold)) {PORTB = 0x3F;}
        else if (input < (8 * threshold)) {PORTB = 0x7F;}
	else if (input == (8 * threshold)) {PORTB = 0xFF;}
    }    
}
