#include "mr32.h"
enum dir{right, left, back, front};
int direction(int groundSensor){
	int dir_left=groundSensor & (1<<4);
	int dir_right=groundSensor & 1;
	int dir_back=groundSensor & 0;
	if(dir_right==1) return right;
	else if (dir_left==1) return left;
	else if(dir_back==1) return back;
	else return front;
	
}
int main(void){
	int groundSensor;
	int vel_max, vel_rotl, vel_roth;
	vel_max = 65;
	vel_rotl = 20;
	vel_roth = 50;
	int option;
	initPIC32();
	closedLoopControl( false );
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
			option=direction(groundSensor);
			switch(option){
				case left:
					setVel2(0,0);
					delay(700);
					setVel2(-vel_rotl, vel_rotl);
					break;
				case right:
					setVel2(0,0);
					delay(700);
					setVel2(vel_rotl, -vel_rotl);
					break;
				case back:
					setVel2(0,0);
					delay(700);
					setVel2(vel_roth, -vel_roth);
					break;
				case front:
					setVel2(vel_roth,vel_roth);
					break;
			}
			waitTick80ms();
			//if(groundSensor==0x1f) break;
		}while(!stopButton());
		setVel2(0, 0);
  	}
	return 0;
}
