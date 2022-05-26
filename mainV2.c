/**
 * \file
 * \brief example_pathfinder.c (PathFinder - basic example)
 */

// ****************************************************************************
// EXAMPLE_PATHFINDER.C
//
// To compile this example: pcompile example_pathfinder.c mr32.c
// ****************************************************************************
//
#include "mr32.h"
//int vel_max, vel_curva_st, vel_curva_hd, vel_rotl, vel_roth;

int vel_max = 65;
int vel_curva_st = 40;
int vel_curva_hd = 10;
int vel_rotl = 20;
int vel_roth = 50;
enum dir{R,L,SR,SL,F,B,S};
int direction(int groundS){
	if(groundS==0x04 || groundS==0x1c) return F;
	else if(groundS==0x08 || groundS==0x0c) return SL;
	else if(groundS==0x06 || groundS==0x02) return SR;
	else if(groundS==0x03 || groundS==0x01) return R;
	else if(groundS==0x18 || groundS==0x10) return L;
	else if(groundS==0x00) return B;
	else if(groundS==0x1f) return S;
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
	setVel2(vel_curva_st, -vel_curva_st);
}
void in_front(void){
	setVel2(vel_max,vel_max);
}

int main(void){
	int groundSensor,bef;
	int count=0,countR=0,countL=0;
	int option;
   	initPIC32();
   	closedLoopControl( true );
   	setVel2(0, 0);

   	printf("PathFinder example\n\n\n");

   	while(1){
    	printf("Press start to continue\n");
      	while(!startButton()){
         	waitTick80ms();
         	groundSensor = readLineSensors(0);	// Read ground sensor
         	printInt(groundSensor, 2 | 5 << 16);
         	printf("\n");
      	}
		bef=groundSensor;
		do{
        	groundSensor = readLineSensors(0);	// Read ground sensor
			if(groundSensor!=0x1f) count=0;
			option=direction(groundSensor);
			switch(option){
				case F:
					in_front();
					break;
				case SL:
					if(groundSensor==0x08) slight_Left(vel_curva_hd);
					else slight_Left(vel_curva_st);
					break;
				case SR:
					if(groundSensor==0x02) slight_Right(vel_curva_hd);
					else slight_Right(vel_curva_st);
					break;
				case R:
					if(groundSensor==bef) turnLeft();
					/*
					countR++;
					if(countR==2){					
						turnRight();
						countR=0;
					}*/
					break;
				case L:
					if(groundSensor==bef) turnRight();
					/*countL++;
					if(countL==2){					
						turnLeft();
						countL=0;
					}*/
					break;
				case B:
					turnBack();
					break;
				case S:
					count++;
					break;
				default:
					break;
			}
			if(count==7) break;
			bef=groundSensor;
	}while(!stopButton());
      	setVel2(0, 0);
   	}
   return 0;
}
