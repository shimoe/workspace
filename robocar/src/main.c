#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"

#define SENSOR_L 0
#define SENSOR_R 1
#define POWER 20000
#define DIFF 5000
#define LED_N 0
#define LED_G 1
#define LED_O 2
#define LED_D 3

#define GRAY 150
#define BLACK 500

#define PITCH 128
#define VOLUME 100

#define GOALTIME 1500
/*------------------------------------------------
  function prototype
  ------------------------------------------------*/
void RunOneSide(int side,int sensor);
int Sensor(int var);
void AcrossBlack(int var1,int var2);
void MtrRunlv(int left,int right);
void AcrossGray(void);
void Turn(int mode);
void LookBack1(void);
void LookBack2(void);
void AcrossT(void);
void Goal(void);
void Action(int mode,int left,int right,int var1,int var2);

/*------------------------------------------------
  grobal variable
  ------------------------------------------------*/
int count = 0;

/*------------------------------------------------
  main
  --------------------------------------------------*/
int main(void){
  const unsigned short MainCycle = 60;
  Init(MainCycle);

  int left,right,var1,var2;

  while(1){
    left = ADRead(0);
    right = ADRead(1);

    var1 = Sensor(left);
    var2 = Sensor(right);

    Action(count,left,right,var1,var2);

  }
  return 0;
}

/*------------------------------------------------
  original function
  ------------------------------------------------*/
void RunOneSide(int side,int sensor){
  switch(side){
  case 0:
    if(sensor > GRAY) MtrRunlv(POWER-DIFF,POWER+DIFF);
    else MtrRunlv(POWER+DIFF,POWER-DIFF);
  LED(LED_O);
  break;

 case 1:
   if(sensor > GRAY) MtrRunlv(POWER+DIFF,POWER-DIFF);
   else MtrRunlv(POWER-DIFF,POWER+DIFF);
   LED(LED_G);
   break;
 default: MtrRunlv(0,0); break;
}
}

int Sensor(int var){
  if(var > GRAY) return 0;
  else return 2;
}

void AcrossBlack(int var1,int var2){
    LED(LED_N);
    MtrRunlv(POWER,POWER);
    if(ADRead(0) < GRAY){
      while(ADRead(1) > GRAY){
	MtrRunlv(0,POWER);
      }
      count++;
    }

    if(ADRead(1) < GRAY){
      while(ADRead(0) > GRAY){
	MtrRunlv(POWER,0);
      }
      count++;
    }
}

void MtrRunlv(int left,int right){
	Mtr_Run_lv(right,-left,0,0,0,0);
}

void AcrossGray(void){
  BuzzerSet(PITCH,VOLUME);
  BuzzerStart();
  while(1){
    if(ADRead(SENSOR_L) < GRAY){
      count++;
      break;
    }

    if(ADRead(SENSOR_R) > GRAY){
      LED(LED_G);
      MtrRunlv((int)((POWER + DIFF)/2),(int)((POWER - DIFF)/2));
    } else{
      LED(LED_O);
      MtrRunlv((int)((POWER - DIFF)/2),(int)((POWER + DIFF)/2));
    }
  }
}

void Turn(int mode){
	LED(LED_N);
  switch(mode){
  case 4:
    while(ADRead(SENSOR_R) > GRAY){
    	LED(LED_G);
    	MtrRunlv(-DIFF,POWER);
    }
    while(1){
    	if(ADRead(SENSOR_L) > GRAY){
	  count++;
	  break;
    	}
    	LED(LED_N);
    	RunOneSide(SENSOR_R,ADRead(SENSOR_R));
    }
    break;

  case 9:
  case 10:
    while(ADRead(SENSOR_R) > GRAY) RunOneSide(SENSOR_L,ADRead(SENSOR_L));
    while(ADRead(SENSOR_R) < GRAY) RunOneSide(SENSOR_L,ADRead(SENSOR_L));
    while(ADRead(SENSOR_R) > GRAY) RunOneSide(SENSOR_L,ADRead(SENSOR_L));
    count++;
  }
}

void LookBack1(void){
  while(1){
    LED(LED_G);
    RunOneSide(SENSOR_R,ADRead(SENSOR_R));
    if(ADRead(SENSOR_L) < GRAY){
      count++;
      break;
    }
  }
}

void AcrossT(void){
  while(ADRead(SENSOR_R) > GRAY) RunOneSide(SENSOR_L,ADRead(SENSOR_L));
  count++;
}

void Goal(void){
MtrRunlv(POWER,POWER);
Wait(GOALTIME);
}

void Action(int mode,int left,int right,int var1,int var2){
  switch(mode){
  case 0:
  case 1:
  case 2:
  case 7:
    BuzzerStop();
    RunOneSide(SENSOR_L,left);
    if((var1 + var2) < 2) AcrossBlack(var1,var2);
    break;

  case 5: AcrossBlack(var1,var2);

  case 3:
  case 6:
    RunOneSide(SENSOR_L,left);
    if((var1 + var2) < 2) AcrossGray();
    break;

  case 4:
  case 10:
  case 11:
    BuzzerStop();
    RunOneSide(SENSOR_L,left);
    if((var1 + var2) < 2) Turn(count);
    break;

  case 8:
    RunOneSide(SENSOR_L,left);
    if((var1 + var2) < 2) LookBack1();
    break;

  case 9:
    while(ADRead(SENSOR_L) < GRAY) MtrRunlv(POWER,-POWER);
    RunOneSide(SENSOR_L,left);
    if(var2 < 0) count++;
    break;

  case 12:
    RunOneSide(SENSOR_L,left);
    if(var2 < 2) AcrossT();
    break;

  case 13:
    RunOneSide(SENSOR_R,right);
    if((var1 + var2) < 2)  Goal();
  }
}
