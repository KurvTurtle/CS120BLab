/*	Author: Christian Melendez
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #3 
 *	DEMONSTRATION LINK: https://drive.google.com/open?id=1-eTQSbVAoWp7RSCNpuiR8-wHW5fk0gDN
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Init, Odds1, Odds2, Evens1, Evens2, firstHalf1, firstHalf2, secondHalf1, secondHalf2, Edges1, Edges2} state;

void festiveLightsDisplay() {
	unsigned char input = ~PINA & 0x01; //Read A0
	unsigned char output = ~PINB & 0x3F; //Write B0-B5
	
	switch(state) { //Transitions
		case Init: //Initial State
			if(input == 0x01){state = Odds1;}
			break;
		case Odds1:
			if(input == 0x00){state = Odds2;}
			break;
		case Odds2:
			if(input == 0x01){state = Evens1;}
			break;
		case Evens1:
			if(input == 0x00){state = Evens2;}
			break;
		case Evens2:
			if(input == 0x01){state = firstHalf1;}
			break;
		case firstHalf1:
                        if(input == 0x00){state = firstHalf2;}
                        break;
                case firstHalf2:
                        if(input == 0x01){state = secondHalf1;}
                        break;
		case secondHalf1:
                        if(input == 0x00){state = secondHalf2;}
                        break;
                case secondHalf2:
                        if(input == 0x01){state = Edges1;}
                        break;
		case Edges1:
                        if(input == 0x00){state = Edges2;}
                        break;
                case Edges2:
                        if(input == 0x01){state = Odds1;}
                        break;



	} //Transitions

	switch(state) { //State actions
		case Init:
			output = 0x00;
			break;
		case Odds1:
			output = 0x15;
			break;
		case Odds2:
			output = 0x15;
			break;
		case Evens1:
			output = 0x2A;
			break;
		case Evens2:
			output = 0x2A;
			break;
		case firstHalf1:
                        output = 0x07;
                        break;
                case firstHalf2:
                        output = 0x07;
                        break;
                case secondHalf1:
                        output = 0x38;
                        break;
                case secondHalf2:
                        output = 0x38;
                        break;
		case Edges1:
                        output = 0x21;
                        break;
                case Edges2:
                        output = 0x21;
                        break;
		default:
			break;
	} //State actions

	PORTB = output;
}



int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; //Configure port B's 8 pins as outputs

    state = Init; //Set initial state
    PORTB = 0x00; //Initial value of PORTB

    while (1) {
	festiveLightsDisplay();
    }
    return 1;
}
