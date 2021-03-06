/*
キラキラ星
14104064 Kouta Shimomatsubae
2015/10/20
 */

#include "LPC13xx.h"
#include "gpio.h"
#include "vs-wrc103.h"
#include "ixbus.h"

#define VOLUME 100

int buzzertime = 400;
int rhythm = 100;


void BassDo(){
	BuzzerSet(179, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(100);
}

void Re(){
	BuzzerSet(160, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(100);
}

void Mi(){
	BuzzerSet(142, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(100);
}

void Fa(){
	BuzzerSet(134, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(100);
}

void So(){
	BuzzerSet(120, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(100);
}

void Ra(){
	BuzzerSet(107, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(rhythm);
}

void Si(){
	BuzzerSet(95, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(rhythm);
}

void HighDo(){
	BuzzerSet(90, VOLUME);
	BuzzerStart();
	Wait(buzzertime);
	BuzzerStop();
	Wait(rhythm);
}

void kirakira1(){
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
}

void kirakira2(){
	So();
	So();
	Fa();
	Fa();
	Mi();
	Mi();
	Re();
	Wait(100);
}

void kirakira_master(){
	kirakira1();
	kirakira2();
	kirakira2();
	kirakira1();
}

void seirogan1(){
	buzzertime = 50;
	rhythm = 0;

	HighDo();

}

void seirogan(){
buzzertime = 50;
rhythm = 0;


}
int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	kirakira_master();

	return 0;
}

