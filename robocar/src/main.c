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

#define GRAY 250




int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	int count = 0;
	int i,j,left,right;
	while(1){
	  left = ADRead(0);
	  right = ADRead(1);
	}
	return 0;
}

void RunOneSide(int side,int sensor){
  switch(side){
  case 0: 
if(sensor < GRAY) Mtr_Run_lv(POWER+DIFF,-(POWER-DIFF),0,0,0,0);
    else Mtr_Run_lv(POWER-DIFF,-(POWER+DIFF),0,0,0,0); break;

  case 1:
 if(sensor < GRAY) Mtr_Run_lv(POWER-DIFF,-(POWER+DIFF),0,0,0,0);
    else Mtr_Run_lv(POWER+DIFF,-(POWER-DIFF),0,0,0,0); break;
  }
}

