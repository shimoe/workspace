#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"


#define PITCH 100
#define VOLUME 100
#define SCALE 179

#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1
#define COUNT 10
#define STEP 100

#define NONE 0
#define GREEN 1
#define ORANGE 2
#define BOTH 3

#define WAIT 1000


void beep(void);
void count(int value);
int check_ps(int side);


int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	int value;
	int sensor = LEFT_SENSOR;

	while(1){
		LED(GREEN);
		Wait(WAIT);
		value = check_ps(sensor);
		count(value);
		LED(ORANGE);
		Wait(WAIT);
	}

	return 0;
}
void beep(){
	BuzzerSet(SCALE, VOLUME);
	BuzzerStart();
	Wait(PITCH);
	BuzzerStop();
	Wait(PITCH);
}

void count(int value){
	int i;

	LED(NONE);
	for(i = 0;i < (value/100);i++){
		beep();
	}
}

int check_ps(int side){
	int i;
	int sum= 0;

	LED(BOTH);
	for(i = 0;i < COUNT;i++){
		sum =sum + ADRead(side);
		Wait(STEP);
	}
	return sum/COUNT;
}
