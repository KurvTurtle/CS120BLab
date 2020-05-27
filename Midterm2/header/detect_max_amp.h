//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i

/* complete the state machine. */

void Detect_Max_Amp(){
    motionAmplitude = (~PINA >> 3) & 0x1F; //Reads from A7-A3 and shifts to bits 0 to 4

    switch(detect_max_amp_state){
        case DMAInit:
		detect_max_amp_state = DMARead;
       		break;
	case DMARead:
		break;
        default:
            	break;
    }

    switch(detect_max_amp_state){
        case DMAInit:
		maxOutput = 0x00;
                break;
	case DMARead:
		if(motionAmplitude == 0) {
			maxOutput = 0x00;
		} else if(motionAmplitude > maxOutput) {
			maxOutput = motionAmplitude; //Writes to B4-B0
			maxOutput = maxOutput << 3; //Writes to B7-B3
		}
		break;
        default:
                break;
    }
}
