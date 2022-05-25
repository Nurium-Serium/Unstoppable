#include "mr32.h"
int buffer[8];
int head=0;
int vel_max, vel_curva_st, vel_curva_hd, vel_rotl, vel_roth, vel;
vel_max = 65;
vel_r=43;
vel_curva_st = 40;
vel_curva_hd = 10;
vel_rotl = 20;
vel_roth = 50;
enum dir{right, slightRight, left, slightLeft, back, front, stop};
void reset_buffer(){
	int i;
	for(i=0;i<5;i++) buffer[i]=-1;
}
int direction(int groundSensor){
	if(head==8) head=0;
	buffer[head]=groundSensor;
	head++;
	int i;
	int countR=0,countF=0,countL=0,countB=0;
	for(i=0;i<head;i++){
		if(buffer[i]==0x1f) countF++;	//11111
		if(buffer[i]==0x03) countR++; 	//00011
		if(buffer[i]==0x18) countL++;	//11000
		if(buffer[i]==0x00) countB++;	//00000
	}
	printf("%d\n",countB);
	if(groundSensor==0x1c || groundSensor==0x04)return front;
	else if(countR<2 && countL>=2) return left;
	else if(groundSensor==0x06) return slightRight;
	else if(groundSensor==0x0c) return slightLeft; 
	else if(countR>=2 || countF<=5) return right;
	else if(countB>=5) return back;
	else if(countF>=7) return stop;
	else return -1;
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
	closedLoopControl( true );
	reset_buffer();
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
				case left:
					turnLeft();
					break;
				default:
					break;
					
			}
			if(option==stop) break;
			waitTick80ms();
		}while(!stopButton());
		setVel2(0, 0);
  	}
	return 0;
}
