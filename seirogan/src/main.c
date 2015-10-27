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

int halfnote,quarternote,eighthnote,sixthnote;




void Sound(int scale,int sigtime,int wait,int repeat){
	int i;

	for(i = 0;i < repeat;i++){
		BuzzerSet(scale, VOLUME);
		BuzzerStart();
		Wait(sigtime);
		BuzzerStop();
		Wait(wait);
	}
}

void Seirogan(){
	int wait = 50;
	halfnote = 800;//PITCH/2;
	quarternote = 400;//PITCH/4;
	eighthnote = 200;//PITCH/8;
	sixthnote = 100;//PITCH/16;

	Sound(HIGH_DO,quarternote,wait,1);
	Sound(HIGH_DO,eighthnote,wait,2);
	Sound(HIGH_DO,quarternote,wait,2);
	Sound(HIGH_MI,quarternote,wait,1);
	Sound(HIGH_MI,eighthnote,wait,2);
	Sound(HIGH_MI,quarternote,wait,2);

	Sound(HIGH_DO,quarternote,wait,1);
	Sound(HIGH_MI,quarternote,wait,1);
	Sound(HIGH_DO,quarternote,wait,1);
	Sound(HIGH_MI,quarternote,wait,1);
	Sound(SO,quarternote,wait,3);

	Sound(HIGH_DO,quarternote,wait,1);
	Sound(HIGH_DO,eighthnote,wait,2);
	Sound(HIGH_DO,quarternote,wait,2);
	Sound(HIGH_MI,quarternote,wait,1);
	Sound(HIGH_MI,eighthnote,wait,2);
	Sound(HIGH_MI,quarternote,wait,2);

	Sound(HIGH_DO,quarternote,wait,1);
	Sound(HIGH_MI,quarternote,wait,1);
	Sound(HIGH_DO,quarternote,wait,1);
	Sound(HIGH_MI,quarternote,wait,1);
	Sound(HIGH_DO,halfnote,wait,1);

}

int main(void)
{
	const unsigned short MainCycle = 60;
	Init(MainCycle);

	Seirogan();

	return 0;
}

