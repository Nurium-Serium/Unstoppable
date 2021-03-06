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

int vel_max = 50;
int vel_curva_st = 40;
int vel_curva_hd = 10;
int vel_rotl = 20;
int vel_roth = 60;
int count= 0;
int buffer_size= 8;
int buffer[8];
int flag= 0;
char decision_array[100];
int head= 0;
int front_flag= 0;

void turnLeft(void){
	setVel2(-vel_curva_hd, vel_roth);
	delay(150);
	decision_array[head]= "L";
	optim();
}

void slight_Left(int vel){
	setVel2(vel_max, vel);
}

void turnRight(void){
	setVel2(vel_roth, -vel_curva_hd);
	delay(150);
}

void slight_Right(int vel){
	setVel2(vel, vel_max);
}

void turnBack(void){
	setVel2(vel_curva_st, -vel_curva_st);
	delay(150);
}
void in_front(void){
	setVel2(vel_max,vel_max);
}

void end(void){
	count++;
	if(count>= buffer_size){
		count= 0;
	}
}

void optim(void){
	if(head<3){
		break;
	}
	else if(head== 100) head= 0;
	else{
		if(decision_array[head]== "B" && front_flag== 0){
			front_flag= 1;
			head++;
		}
		else if(decision_array[head]== "B" && front_flag== 1){
			front_flag= 2;
			head++;
		else if(decision_array[head]== "R" && front_flag== 2){
			head= head-2;
			decision_array[head]= "F";
			front_flag= 0;
		}
	}
}

int main(void)
{
   int groundSensor;
   //int buffer[8];

   initPIC32();
   closedLoopControl( true );
   setVel2(0, 0);

   printf("PathFinder example\n\n\n");

   while(1)
   {
      printf("Press start to continue\n");
      while(!startButton())
      {
         waitTick80ms();
         groundSensor = readLineSensors(0);	// Read ground sensor
         printInt(groundSensor, 2 | 5 << 16);
         printf("\n");
      }

      do
      {
         groundSensor = readLineSensors(0);	// Read ground sensor

         switch(groundSensor)
         {
            case 0x04:  // 0b00100:
               in_front();
               break;
            case 0x0C:  // 0b01100:
               slight_Right(vel_curva_st);
               break;
            case 0x18:  // 0b11000:
               turnLeft();
               break;
	    case 0x08:  // 0b01000:
               turnLeft();
               break;
            case 0x1c:  // 0b11100:
               turnLeft();
               break;
            case 0x06:  // 0b00110:
               slight_Left(vel_curva_st);
               break;
            case 0x03:  // 0b00011:
               turnRight();
               break;
            case 0x07:  // 0b00111:
               turnRight();
               break;
	    case 0x02:  // 0b00010:
               turnRight();
               break;
            case 0x0e:	//0b01110
            	in_front();
            case 0x1f:
            	
            	//flag++;
            	//in_front();
            	//delay(50);
            	//if(groundSensor= 0x1f){
            		//setVel2(0,0);
            	//}
            	
            	//else turnRight();
		turnRight();
            	break;
            case 0x00:
            	setVel2(-vel_curva_st, vel_curva_st);
            	break;	
            default:
               break;
         }
      } while(!stopButton());
      setVel2(0, 0);
   }
   return 0;
}

