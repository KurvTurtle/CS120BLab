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

int count = 0;
unsigned char tempC = 0x07;

void AlternateLED() {
	unsigned char button1 = PINA & 0x01;
	unsigned char button2 = PINA & 0x02;

	switch(State) { //Transitions
		case Init: //Initial State
			if(button1 && !button2) {
				State = Inc; 
			} else if(!button1 && button2){
				State = Dec;
			} else if(button1 && button2) {
				State = Reset;
			} else {
				State = Init;
			}
			break;
		case Inc:
			if(!button1 && button2) {
				State = Dec;
			} else if(button1 && button2) {
				State = Reset;
			} else if(button1 && !button2){
				State = Inc;
			} else {
				count = 1;
				State = Inc;
			}
			break;
		case Dec:
			if(button1 && !button2) {
			        State = Inc;
                        } else if(button1 && button2) {
                                State = Reset;
                        } else if(!button1 && button2){
				State = Dec;
			} else {
				count = 2;
				State = Dec;
			}
			break;
		case Reset:
			if(button1 && !button2) {
                                State = Inc;
                        } else if(!button1 && button2){
                                State = Dec;
                        } else if(button1 && button2) {
                                State = Reset;
                        } else {
                                State = Reset;
                        }
			break;
		default:
			State = Init;
			break;
	}; //Transitions

	switch(State) { //State Actions
		case Init:
			tempC = 0x07;
			break;
		case Inc:
			//if(count != 1) {
					switch(tempC) {//Increment
                                        	case 0x00: tempC = 0x01; break;
	                                        case 0x01: tempC = 0x02; break;
        	                                case 0x02: tempC = 0x03; break;
                	                        case 0x03: tempC = 0x04; break;
                        	                case 0x04: tempC = 0x05; break;
                                	        case 0x05: tempC = 0x06; break;
	                                        case 0x06: tempC = 0x07; break;
        	                                case 0x07: tempC = 0x08; break;
                	                        case 0x08: tempC = 0x09; break;
                        	                case 0x09: tempC = 0x09; break;
                                	}; //Increment
			//}//If
			break;
		case Dec:
			//if(count != 2) {
				switch(tempC & 0x0F) { //Decrement
                                        case 0x00: tempC = 0x00; break;
                                        case 0x01: tempC = 0x00; break;
                                        case 0x02: tempC = 0x01; break;
                                        case 0x03: tempC = 0x02; break;
                                        case 0x04: tempC = 0x03; break;
                                        case 0x05: tempC = 0x04; break;
                                        case 0x06: tempC = 0x05; break;
                                        case 0x07: tempC = 0x06; break;
                                        case 0x08: tempC = 0x07; break;
                                        case 0x09: tempC = 0x08; break;
                                }; //Decrement
			//}
			break;
		case Reset:
			tempC = 0x00;
			break;
		default: 
			tempC = 0x07;
			break;
	}; //State Actions

	PORTC = tempC;
}

int main(void) {
	State = Init; //Initial State
	DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; //Configure port B's 8 pins as outputs
        
	while(1) {
		PORTC = 0x07;
		AlternateLED();
	} //While(1)
} //Main
