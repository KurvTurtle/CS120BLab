/*	Author: Christian Melendez
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: 
 *      Demonstration Link: https://drive.google.com/open?id=12qh7MrA0URr2iLwwXb0KgEW9Cg_EpPhy 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
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

enum States{Init, LED0, LED1, LED2, Stay, Rest, Reset} state;

void LEDGame() {
	unsigned char input = ~PINA & 0x01; //Read A0
	static unsigned char score = 5;

	switch(state) { //Transitions
		case Init: //Initial State
			state = LED0;
			break;
		case LED0:
			if(input==0x01){state=Stay;}
			else{state = LED1;}
			break;
		case LED1:
			if(input==0x01){state=Stay;}
			else{state= LED2;}
                        break;
		case LED2:
			if(input==0x01){state=Stay;}
			else{state = LED0;}
                        break;
		case Stay:
			if(input==0x01){state=Stay;}
			else if(input==0x00){state=Rest;}
			break;
		case Rest:
			if(input==0x01){state=Reset;}
			else if(input==0x00){state=Rest;}
			break;
		case Reset:
			if(input==0x01){state=Reset;}
			else if(input==0x00){state=LED0;}
			break;
		default:
			state = LED0;
			break;
	} //Transitions

	switch(state) { //State actions
		case Init:
			PORTB = 0x00;
			break;
		case LED0:
			PORTB = 0x01;
			break;
		case LED1:
                        PORTB = 0x02;
                        break;
		case LED2:
                        PORTB = 0x04;
                        break;
		case Stay:
			LCD_ClearScreen();
			if(PORTB == 0x01){
				if(score > 0){score--;}
				LCD_WriteData(score + '0');
				PORTB = 0x01;
			} else if(PORTB == 0x02){
				score++;
				if(score < 9){LCD_WriteData(score + '0');}
				else{LCD_DisplayString(1, "WINNER");}
				PORTB = 0x02;
			} else if(PORTB == 0x04){
				if(score > 0){score--;}
				LCD_WriteData(score + '0');
				PORTB = 0x04;
			}
			break;
		case Rest:
			if(PORTB == 0x01){
                                PORTB = 0x01;
                        } else if(PORTB == 0x02){
                                PORTB = 0x02;
                        } else if(PORTB == 0x04){
                                PORTB = 0x04;
                        }

			break;
		case Reset:
			break;
		default:
			PORTB = 0x01;
			break;
	} //State actions
}


void main() {
    DDRA = 0x00; PORTA = 0xFF; //Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0xFF; //Configure port B's 8 pins as outputs
    DDRC = 0xFF; PORTC = 0x00; //Configure port C's 8 pins as outputs
    DDRD = 0xFF; PORTD = 0x00; //Configure port D's 8 pins as outputs

    LCD_init(); //Initializes the LCD Screen
    LCD_WriteData(5 + '0'); //Initial value of LCD Screen
    
    TimerSet(300);
    TimerOn();
    
    PORTB = 0x00; //Initial value of PORTB

    state = Init; //Set initial state

    while (1) { 
	//User Code
	LEDGame(); //Execute 1 SM tick
	while(!TimerFlag){} //Wait for SM's period
	TimerFlag = 0; //Lower flag
	}    
}

