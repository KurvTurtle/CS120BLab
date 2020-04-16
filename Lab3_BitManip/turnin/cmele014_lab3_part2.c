/*	Author: Christian Melendez
 *	Partner(s) Name: None
 *      Lab Section: 021
 *	Assignment: Lab 3  Exercise 2 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *     I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
    unsigned char tmpA = 0x00; //Temporary variable to hold the value of A
    unsigned char tmpB = 0x00; //Temporary variable to hold the value of B
    unsigned char tmpC = 0x00; //Temporary variable to hold the value of C

    while(1) {
        tmpA = PINA & 0x0F;        
	 
	if(tmpA ==  0x00) {
	    tmpC = 0x40;
	} else if(tmpA == 0x01 | tmpA == 0x02) {
	    tmpC = 0x60;
	} else if(tmpA == 0x03 | tmpA == 0x04) {
	    tmpC = 0x70 ;
	} else if(tmpA == 0x05 | tmpA == 0x06) {
	    tmpC = 0x38;
	} else if(tmpA == 0x07 | tmpA == 0x08 | tmpA == 0x09) {
	    tmpC = 0x3C;
	} else if(tmpA == 0x0A | tmpA == 0x0B | tmpA == 0x0C) {
	    tmpC = 0x3E;
	} else if(tmpA == 0x0D | tmpA == 0x0E | tmpA == 0x0F) {
	    tmpC = 0x3F;
	}
	PORTC = tmpC;
    }
    return 0;
}
