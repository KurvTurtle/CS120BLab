/*	Author: Christian Melendez
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #5  Exercise #1
 *	DEMONSTRATION LINK: https://drive.google.com/open?id=1JYUnkMzTo7_31C4GzdKh9jrp5qTDN8Wj
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{L0, L1, L2, L3, L4, L5, L6} state;

void fuelLevels() {
	unsigned char input = ~PINA & 0x0F;
	
	switch(state) { //Transitions
		case L0: //Initial State
			if (input == 0x00) {
				state = L0;
                       } else if (input == 0x01 | input == 0x02) {
		                state = L1;
		        } else if (input == 0x03 | input == 0x04) {
                		state = L2;
		        } else if (input == 0x05 | input == 0x06) {
                		state = L3;
		        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                		state = L4;
		        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                		state = L5;
		        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                		state = L6;
		        }
			break;
		case L1:
			 if (input == 0x00) {
                                state = L0;
                        } else if (input == 0x01 | input == 0x02) {
                                state = L1;
                        } else if (input == 0x03 | input == 0x04) {
                                state = L2;
                        } else if (input == 0x05 | input == 0x06) {
                                state = L3;
                        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                                state = L4;
                        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                                state = L5;
                        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                                state = L6;
                        }
			break;
		case L2:  if (input == 0x00) {
                                state = L0;
                        } else if (input == 0x01 | input == 0x02) {
                                state = L1;
                        } else if (input == 0x03 | input == 0x04) {
                                state = L2;
                        } else if (input == 0x05 | input == 0x06) {
                                state = L3;
                        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                                state = L4;
                        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                                state = L5;
                        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                                state = L6;
                        }
			break;
		case L3:
		       if (input == 0x00) {
                                state = L0;
                        } else if (input == 0x01 | input == 0x02) {
                                state = L1;
                        } else if (input == 0x03 | input == 0x04) {
                                state = L2;
                        } else if (input == 0x05 | input == 0x06) {
                                state = L3;
                        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                                state = L4;
                        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                                state = L5;
                        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                                state = L6;
                        }
                        break;
		case L4:
			if (input == 0x00) {
                                state = L0;
                        } else if (input == 0x01 | input == 0x02) {
                                state = L1;
                        } else if (input == 0x03 | input == 0x04) {
                                state = L2;
                        } else if (input == 0x05 | input == 0x06) {
                                state = L3;
                        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                                state = L4;
                        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                                state = L5;
                        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                                state = L6;
                        }
                        break;
		case L5:
			if (input == 0x00) {
                                state = L0;
                        } else if (input == 0x01 | input == 0x02) {
                                state = L1;
                        } else if (input == 0x03 | input == 0x04) {
                                state = L2;
                        } else if (input == 0x05 | input == 0x06) {
                                state = L3;
                        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                                state = L4;
                        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                                state = L5;
                        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                                state = L6;
                        }
                        break;
		case L6:
			if (input == 0x00) {
                                state = L0;
                        } else if (input == 0x01 | input == 0x02) {
                                state = L1;
                        } else if (input == 0x03 | input == 0x04) {
                                state = L2;
                        } else if (input == 0x05 | input == 0x06) {
                                state = L3;
                        } else if (input == 0x07 | input == 0x08 | input == 0x09) {
                                state = L4;
                        } else if (input == 0x0A | input == 0x0B | input == 0x0C) {
                                state = L5;
                        } else if (input == 0x0D | input == 0x0E | input == 0x0F) {
                                state = L6;
                        }
                        break;
	} //Transitions

	switch(state) { //State actions
		case L0: PORTC = 0x40; break; //Low fuel
		case L1: PORTC = 0x60; break; //Low fuel
		case L2: PORTC = 0x70; break; //Low fuel
		case L3: PORTC = 0x38; break;
		case L4: PORTC = 0x3C; break;
		case L5: PORTC = 0x3E; break;
		case L6: PORTC = 0x3F; break;
	} //State actions
}



int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; //Configure port C's 8 pins as outputs

    state = L0;

    while (1) {
	PORTC = 0x00;
	fuelLevels();
    }
    return 1;
}
