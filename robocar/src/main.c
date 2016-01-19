#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"

#define SENSOR_L 0
#define SENSOR_R 1
#define POWER 10000
#define DIFF 5000
#define LED_N 0
#define LED_G 1
#define LED_O 2
#define LED_D 3 

#define GRAY 500
#define BLACK 250


void RunOneSide(int side,int sensor);
int Sensor(int var);

int main(void){
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	int count = 0;
	int i,j,left,right,var1,var2;



	while(1){
	  left = ADRead(0);
	  right = ADRead(1);

	  var1 = Sensor(left);
	  var2 = Sensor(right);

	  RunOneSide(SENSOR_L,left);
	  

	  if(1){
	    LED(3);
	    while(1) Mtr_Run_lv(0,0,0,0,0,0);
	  }

	  /* while((var1 + var2) < 2){
	    LED(LED_D);
	    Mtr_Run_lv(POWER,-POWER,0,0,0,0);
	    if(ADRead(0) > GRAY){
	      while(ADRead(1) < GRAY){
		LED(LED_N);
		 Mtr_Run_lv(POWER+DIFF,-(POWER-DIFF),0,0,0,0);
	      }
	      count++;
	      break;
	    }

	    if(ADRead(1) > GRAY){
	      while(ADRead(0) < GRAY){
		LED(LED_N);
		Mtr_Run_lv(POWER-DIFF,-(POWER+DIFF),0,0,0,0);
	      }
	      count++;
	      break;
	    }
	  }*/
	}
	return 0;
}

void RunOneSide(int side,int sensor){
  switch(side){
  case 0: 
    if(sensor < GRAY) Mtr_Run_lv(POWER-DIFF,-(POWER+DIFF),0,0,0,0);
    else Mtr_Run_lv(POWER+DIFF,-(POWER-DIFF),0,0,0,0);
    LED(LED_G);
    break;

  case 1:
    if(sensor < GRAY) Mtr_Run_lv(POWER+DIFF,-(POWER-DIFF),0,0,0,0);
    else Mtr_Run_lv(POWER-DIFF,-(POWER+DIFF),0,0,0,0);
    LED(LED_O);
    break;
  default: Mtr_Run_lv(0,0,0,0,0,0); break;
  }
}

int Sensor(int var){
  if(var < BLACK) return 0;
  else if(var < GRAY) return 1;
  else return 2;
}

