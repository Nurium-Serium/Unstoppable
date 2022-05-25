#include "mr32.h"
int main(void){
	int groundSensor;
    	initPIC32();
    	closedLoopControl(false);
    	setVel2(0, 0);
	while(1){
	    	printf("Press start to continue\n");
	        while(!startButton()){
	        	waitTick80ms();
	         	groundSensor = readLineSensors(0);	// Read ground sensor
	         	printInt(groundSensor, 2 | 5 << 16);
	         	printf("\n");
	        }
		do{
			groundSensor = readLineSensors(0);
	    		switch(groundSensor){
				case 0x04:
					setVel2(45,45); //00100
					break;
				case 0x0c:
					setVel2(40,45); //01100
					break;
				case 0x06:
					setVel2(45,40); //00110
					break;
				case 0x00: 
					setVel2(40,-40);
					break;
				case 0x18:
					setVel2(
			}
        
	      	}while(!stopButton());
	      	setVel2(0, 0);
	}
    	return 0;
}
