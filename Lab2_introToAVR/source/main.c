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
				// Initialize output on PORTB to 0x00
    unsigned char temp_input = 0x00;
    while(1) {
        temp_input = PINA;
        PORTB = temp_input;	// Writes port B's 8 pins with the values					// on port A's 8 pins
    }
    return 1;
}
