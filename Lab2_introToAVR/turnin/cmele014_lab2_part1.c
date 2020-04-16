/*	Author: cmele014
 *  Partner(s) Name: Jo Smith
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //input
    DDRB = 0xFF; PORTB = 0x00; //output

    unsigned char tempB = 0x00;

    while (1) {
        tempB = 0x00;
        tempB = ~PINA;
        PORTB = tempB;
    }
    return 1;
}
