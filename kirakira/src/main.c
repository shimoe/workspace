/*
キラキラ星
14104064 Kouta Shimomatsubae
2015/10/20
 */

#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"

#define BUZZER_TIME 400
#define RHYTHM 100
#define VOLUME 100


void BassDo(){
	BuzzerSet(179, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(100);
}

void Re(){
	BuzzerSet(160, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(100);
}

void Mi(){
	BuzzerSet(142, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(100);
}

void Fa(){
	BuzzerSet(134, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(100);
}

void So(){
	BuzzerSet(120, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(100);
}

void Ra(){
	BuzzerSet(107, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(RHYTHM);
}

void Si(){
	BuzzerSet(95, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(RHYTHM);
}

void HighDo(){
	BuzzerSet(90, VOLUME);
	BuzzerStart();
	Wait(BUZZER_TIME);
	BuzzerStop();
	Wait(RHYTHM);
}

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	BassDo();
	BassDo();
	So();
	So();
	Ra();
	Ra();
	So();
	Wait(100);

	Fa();
	Fa();
	Mi();
	Mi();
	Re();
	Re();
	BassDo();
	Wait(100);

	So();
	So();
	Fa();
	Fa();
	Mi();
	Mi();
	Re();
	Wait(100);


	return 0;
}

