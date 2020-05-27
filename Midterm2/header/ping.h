//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i



static int ticks = 0;

/*complete the state machine*/

void Ping(){
    switch(ping_state){
        case PInit:
            break;
	case PWait:
	    if(ticks >= 9) {
		    ping_state = PRead; //Changes state after 9 ticks (90% Duty Cycle)
		    ticks = 0;
	    } else {
		    ticks++;
	    }
	    break;
	case PRead:
	    ping_state = PWait;
	    break;
        default:
            ping_state = PInit;
            break;
    }
    switch(ping_state){
        case PInit:
            break;
	case PWait:
	    pingOutput = 0x00; //Sets B0 to 0
	    break;
	case PRead:
	    pingOutput = 0x01; //Sets B0 to 1
        default:
            break;
    }
}

