/*	Author: Christian Melendez
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #2
 *	DEMONSTRATION LINK: https://drive.google.com/
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Init, Increment, Decrement, Pressed, Reset} state;

void incrementDecrement() {
	unsigned char input = ~PINA & 0x03; //Read A0 and A1
	
	switch(state) { //Transitions
		case Init: //Initial State
			if (input == 0x00) {
				state = Init;
			} else if (input == 0x01) {
				state = Increment;
			} else if (input == 0x02) {
				state = Decrement;
			} else if (input == 0x03) {
				state = Reset;
			}
			break;
		case Increment:
			state = Pressed;
			break;
		case Decrement:
			state = Pressed;
			break;
		case Pressed:
			if (input == 0x00) {
				state = Init;
			} else if (input == 0x03) {
				state = Reset;
			}
			break;
		case Reset:
			state = Init;
			break;
	} //Transitions

	switch(state) { //State actions
		case Init: 
			break;
		case Increment: 
			switch(PORTC) { //Increment
                                case 0x00: PORTC = 0x01; break;
                                case 0x01: PORTC = 0x02; break;
                                case 0x02: PORTC = 0x03; break;
                                case 0x03: PORTC = 0x04; break;
                                case 0x04: PORTC = 0x05; break;
                                case 0x05: PORTC = 0x06; break;
                                case 0x06: PORTC = 0x07; break;
                                case 0x07: PORTC = 0x08; break;
                                case 0x08: PORTC = 0x09; break;
                                case 0x09: PORTC = 0x09; break;
                        } //Increment
		break;
		case Decrement: 
			switch(PORTC) { //Decrement
				case 0x00: PORTC = 0x00; break;
				case 0x01: PORTC = 0x00; break;
				case 0x02: PORTC = 0x01; break;
				case 0x03: PORTC = 0x02; break;
				case 0x04: PORTC = 0x03; break;
				case 0x05: PORTC = 0x04; break;
				case 0x06: PORTC = 0x05; break;
				case 0x07: PORTC = 0x06; break;
				case 0x08: PORTC = 0x07; break;
				case 0x09: PORTC = 0x08; break;
			} //Decrement
			break;
		case Pressed:
			break;
		case Reset:
			PORTC = 0x00;
			break;
	} //State actions
}



int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; //Configure port C's 8 pins as outputs

    state = Init; //Set initial state
    PORTC = 0x00; //Initial value of PORTC

    while (1) {
	incrementDecrement();
    }
    return 1;
}
