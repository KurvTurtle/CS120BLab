/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: 
 *      Demonstration Link: https://drive.google.com/open?id=1MYjKzo-dHF9pTqwTE1Efnea6bB2kHy7N
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; //TimerISR() sets this to 1, programmer should clear to 0

//Internal variables for mapping AVR's ISR to our cleaner TimerISR() model
unsigned long _avr_timer_M = 1; //Start count from here, down to 0, default 1ms
unsigned long _avr_timer_cntcurr = 0; //Current internal count of 1ms ticks

void TimerOn() {
	//AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B; //bit3 = 0: CTC mode (clear timer on compare)
		       //bit2bit1bit0 = 011: pre-scaler /64
		       //00001011: 0x0B
		       //8MHz clock or 8,000,000 /64 = 125,000 ticks/s
		       //TCNT1 register will count at 125,000 ticks/s

	//AVR output compare register OCR1A
	OCR1A = 125; //Timer interrupt will be generated when TCNT1 == OCR1A
                     //We want a 1ms tick, 0.001s * 125,000 ticks/s = 125
		     //When TCNT1 register equals 125, 1ms has passed, thus compare to 125
		     
	//AVR timer interrupt mask register
	TIMSK1 = 0x02; //bit1: OCIE1A -- enables compare match interrupt

	//Initialize AVR counter
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M; //TimerISR() will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; //0x80: 1000000
	}

void TimerOff() {
	TCCR1B = 0x00; //bit3bit1bit0-000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

//In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	
	//CPU automatically calls when TCNT1 == OCR1 (every 1ms per TimerISR() settings)
	_avr_timer_cntcurr--; //Count down to 0 rather than up to TOP

	if(_avr_timer_cntcurr == 0) { //Results in a more efficient compare
		TimerISR(); //Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

//Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

//Shared Variables
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;


//SM States
enum States1{Init1, One, Two, Three} threeState;
enum States2{Init2, On, Off} blinkingState;
enum States3{Init3, Read} combinedState;

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
			PORTB = blinkingLED | threeLEDs;
			break;
                default:
                        break;
        } //State actions
}


void main() {
    DDRB = 0xFF; PORTB = 0xFF; //Configure port B's 8 pins as outputs
    
    unsigned long three_elapsedTime = 0;
    unsigned long blinking_elapsedTime = 0;
    const unsigned long timerPeriod = 100;

    TimerSet(100);
    TimerOn();
    
    PORTB = 0x00; //Initial value of PORTB

    //Initial States
    threeState = Init1;
    blinkingState = Init2;
    combinedState = Init3;

    while (1) { 
	//Calls to SMs
	if(three_elapsedTime >= 300) {
		threeLEDsSM();
		three_elapsedTime = 0;
	}
	if(blinking_elapsedTime >= 1000) {
		blinkingLEDSM();
		blinking_elapsedTime = 0;
	}

	combineLEDsSM();

	while(!TimerFlag){} //Wait for SM's period
	TimerFlag = 0; //Lower flag
	three_elapsedTime += timerPeriod;
	blinking_elapsedTime += timerPeriod;
	}    
}
