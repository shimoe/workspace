#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"

void green(int n);
void green_and_orange(int t,int n);
void blink_for_ever(void);
void buzzer_beat(void);

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	int t,n;

	BuzzerSet(179, 100);
	t = 500;
	n = 5;

	green(n);
	buzzer_beat();
	green_and_orange(t,n);
	buzzer_beat();
	blink_for_ever();
	buzzer_beat();

	return 0;
}

void green(int n){
	int i;
	for(i = 0;i < n;i++){
		LED(1);
		Wait(1000);
	}
}

void green_and_orange(int t,int n){
	int i;
	for(i = 0;i < n;i++){
		LED(1);
		Wait(t);
		LED(2);
		Wait(t);
	}
}

void blink_for_ever(void){
	while(1){
		LED(0);
		Wait(500);
	LED(1);
	Wait(500);
	LED(2);
	Wait(500);
	LED(3);
	Wait(500);
}
}


void buzzer_beat(void){
	BuzzerStart();
	Wait(1000);
	BuzzerStop();
	Wait(100);
}
