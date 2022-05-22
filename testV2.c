#include "mr32.h"
int buffer[5];
int head=0;
int vel_max, vel_curva_st, vel_curva_hd, vel_rotl, vel_roth, vel;
vel_max = 65;
vel_r=35;
vel_curva_st = 40;
vel_curva_hd = 10;
vel_rotl = 20;
vel_roth = 50;
enum dir{right, slightRight, left, slightLeft, back, front, stop};
void reset_buffer(){
	int i;
	for(i=0;i<5;i++) buffer[i]=0;
}
int direction(int groundSensor){
	buffer[head]=groundSensor;
	head++;
	if(head==5) head=0;
	int i;
	int countR=0,countF=0;
	for(i=0;i<head;i++){
		if(buffer[i]==0x1f) countF++;	//11111
		if(buffer[i]==0x03) countR++; 	//00011
	}
	if(groundSensor==0x04) return front;
	else if(groundSensor==0x00) return back;
	else if(groundSensor==0x06) return slightRight;
	else if(groundSensor==0x0c) return slightLeft;
	else if(countR>=2) return right;
	else if(countF<=3) return right;
	else return stop;
}
void turnLeft(void){
	setVel2(-vel_rotl, vel_roth);
}

void slight_Left(int vel){
	setVel2(vel_r, vel);
}

void turnRight(void){
	setVel2(vel_roth, -vel_rotl);
}

void slight_Right(int vel){
	setVel2(vel, vel_r);
}

void turnBack(void){
	setVel2(vel_curva_st, -vel_curva_st);
}
void in_front(void){
	setVel2(vel_r,vel_r);
}
int main(void){
	int groundSensor;
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
				case front:
					in_front();
					break;
				case right:
					turnRight();
					break;
				case slightRight:
					slight_Right(vel_curva_st);
					break;
				case slightLeft:
					slight_Left(vel_curva_st);
				case back:
					turnBack();
					break;
				default:
					break;
					
			}
			if(option==stop) break;
		}while(!stopButton());
		setVel2(0, 0);
  	}
	return 0;
}
