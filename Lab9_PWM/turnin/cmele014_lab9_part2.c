/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
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

enum States{Init, C, D, E, F, G, A2, B2, C2, Off, CPressed, DPressed, EPressed, FPressed, GPressed, A2Pressed, B2Pressed, C2Pressed} state;

void PWM_Cycle() {
	unsigned char input = ~PINA & 0x07; //Read A0, A1, and A3

	switch(state) { //Transitions
		case Init: //Initial State
			state = CPressed;
			break;
		case C:
			if (input == 0x01) {state = DPressed;}
			else if (input == 0x02) {set_PWM(0);}
			break;
		case CPressed:
			if(input != 0x01 || input != 0x04) {state = C;}
			break;		
		case D:
			if (input == 0x01) {state = EPressed;}
                        else if (input == 0x02) {set_PWM(0);}
			else if (input == 0x04) {state = CPressed;}
			break;
		case DPressed:
                        if(input != 0x01 || input != 0x04) {state = D;}
                        break;
		case E:
			if (input == 0x01) {state = FPressed;}
                        else if (input == 0x02) {set_PWM(0);}
			else if (input == 0x04) {state = DPressed;}
			break;
		case EPressed:
                        if(input != 0x01 || input != 0x04) {state = E;}
                        break;
		case F:
			if (input == 0x01) {state = GPressed;}
                        else if (input == 0x02) {set_PWM(0);}
			else if (input == 0x04) {state = EPressed;}
			break;
		case FPressed:
                        if(input != 0x01 || input != 0x04) {state = F;}
                        break;
		case G:
			if (input == 0x01) {state = A2Pressed;}
                        else if (input == 0x02) {set_PWM(0);}
			else if (input == 0x04) {state = FPressed;}
			break;
		case GPressed:
                        if(input != 0x01 || input != 0x04) {state = G;}
                        break;
		case A2:
			if (input == 0x01) {state = B2Pressed;}
                        else if (input == 0x02) {set_PWM(0);}
			else if (input == 0x04) {state = GPressed;}
			break;
		case A2Pressed:
                        if(input != 0x01 || input != 0x04) {state = A2;}
                        break;
		case B2:
			if (input == 0x01) {state = C2Pressed;}
                        else if (input == 0x02) {set_PWM(0);}
			else if (input == 0x04) {state = A2Pressed;}
			break;
		case B2Pressed:
                        if(input != 0x01 || input != 0x04) {state = B2;}
                        break;
		case C2:
			if (input == 0x04) {state = B2Pressed;}
                        else if (input == 0x02) {set_PWM(0);}
			break;
		case C2Pressed:
                        if(input != 0x01 || input != 0x04) {state = C2;}
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
		default:
			set_PWM(0);
			break;
	} //State actions
}


void main() {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00; //Configure port B's 8 pins as outputs
    
    TimerSet(100);
    TimerOn();
    
    PWM_on(); //Initializes PWM

    PORTB = 0x00; //Initial value of PORTB

    state = Init; //Set initial state

    while (1) { 
	//User Code
	PWM_Cycle(); //Execute 1 SM tick
	while(!TimerFlag){} //Wait for SM's period
	TimerFlag = 0; //Lower flag
	}    
}
