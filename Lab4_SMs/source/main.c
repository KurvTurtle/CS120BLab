/*	Author: Christian Melendez
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Init, Inc, Dec, Reset} State;

void AlternateLED() {
	unsigned char button1 = PINA & 0x01;
	unsigned char button2 = (PINA & 0x02) >> 1;
	
	int count = 0;
	switch(State) { //Transitions
		case Init: //Initial State
			if(button1 && !button2) {
				switch(PORTC & 0x0F) {//Increment
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
                                }//Increment
				State = Inc; 
			} else if(!button1 && button2){
				switch(PORTC & 0x0F) {//Decrement
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
				}//Decrement
				State = Dec;
			} else if(button1 && button2) {
				State = Reset;
			} else {
				State = Init;
			}
			break;
		case Inc:
			if(!button1 && button2) {
				switch(PORTC & 0x0F) {//Decrement
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
                                }//Decrement
				State = Dec;
			} else if(button1 && button2) {
				State = Reset;
			} else {
				State = Inc;
			}
			break;
		case Dec:
			if(button1 && !button2) {
				switch(PORTC & 0x0F) {//Increment
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
                                }//Increment
                                State = Inc;
                        } else if(button1 && button2) {
                                State = Reset;
                        } else {
				State = Dec;
			}
			break;
		case Reset:
			if(1){State = Init;}
			break;
		default:
			State = Init;
			break;
	} //Transitions

	switch(State) { //State Actions
		case Init:
			if(count == 0) {PORTC = 0x07;}
		        else {PORTC = 0x00;}
			break;
		case Inc:
			break;
		case Dec:
			break;
		case Reset:
			PORTC = 0x00;
			count = 1;
			break;
		default: 
			PORTC = 0x07;
			break;
	} //State Actions
}

int main(void) {
	State = Init; //Initial State
	DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; //Configure port B's 8 pins as outputs
        
	while(1) {
		PORTC = 0x01;
		AlternateLED();
	} //While(1)
} //Main
