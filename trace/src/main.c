#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"


#define VOLUME 100
#define BASS_DO 179
#define RE 160
#define MI 142
#define FA 134
#define SO 120
#define RA 107
#define SI 95
#define HIGH_DO 90
#define HIGH_MI 80
#define PITCH 1600


#define WAIT 500
#define POWER 20000
#define DEFF 5000

#define LEFT_SIDE 0
#define RIGHT_SIDE 1

#define BLACK 300

void forward1(void);
void turn_left(void);
void turn_right(void);
int is_black(int side);
int both_black(void);
void to_black(void);


void sensor(void);
void fanfare();

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	while(1){
		sensor();
	}

	return 0;
}

void sensor(){

	switch(((ADRead(0) > BLACK) << 1 + (ADRead(1) > BLACK)) & 3){

	case 0 : Mtr_Run_lv(POWER,-POWER,0,0,0,0);
				break;
	case 1 : Mtr_Run_lv(POWER-DEFF,-(POWER+DEFF),0,0,0,0);
				break;
	case 2 : Mtr_Run_lv(POWER+DEFF,-(POWER-DEFF),0,0,0,0);
				break;
	case 3 : Mtr_Run_lv(0,0,0,0,0,0);
				fanfare();
				break;
	}

}


void fanfare(){
	BuzzerSet(HIGH_DO, 100);
			BuzzerStart();
			Wait(1000);
			BuzzerStop();
			Wait(1000);
}


void forward1(){
	Mtr_Run_lv(POWER,-POWER,0,0,0,0);
	Wait(130);
	Mtr_Run_lv(0,0,0,0,0,0);
	Wait(WAIT);
}

void turn_left(){
	Mtr_Run_lv(POWER,POWER,0,0,0,0);
	Wait(30);
	Mtr_Run_lv(0,0,0,0,0,0);
	Wait(WAIT);
}

void turn_right(){
	Mtr_Run_lv(-POWER,-POWER,0,0,0,0);
	Wait(30);
	Mtr_Run_lv(0,0,0,0,0,0);
	Wait(WAIT);
}

int is_black(int side){
	if(ADRead(side) > 300){
		return 1;
	}else{
		return 0;
	}
}

int both_black(){
	if(ADRead(0) > 300 && ADRead(1) > 300){
		return 1;
	}else{
		return 0;
	}
}

void to_black(){

	if(both_black()){
		Mtr_Run_lv(0,0,0,0,0,0);
		fanfare();
	}else{
		if(is_black(LEFT_SIDE) == 1 && is_black(RIGHT_SIDE) == 0){
			turn_left();
		}
		else if(is_black(LEFT_SIDE) == 0 && is_black(RIGHT_SIDE) == 1){
			turn_right();
		}
		else{
			forward1();
		}

	}

}
