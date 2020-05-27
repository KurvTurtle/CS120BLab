//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i

static int zeroMotion = 0;

/*complete the state machine*/

void Detect_EQ() {
    motionAmplitude = (~PINA >> 3) & 0x1F;

    switch(detect_eq_state){
        case DEQInit:
	    detect_eq_state = DEQRead;
	case DEQRead:
		break;
        default:
        	detect_eq_state = DEQInit;
        	break;
    }

    switch(detect_eq_state){
        case DEQInit:
        	break;
	case DEQRead:
		if(motionAmplitude > 0) {
			DEQOutput = 0x02;
			zeroMotion = 0;
		} else if(motionAmplitude == 0){
			zeroMotion++;
			if(zeroMotion >= 10) { //If motionAmplitude is 0 for 10 ticks
				DEQOutput = 0x00;
			}
		}
        default:
        	break;
    }
}
