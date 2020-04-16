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
    unsigned char cntavail = 0x00;

    while(1) {
        tmpA = PINA & 0x0F;
        
	cntavail = ~tmpA & 0x0F;
	if(tmpA == 0x0F) {
	    cntavail = 0x80;
	}
	PORTC = cntavail;
    }
    return 0;
}
