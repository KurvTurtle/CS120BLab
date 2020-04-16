/*	Author: cmele014
 *  Partner(s) Name: Jo Smith
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
    unsigned char tmpB = 0x00; //Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; //Temporary variable to hold the value of A

    while(1) {
        tmpA = PINA & 0x03;
        
	if(tmpA == 0x00) {
	    tmpB = (tmpB & 0xFE) | 0x00;
        } else if(tmpA == 0x01) {
	    tmpB = (tmpB & 0xFE) | 0x01;
	} else if(tmpA == 0x02) {
	    tmpB = (tmpB & 0xFE) | 0x00;
	} else {
	    tmpB = (tmpB & 0xFE) | 0x00;
	}
	PORTB = tmpB;
    }
    return 0;
}
