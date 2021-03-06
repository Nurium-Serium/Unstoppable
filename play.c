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
int vel_max, vel_curva_st, vel_curva_hd, vel_rotl, vel_roth;

vel_max = 65;
vel_curva_st = 40;
vel_curva_hd = 10;
vel_rotl = 20;
vel_roth = 50;

void turnLeft(void){
	setVel2(-vel_rotl, vel_roth);
}

void slight_Left(int vel){
	setVel2(vel_max, vel);
}

void turnRight(void){
	setVel2(vel_roth, -vel_rotl);
}

void slight_Right(int vel){
	setVel2(vel, vel_max);
}

void turnBack(void){
	setVel2(vel_curva_st, -vel_curva_st);
}
void in_front(void){
	setVel2(vel_max,vel_max);
}

int main(void)
{
   int groundSensor;

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
               slight_right(vel_curva_st);
               break;
            case 0x08:  // 0b01000:
               slight_right(vel_curva_hd);
               break;
            case 0x10:  // 0b10000:
               turnLeft();
               break;
            case 0x06:  // 0b00110:
               slight_left(vel_curva_st);
               break;
            case 0x02:  // 0b00010:
               slight_left(vel_curva_hd);
               break;
            case 0x01:  // 0b00001:
               turnRight();
               break;
            case 0x1f:
            	turnRight();
            	break;
            default:
               break;
         }
      } while(!stopButton());
      setVel2(0, 0);
   }
   return 0;
}
