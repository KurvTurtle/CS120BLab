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

enum States{L1, L2, Wait1, Wait2} State;

void AlternateLED() {
	switch(State) { //Transitions
		case L1: //Initial State
			if(PORTA == 0x00) {
				State = Wait1; 
			} else if(PORTA == 0x01){
				State = L1;
			}
			break;
		case L2:
			if(PORTA == 0x00) {
                                State = Wait2;
                        } else if(PORTA == 0x01) {
				State = L2;
			}
			break;
		case Wait1:
			if(PORTA == 0x00) {
                                State = Wait1;
                        } else if(PORTA == 0x01) {
                                State = L2;
                        }
			break;
		case Wait2:
			if(PORTA == 0x00) {
                                State = Wait2;
                        } else if(PORTA == 0x01) {
                                State = L1;
                        }
			break;
		default:
			State = L1;
			break;
	} //Transitions

	switch(State) { //State Actions
		case L1:
			PORTB = 0x01;
			break;
		case L2:
			PORTB = 0x02;
			break;
		default: 
			PORTB = 0x01;
			break;
	} //State Actions
}

int main() {
	State = L1; //Initial State
	DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; //Configure port B's 8 pins as outputs
        
	while(1) {
		AlternateLED();
	} //While(1)
} //Main
