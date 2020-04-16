/*	Author: cmele014
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab 2  Exercise 3 
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
    unsigned char tmpA = 0x00; //Temporary variable to hold the value of A
    unsigned char cntavail = 0x04;

    while(1) {
        tmpA = PINA & 0x0F;
        
	if(tmpA == 0x00) {
	    cntavail = 0x04;
	} else  if(tmpA == 0x03 | tmpA == 0x05 | tmpA == 0x06 |  tmpA == 0x09 |tmpA == 0x0A | tmpA == 0x0C) {
	    cntavail = 0x02;
	} else if(tmpA == 0x07 | tmpA == 0x0B | tmpA == 0x0D | tmpA == 0x0E) {
	    cntavail = 0x01 ;
	} else if(tmpA == 0x0F ) {
	    cntavail = 0x80;
	} else {
	    cntavail = 0x03;
	}

	PORTC = cntavail;
    }
    return 0;
}
