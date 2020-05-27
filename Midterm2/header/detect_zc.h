//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i


static int t = 0;
unsigned char motionDirection1 = 0x00;
unsigned char motionDirection2 = 0x00;

/*complete the state machine*/


void Detect_ZC() {
	if(t == 0) {
		unsigned char motionDirection1 = ~PINA & 0x07; //Reads A0, A1, & A2
		t = 1;
	} else if (t == 1) {
		unsigned char motionDirection2 = ~PINA & 0x07; //Reads A0, A1, & A2
		t = 0;
	}

    switch(detect_zc_state){
        case DZCInit:
		detect_zc_state = DZCRead;
                break;
        case DZCRead:
		break;    
        default:
                detect_zc_state = DZCInit;
                break;
    }
    switch(detect_zc_state){
        case DZCInit:
        	    break;
	case DZCRead:
		    if(t == 1) {
		    	switch(motionDirection1) {
			    case 0x00:
				    if(motionDirection2 == 0x04) {
					    zcOutput = 0x04;
				    }
				    break;
			    case 0x01:
				    if(motionDirection2 == 0x05) {
                                            zcOutput = 0x04;
                                    }
				    break;
			    case 0x02:
				    if(motionDirection2 == 0x06) {
                                            zcOutput = 0x04;
                                    }
				    break;
			    case 0x03:
				    if(motionDirection2 == 0x07) {
                                            zcOutput = 0x04;
                                    }
				    break;
			    case 0x04:
				    if(motionDirection2 == 0x00) {
                                            zcOutput = 0x04;
                                    }
				    break;
			    case 0x05:
				    if(motionDirection2 == 0x01) {
                                            zcOutput = 0x04;
                                    }
				    break;
			    case 0x06:
				    if(motionDirection2 == 0x02) {
                                            zcOutput = 0x04;
                                    }
				    break;
			    case 0x07:
				    if(motionDirection2 == 0x03) {
                                            zcOutput = 0x04;
                                    }
				    break;
		    	}
		    } else if (t == 0) {
			    switch(motionDirection2) {
                            case 0x00:
                                    if(motionDirection1 == 0x04) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x01:
                                    if(motionDirection1 == 0x05) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x02:
                                    if(motionDirection1 == 0x06) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x03:
                                    if(motionDirection1 == 0x07) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x04:
                                    if(motionDirection1 == 0x00) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x05:
                                    if(motionDirection1 == 0x01) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x06:
                                    if(motionDirection1 == 0x02) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                            case 0x07:
                                    if(motionDirection1 == 0x03) {
                                            zcOutput = 0x04;
                                    }
                                    break;
                        }

		    }
		    break;
        default:
	            break;
    }
}
