/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *	Demonstration Link: https://drive.google.com/open?id=1Eavg1Bf4i637NsA_e7LNARndBgqwfOwf
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//0.954Hz is the lowest frequency possible with this function
//Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; //Keeps track of the currently set frequency
					 //Will only update the registers when the frequency changes
	if(frequency != current_frequency) {
		if(!frequency) {TCCR3B &= 0x08;} //Stop timer/counter
		else {TCCR3B |= 0x03;} //Resumes/continues timer/counter

		//Prevents OCR3A from overflowing, using prescaler 64
		//0.954 is the smallest frequency that will not result in overflow
		if(frequency < 0.954) {OCR3A = 0xFFFF;}

		//Prevents OCR3A from underflowing, using prescaler 64
		//31250 is the largest frequency that will not result in underflow
		else if (frequency > 31250) {OCR3A = 0x0000;}

		//Set OCR3A based on desired frequency
		else {OCR3A = (short)(8000000/(128*frequency)) - 1;}

		TCNT3 = 0; //Resets counter
		current_frequency = frequency; //Updates the current frequency
	} //If statement
} //Function end

void PWM_on() {
	TCCR3A = (1 << COM3A0); //COM3A0: Toggle PB3 on compare match between coutner and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	//WGM32: Reset counter(TCNT3) when TCNT3 matches OCR3A
	//CS31 & CS30: Set a prescaler of 64
	
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{Init, C, D, E, F, G, A2, B2, C2, Off, CPressed, DPressed, EPressed, FPressed, GPressed, A2Pressed, B2Pressed, C2Pressed} state;

void PWM_Cycle() {
	unsigned char input = ~PINA & 0x07; //Read A0, A1, and A3
	unsigned char a = ~PINA & 0x01;
	unsigned char b = ~PINA & 0x02;
	unsigned char c = ~PINA & 0x04;

	switch(state) { //Transitions
		case Init: //Initial State
			state = CPressed;
			break;
		case C:
			if (a && !b && !c) {state = DPressed;}
			else if (!a && b && !c) {set_PWM(0);}
			break;
		case CPressed:
			if(!a && !c) {state = C;}
			break;		
		case D:
			if (a && !b && !c) {state = EPressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			else if (!a && !b && c) {state = CPressed;}
			break;
		case DPressed:
                        if(!a && !c) {state = D;}
                        break;
		case E:
			if (a && !b && !c) {state = FPressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			else if (!a && !b && c) {state = DPressed;}
			break;
		case EPressed:
                        if(!a && !c) {state = E;}
                        break;
		case F:
			if (a && !b && !c) {state = GPressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			else if (!a && !b && c) {state = EPressed;}
			break;
		case FPressed:
                        if(!a && !c) {state = F;}
                        break;
		case G:
			if (a && !b && !c) {state = A2Pressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			else if (!a && !b && c) {state = FPressed;}
			break;
		case GPressed:
                        if(!a && !c) {state = G;}
                        break;
		case A2:
			if (a && !b && !c) {state = B2Pressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			else if (!a && !b && c) {state = GPressed;}
			break;
		case A2Pressed:
                        if(!a && !c) {state = A2;}
                        break;
		case B2:
			if (a && !b && !c) {state = C2Pressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			else if (!a && !b && c) {state = A2Pressed;}
			break;
		case B2Pressed:
                        if(!a && !c) {state = B2;}
                        break;
		case C2:
			if (!a && !b && c) {state = B2Pressed;}
                        else if (!a && b && !c) {set_PWM(0);}
			break;
		case C2Pressed:
                        if(!a && !c) {state = C2;}
                        break;
		default:
			break;
	} //Transitions

	switch(state) { //State actions
		case Init:
			set_PWM(0);
			break;
		case CPressed:
			set_PWM(261.63);
			break;
		case DPressed:
			set_PWM(293.66);
			break;
		case EPressed:
			set_PWM(329.63);
			break;
		case FPressed:
                        set_PWM(349.23);
                        break;
                case GPressed:
                        set_PWM(392.00);
                        break;
                case A2Pressed:
                        set_PWM(440.00);
                        break;
		case B2Pressed:
			set_PWM(493.88);
			break;
		case C2Pressed:
			set_PWM(523.25);
			break;
		case C:
                        set_PWM(261.63);
                        break;
                case D:
                        set_PWM(293.66);
                        break;
                case E:
                        set_PWM(329.63);
                        break;
                case F:
                        set_PWM(349.23);
                        break;
                case G:
                        set_PWM(392.00);
                        break;
                case A2:
                        set_PWM(440.00);
                        break;
                case B2:
                        set_PWM(493.88);
                        break;
                case C2:
                        set_PWM(523.25);
                        break;
		default:
			set_PWM(0);
			break;
	} //State actions
}


void main() {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; //Configure port B's 8 pins as outputs
    
    PWM_on(); //Initializes PWM

    PORTB = 0x00; //Initial value of PORTB

    state = Init; //Set initial state

    while (1) { 
	//User Code
	PWM_Cycle(); //Execute 1 SM tick
	}    
}
