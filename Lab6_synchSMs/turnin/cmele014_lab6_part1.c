/*	Author: Christian Melendez
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *      Demonstration Link: https://drive.google.com/open?id=13tt3dF82A-KVhMjaaOajmDH2665wDO3m
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

	
enum States{LED0, LED1, LED2} state;

void blinkLEDs() {
	unsigned char output = ~PINB & 0x07; //Write B0-B2

	switch(state) { //Transitions
		case LED0: //Initial State
			state = LED1;
			break;
		case LED1:
                        state = LED2;
                        break;
		case LED2:
                        state = LED0;
                        break;
		default:
			state = LED0;
	} //Transitions

	switch(state) { //State actions
		case LED0:
			output = 0x01;
			break;
		case LED1:
                        output = 0x02;
                        break;
		case LED2:
                        output = 0x04;
                        break;
		default:
			output = 0x01;
			break;
	} //State actions

	PORTB = output;
}



void main() {
    DDRB = 0xFF; PORTB = 0x00; //Configure port C's 8 pins as outputs

    PORTB = 0x00; //Initial value of PORTB
    
    TimerSet(1000);
    TimerOn();
    
    state = LED0; //Set initial state

    while (1) { 
	//User Code
	blinkLEDs(); //Execute 1 SM tick
	while(!TimerFlag){} //Wait for SM's period
	TimerFlag = 0; //Lower flag
	}    
}
