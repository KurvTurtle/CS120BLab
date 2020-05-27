//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i



/*complete the state machine*/

void Transmit(){


    switch(transmit_state){
        case TInit:
	    transmit_state = TWrite;
            break;
	case TWrite:
	    break;
        default:
            transmit_state = TInit;
            break;
    }
    switch(transmit_state){
        case TInit:
            break;
	case TWrite:
	    PORTB = pingOutput | DEQOutput | maxOutput | zcOutput;
	    break;
        default:
            break;
    }

}


