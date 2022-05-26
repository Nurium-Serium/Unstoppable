#include "mr32.h"
int buffer[8];
int head=0;
int vel_max, vel_curva_st, vel_curva_hd, vel_rotl, vel_roth, vel;
vel_max = 50;
vel_curva_st = 35;
vel_curva_hd = 10;
vel_rotl = 20;
vel_roth = 50;
enum dir{right, slightRight, left, slightLeft, back, front, stop};
void reset_buffer(){
	int i;
	for(i=0;i<8;i++) buffer[i]=-1;
	head=0;
}
enum dir direction(int groundSensor){
	if(head==8) head=0;
	buffer[head]=groundSensor;
	printInt(groundSensor, 2 | 5 << 16);
	printf("\n");
	head++;
	int i;
	int countR=0,countF=0,countL=0,countB=0;
	for(i=0;i<=head;i++){
		if(buffer[i]==0x1f){ 
			countF++;
			countL=0;
			countB=0;
			countR=0;
		}	//11111
		else if(buffer[i]==0x03||buffer[i]==0x01){
			countR++; 	
			countL=0;
			countB=0;
			countF=0;
		}	//00011 || 00001
		else if(buffer[i]==0x18||buffer[i]==0x10){
			countL++;
			countF=0;
			countB=0;
			countR=0;
		}	//11000 || 10000
		else if(buffer[i]==0x00){
			countB++;
			countL=0;
			countF=0;
			countR=0;
		}	//00000
		else{
			countB=0;
			countL=0;
			countF=0;
			countR=0;
		}
	}
	printf("B: %d, R: %d, L: %d, F: %d\n",countB,countR,countL,countF);
	if(groundSensor==0x1c || groundSensor==0x04) return front;
	else if(groundSensor==0x02||groundSensor==0x06) return slightRight;
	else if(groundSensor==0x0c||groundSensor==0x08) return slightLeft; 
	else if(countR>=2 || countF<=6) return right;
	else if(countL>=2) return left;
	else if(countB>=2) return back;
	else if(countF>=7) return stop;
	else return -1;
}
void turnLeft(void){
	setVel2(-vel_rotl, vel_roth);
}

void slight_Left(int vel){
	setVel2(vel, vel_max);
}

void turnRight(void){
	setVel2(vel_roth, -vel_rotl);
}

void slight_Right(int vel){
	setVel2(vel_max, vel);
}

void turnBack(void){
	//setVel2(vel_curva_st, -vel_curva_st);
	setVel2(40,-40);
	
}
void in_front(void){
	setVel2(vel_max,vel_max);
}
int main(void){
	int groundSensor;
	int option=-1;
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
					if(groundSensor==0x02)	slight_Right(vel_curva_hd);
					else slight_Right(vel_curva_st);
					break;
				case slightLeft:
					if(groundSensor==0x08)	slight_Left(vel_curva_hd);
					else slight_Left(vel_curva_st);
					break;
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
		reset_buffer();
  	}
	return 0;
}
