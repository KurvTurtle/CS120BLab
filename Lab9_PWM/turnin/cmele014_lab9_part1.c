/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description:
 *	Demonstration Link: https://drive.google.com/open?id=1QGShcQ0ahGyEbXST8_el9lnD2-LuWqjD
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

enum States{Init, C, D, E} state;

void PWM_CDE() {
	unsigned char button1 = ~PINA & 0x01; //Read A0
	unsigned char button2 = ~PINA & 0x02; //Read A1
	unsigned char button3 = ~PINA & 0x04; //Read A2

	switch(state) { //Transitions
		case Init: //Initial State
			if (button1 && !button2 && !button3) {
				state = C;
			} else if (!button1 && button2 && !button3) {
				state = D;
			} else if (!button1 && !button2 && button3) {
				state = E;
			} else {
				state = Init;
			}
			break;
		case C:
			if (button1 && !button2 && !button3) {state = C;}
			else {state = Init;}
			break;
		case D:
			if (!button1 && button2 && !button3) {state = D;}
			else {state = Init;}
			break;
		case E:
			if (!button1 && !button2 && button3) {state = E;}
			else {state = Init;}
			break;
		default:
			break;
	} //Transitions

	switch(state) { //State actions
		case Init:
			set_PWM(0.00);
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
	PWM_CDE(); //Execute 1 SM tick
	}    
}
