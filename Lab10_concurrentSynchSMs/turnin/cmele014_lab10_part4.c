/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #10  Exercise #3/4
 *	Exercise Description: 
 *      Demonstration Link: https://drive.google.com/open?id=1PtbQlBpa9zTD_eEndT90fKNfIGiOWZX1
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

//Shared Variables
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speaker = 0x00;
int speakerPeriod = 2;

//SM States
enum States1{Init1, One, Two, Three} threeState;
enum States2{Init2, On, Off} blinkingState;
enum States3{Init3, Read} combinedState;
enum States4{Init4, ToggleOn, ToggleOff} speakerState;
enum States5{Init5, Increment, IncrementWait, Decrement, DecrementWait} F_State;

void threeLEDsSM() {

	switch(threeState) { //Transitions
		case Init1: //Initial State
			threeState = One;
			break;
		case One:
			threeState = Two;
			break;
		case Two:
			threeState = Three;
			break;
		case Three:
			threeState = One;
			break;
		default:
			break;
	} //Transitions

	switch(threeState) { //State actions
		case Init1:
			break;
		case One:
			threeLEDs = 0x01;
			break;
		case Two:
			threeLEDs = 0x02;
			break;
		case Three:
			threeLEDs = 0x04;
			break;
		default:
			break;
	} //State actions
}

void blinkingLEDSM() {

	switch(blinkingState) { //Transitions
		case Init2: //Initial State
			blinkingState = On;
			break;
		case On:
			blinkingState = Off;
			break;
		case Off:
			blinkingState = On;
			break;
		default:
			break;
	} //Transitions

	switch(blinkingState) { //State actions
		case Init2:
			break;
		case On:
			blinkingLED = 0x08;
			break;
		case Off:
			blinkingLED = 0x00;
		default:
			break;
	} //State actions
}

void combineLEDsSM() {
        
        switch(combinedState) { //Transitions
                case Init3: //Initial State
			combinedState = Read;
                        break;
		case Read:
			break;
                default:
                        break;
        } //Transitions

        switch(combinedState) { //State actions
                case Init3:
                        break;
		case Read:
			PORTB = blinkingLED | threeLEDs | speaker;
			break;
                default:
                        break;
        } //State actions
}

void speakerSM() {

        switch(speakerState) { //Transitions
		unsigned char input = ~PINA & 0x04; //Reads A2

                case Init4: //Initial State
			speakerState = ToggleOn;
			break;
		case ToggleOn:
			speakerState = ToggleOff;
                        break;
		case ToggleOff:
                        speakerState = ToggleOn;
                        break;
                default:
                        break;
        } //Transitions

        switch(speakerState) { //State actions
                case Init4:
                        break;
                case ToggleOn:
			speaker = 0x10;
                        break;
		case ToggleOff:
			speaker = 0x00;
			break;
                default:
                        break;
        } //State actions
}

void changeFrequency() {
	unsigned char button1 = ~PINA & 0x01; //Read A0
	unsigned char button2 = ~PINA & 0x02; //Read A1

	switch(F_State) { //Transitions
		case Init5:
			if(button1 && !button2) {F_State = IncrementWait;} 
			else if(!button1 && button2) {F_State = DecrementWait;}
			break;
		case IncrementWait:
			if(!button1 && !button2) {F_State = Increment;}
			else {F_State = IncrementWait;}
			break;
		case Increment:
			F_State = Init5;
			break;
		case DecrementWait:
                        if(!button1 && !button2) {F_State = Decrement;}
			else {F_State = DecrementWait;}
                        break;
                case Decrement:
                        F_State = Init5;
                        break;
		default:
			break;
	} //Transitions

	switch(F_State) { //State actions
		case Init5:
			break;
		case Increment:
			switch(speakerPeriod) {
				case 1: speakerPeriod = 1; break;	
				case 2: speakerPeriod = 1; break;
				case 3: speakerPeriod = 2; break;
                                case 4: speakerPeriod = 3; break;
				case 5: speakerPeriod = 4; break;
                                case 6: speakerPeriod = 5; break;
			}
			break;
		case Decrement:
			switch(speakerPeriod) {
                                case 1: speakerPeriod = 2; break;
                                case 2: speakerPeriod = 3; break;
                                case 3: speakerPeriod = 4; break;
                                case 4: speakerPeriod = 5; break;
                                case 5: speakerPeriod = 6; break;
                                case 6: speakerPeriod = 6; break;
                        }
			break;
		default:
			break;
	} //State actions
}

void main() {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as outputs
    DDRB = 0xFF; PORTB = 0x00; //Configure port B's 8 pins as outputs
    
    unsigned long three_elapsedTime = 0;
    unsigned long blinking_elapsedTime = 0;
    unsigned long speaker_elapsedTime = 0;
    const unsigned long timerPeriod = 1;

    TimerSet(1);
    TimerOn();
    
    PORTB = 0x00; //Initial value of PORTB

    //Initial States
    threeState = Init1;
    blinkingState = Init2;
    combinedState = Init3;
    speakerState = Init4;
    F_State = Init5;

    while (1) { 

	unsigned char welp = ~PINA & 0x04; //Read A2

	//Calls to SMs
	if(three_elapsedTime >= 300) {
		threeLEDsSM();
		three_elapsedTime = 0;
	}
	if(blinking_elapsedTime >= 1000) {
		blinkingLEDSM();
		blinking_elapsedTime = 0;
	}

	if(welp) {
	if(speaker_elapsedTime >= speakerPeriod) {
		speakerSM();
		speaker_elapsedTime = 0;
	}
	}
	changeFrequency();

	combineLEDsSM();

	while(!TimerFlag){} //Wait for SM's period
	TimerFlag = 0; //Lower flag
	three_elapsedTime += timerPeriod;
	blinking_elapsedTime += timerPeriod;
	speaker_elapsedTime += timerPeriod;
    }    
}
