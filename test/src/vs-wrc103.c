#include "LPC13xx.h"                        /* LPC13xx definitions */
#include "gpio.h"
#include "vs-wrc103.h"


BYTE bBuzzerFlag;
unsigned int iBuzzerPitch;

#define WDEN		0x00000001
#define WDRESET		0x00000002
#define WDTOF		0x00000004
#define WDINT		0x00000008

unsigned short MainCnt;// = 100;
unsigned char WDF;

unsigned char EncFlag = 0;
void Timer16Init(void);
void Timer32Init(void);
void ADCInit();

unsigned short getMainCycle(){
	return MainCnt;
}


unsigned long Sync(){
	unsigned int WDTV = 0;
	unsigned int cnt = 0;

	while(LPC_WDT->TV < (1800000/MainCnt)){
		__NOP();
		cnt++;
		if(cnt == 0)
			cnt++;
	}

	WDTV = LPC_WDT->TV;

	LPC_WDT->MOD &= ~WDTOF;		/* clear the time-out inerrupt flag */
	LPC_WDT->TC = 1800000/MainCnt;
	LPC_WDT->MOD |= WDEN;
	LPC_WDT->FEED = 0xAA;
	LPC_WDT->FEED = 0x55;

	while(WDTV <= LPC_WDT->TV){
		__NOP();
	}
	return cnt;
}

void Init(unsigned short maincnt){
	bSendUSBdata = 0;
	bBuzzerFlag = 0;
//WDT
	/* Enable AHB clock to the WDT */
	//LPC_SYSCON->WDTOSCCTRL = (0x0e << 5)|(0x01 << 1); 		//3.2M,Div 4 = 1110 00001
	//LPC_SYSCON->WDTOSCCTRL = (0x00 << 5)|(0x01 << 1); 			//72M,Div 4 = 1110 00001

	LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<15));	//PDSLEEPCFG,

	if(maincnt < 1)
		maincnt = 1;
	else if(maincnt > 1000)
		maincnt = 1000;
	MainCnt = maincnt;
	WDF = 0;
	LPC_SYSCON->WDTCLKDIV = 10;//7500/maincnt;	//32000/(maincnt*4);
	//LPC_WDT->TC = 1800000/150;
	LPC_WDT->TC = 1800000/MainCnt;
	LPC_WDT->MOD = WDEN;
	LPC_WDT->FEED = 0xAA;
	LPC_WDT->FEED = 0x55;

//SW
  GPIOSetDir( SW_PORT, SW_BIT, 0 );	//set input port pin

//LED
  GPIOSetDir( 0, 3, 1 );	//set output port pin
  GPIOSetDir( 0, 7, 1 );	//set output port pin

  //USB CONNECT output port pins
  GPIOSetDir( 0, 6, 1 );

  //ADC
  LPC_IOCON->JTAG_TDI_PIO0_11	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->JTAG_TDI_PIO0_11	|= 0x01; // ADC IN0
  LPC_IOCON->JTAG_TMS_PIO1_0	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->JTAG_TMS_PIO1_0	|= 0x01; // ADC IN1
  LPC_IOCON->JTAG_TDO_PIO1_1	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->JTAG_TDO_PIO1_1	|= 0x01; // ADC IN2
  LPC_IOCON->JTAG_nTRST_PIO1_2	&= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->JTAG_nTRST_PIO1_2	|= 0x01; // ADC IN3

#ifndef USE_LPC_LINK
  LPC_IOCON->ARM_SWDIO_PIO1_3   &= ~0x8F; // Clear bit7, change to analog mode.
  LPC_IOCON->ARM_SWDIO_PIO1_3   |= 0x01;  // ADC IN4
#endif

  LPC_IOCON->PIO1_4    &= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->PIO1_4    |= 0x01; // ADC IN5
  LPC_IOCON->PIO1_10   &= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->PIO1_10   |= 0x01; // ADC IN6
  LPC_IOCON->PIO1_11   &= 0x67; // Clear bit7, change to analog mode. PullUp disable
  LPC_IOCON->PIO1_11   |= 0x01; // ADC IN7

//MTR
  //Timer16B0,B1
  //use MTR1
  LPC_IOCON->PIO0_8   &= 0xE7;  // PullUp disable,CT16B0_MAT0
  LPC_IOCON->PIO0_8   |= 0x02;  // Selects function
  //use MTR2
  LPC_IOCON->PIO1_9   &= 0xE7; // PullUp disable,CT16B1_MAT0
  LPC_IOCON->PIO1_9   |= 0x01;  // Selects function

  //use MTR3
  LPC_IOCON->PIO1_6   &= 0xE7; // PullUp disable
  LPC_IOCON->PIO1_6   |= 0x02;  // Selects function CT32B0_MAT0

  //use MTR4
  LPC_IOCON->PIO1_7   &= 0xE7; // PullUp disable
  LPC_IOCON->PIO1_7   |= 0x02;  // Selects function CT32B0_MAT1

#ifndef USE_LPC_LINK

  //use MTR5 & LPCLINK
  LPC_IOCON->PIO0_9   &= 0xE7; // PullUp disable
  LPC_IOCON->PIO0_9   |= 0x02;  // Selects function CT16B0_MAT1

  //use MTR6 & LPCLINK
  LPC_IOCON->JTAG_TCK_PIO0_10   &= 0xE7; // PullUp disable
  LPC_IOCON->JTAG_TCK_PIO0_10   |= 0x03;  // Selects function CT16B0_MAT2
#endif


  //PIO3_1 PSPAD-SEL
  //PIO3_2 PSPAD-CLK
  //PIO3_3 PSPAD-DAT
  //PSPAD-CMD
  LPC_IOCON->PIO3_0   = 0x00;  // Selects function PIO

  //PSPAD-SEL
  LPC_IOCON->PIO3_1   = 0x00;  // Selects function PIO

  //PSPAD-CLK
  LPC_IOCON->PIO3_2   = 0x00;  // Selects function PIO

  //PSPAD-DAT
  LPC_IOCON->PIO3_3   = 0x10;  // Selects function PIO,PullUp enable


  //GPIO
  //use MTR1 GPIO
  GPIOSetDir( 2, 0, 1 );	//set output port pin
  GPIOSetDir( 2, 1, 1 );	//set output port pin
  //use MTR2 GPIO
  GPIOSetDir( 2, 2, 1 );	//set output port pin
  GPIOSetDir( 2, 3, 1 );	//set output port pin
  //use MTR3 GPIO
  GPIOSetDir( 2, 4, 1 );	//set output port pin
  GPIOSetDir( 2, 5, 1 );	//set output port pin
  //use MTR4 GPIO
  GPIOSetDir( 2, 6, 1 );	//set output port pin
  GPIOSetDir( 2, 7, 1 );	//set output port pin
  //use MTR5 GPIO
  GPIOSetDir( 2, 8, 1 );	//set output port pin
  GPIOSetDir( 2, 9, 1 );	//set output port pin
  //use enc & MTR6 GPIO
  GPIOSetDir( 2, 10, 1 );	//set output port pin
  GPIOSetDir( 2, 11, 1 );	//set output port pin


  //PIO1_3,PIO0_9,PIO0_1 is use LPCLINK
  //PS_PAD
  GPIOSetDir( 3, 0, 1 );	//set output port pin
  GPIOSetDir( 3, 1, 1 );	//set output port pin
  GPIOSetDir( 3, 2, 1 );	//set output port pin
  GPIOSetDir( 3, 3, 0 );	//set input port pin

  //Buzzer
  GPIOSetDir( 1, 8, 1 );	//set output port pin


  ADCInit();
  Timer16Init();
  Timer32Init();

  //Motor Bit Clear
  LPC_GPIO2->DATA &= ~0x0FFF;
}




void Mtr_Run_lv(short m1,short m2,short m3,short m4,short m5,short m6){
	int i = 0;
	short mta[6];
	unsigned int t_Duty[6];

	uint32_t Gpio2Temp;

	Gpio2Temp = LPC_GPIO2->DATA;
	Gpio2Temp &= ~0x0FFF;

	mta[0] = m1;
	mta[1] = m2;
	mta[2] = m3;
	mta[3] = m4;
	mta[4] = m5;
	mta[5] = m6;

	for(i = 0;i < 6; i++){
		if(mta[i] == 0x8000)
			mta[i] = 0;
	}
	mta[1] = -mta[1];
	mta[3] = -mta[3];
	mta[5] = -mta[5];

	for(i = 0;i < 6; i++){
		if(mta[i] > 0){
			t_Duty[i] = (unsigned int)(~(mta[i]*2));
			Gpio2Temp |= 1<<i*2;
		}
		else if(mta[i] < 0){
			t_Duty[i] = (unsigned int)(~(-mta[i]*2));
			Gpio2Temp |= 2<<i*2;
		}
		else{
			t_Duty[i] = 0;
		}
	}

	LPC_TMR16B0->MR0 = t_Duty[0]&0x0000FFFF;
	LPC_TMR16B1->MR0 = t_Duty[1]&0x0000FFFF;

	LPC_TMR32B0->MR0 = t_Duty[2]&0x0000FFFF;
	LPC_TMR32B0->MR1 = t_Duty[3]&0x0000FFFF;

	LPC_TMR16B0->MR1 = t_Duty[4]&0x0000FFFF;
	LPC_TMR16B0->MR2 = t_Duty[5]&0x0000FFFF;

    LPC_GPIO2->DATA = Gpio2Temp;
}

void Timer32Init(void){

	//Timer Stop
	LPC_TMR32B0->TCR &= ~0x01;

	//	MAT0,MAT1 ,MAT2enable, MAT3 disable MAT2 is used Buzzer
	LPC_TMR32B0->PWMC = (0<<3|1<<2|1<<1|1<<0);

	//maximum value for the Prescale Counter. Divide Mainclk
	LPC_TMR32B0->PR = 0;

	//interrupt disable. TC clear MR3 matches TC reset enable
    LPC_TMR32B0->MCR = (1<<10 | 1<<9 | 1<<6);

    //interrupt flag clear
    LPC_TMR32B0->IR = 0;

    //Duty set
    /* LPC_TMR32B0->MR0 = 0x00008000;	//MTR3
     LPC_TMR32B0->MR1 = 0x00008000;	//MTR4*/

    LPC_TMR32B0->MR3 = 0x00010000;	//MR3 set cycle

    LPC_TMR32B0->MR0 = (~0x0000)&0x0000FFFF;			//MTR3
    LPC_TMR32B0->MR1 = (~0x0000)&0x0000FFFF;			//MTR4
    LPC_TMR32B0->MR2 = 0x00011000;	//buzzer

    //PWM cycle set

    //TimerStart
    LPC_TMR32B0->TCR |= 0x01;
}

void Timer16Init(void){

  /* Enable AHB clock to the CT16B0,CT16B1. */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<9)|(1<<8)|(1<<7));

  //Timer stop
  LPC_TMR16B0->TCR &= ~0x01;
  LPC_TMR16B1->TCR &= ~0x01;

  //set PWMmode
  LPC_TMR16B0->PWMC = (0<<3|1<<2|1<<1|1<<0);	//	MAT0,MAT1,MAT2 enable, MAT3 disable
  LPC_TMR16B1->PWMC = (0<<3|0<<2|0<<1|1<<0);	//	MAT0 enable, MAT1,MAT2,MAT3 disable

  //interrupt flag clear
  LPC_TMR16B0->IR = 0;
  LPC_TMR16B1->IR = 0;

  //interrupt
  LPC_TMR16B0->MCR = 0;
  LPC_TMR16B1->MCR = 0;

  //maximum value for the Prescale Counter. Divide Mainclk
  LPC_TMR16B0->PR = 0;
  LPC_TMR16B1->PR = 0;

  //Set Duty
  LPC_TMR16B0->MR0 = ~0x0000;	//MTR1
  LPC_TMR16B0->MR1 = ~0x0000;	//MTR5
  LPC_TMR16B0->MR2 = ~0x0000;	//MTR6
  LPC_TMR16B1->MR0 = ~0x0000;	//MTR2

  //Timer start
  LPC_TMR16B0->TCR |= 0x01;
  LPC_TMR16B1->TCR |= 0x01;

  return;
}


#define ADC_DONE		0x80000000
#define ADC_OVERRUN		0x40000000

void ADCInit(void){

	/* Disable Power down bit to the ADC block. */
  LPC_SYSCON->PDRUNCFG &= ~(0x1<<4);

  /* Enable AHB clock to the ADC. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);

  LPC_ADC->CR = ( 0x01 << 0 ) |  /* SEL=1,select channel 0~7 on ADC0 */

	((48000000/1000000 - 1) << 8) |  /* CLKDIV = Fpclk / 1000000 - 1 */

	( 0 << 16 ) | 		/* BURST = 0, no BURST, software controlled */
	( 0 << 17 ) |  		/* CLKS = 0, 11 clocks/10 bits */
	( 1 << 21 ) |  		/* PDN = 1, normal operation */
	( 0 << 22 ) |  		/* TEST1:0 = 00 */
	( 0 << 24 ) |  		/* START = 0 A/D conversion stops */
	( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */

  return;
}

unsigned short ADRead( uint8_t ch )
{

  uint32_t regVal,GDR;
  unsigned short ADC_Data;

	if ( ch > 7 )
		ch = 0;

  LPC_ADC->CR &= 0xFFFFFF00;
  LPC_ADC->CR |= (1 << 24) | (1 << ch);


  while ( 1 )
  {
	  switch(ch){
	  case 0:
			regVal = LPC_ADC->DR0;
	  break;
	  case 1:
			regVal = LPC_ADC->DR1;
	  break;
	  case 2:
			regVal = LPC_ADC->DR2;
	  break;
	  case 3:
			regVal = LPC_ADC->DR3;
	  break;
	  case 4:
			regVal = LPC_ADC->DR4;
	  break;
	  case 5:
			regVal = LPC_ADC->DR5;
	  break;
	  case 6:
			regVal = LPC_ADC->DR6;
	  break;
	  case 7:
			regVal = LPC_ADC->DR7;
	  break;
	  }
	  GDR = LPC_ADC->GDR;

	if ( GDR & ADC_DONE )
		{
	  break;
	}
  }

  if(ch == 1){
   unsigned short temp;
   temp = (unsigned short)(( regVal >> 6 ) & 0x3FF);
   temp = temp;
  }

  /* stop ADC now */
  LPC_ADC->CR &= 0xF8FFFFFF;

  if ( GDR & ADC_OVERRUN )	/* save data when it's not overrun, otherwise, return zero */
  {
	return ( 0 );
  }

  ADC_Data = (unsigned short)(( GDR >> 6 ) & 0x3FF);
  return ( ADC_Data );
}

void LED(unsigned char LedOn)
{
	GPIOSetValue(0, 7, ~LedOn&0x01);		//SetLED1
	GPIOSetValue(0, 3, (~LedOn>>1)&0x01);	//SetLED2
}

unsigned char getSW(){
	if(GPIOGetValueBit(SW_PORT, SW_BIT) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

//GAME_PAD ********************************************************//

//PIO3_0 PSPAD-CMD
//PIO3_1 PSPAD-SEL
//PIO3_2 PSPAD-CLK
//PIO3_3 PSPAD-DAT


//GAME_PAD
#define PS_SEL_raise	GPIOSetValue(3,1,1)
#define PS_SEL_fall		GPIOSetValue(3,1,0)

#define PS_CLK_raise	GPIOSetValue(3,2,1)
#define PS_CLK_fall		GPIOSetValue(3,2,0)

#define PS_CMD_raise	GPIOSetValue(3,0,1)
#define PS_CMD_fall		GPIOSetValue(3,0,0)

#define PS_DAT			GPIOGetValueBit(3,3)

static const uint8_t CMD_config_mode_enter[] =     {0x01,0x43,0x00,0x01, 0x00,0x00,0x00,0x00, 0x00};
static const uint8_t CMD_config_mode_exit[] =      {0x01,0x43,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00};
static const uint8_t CMD_config_mode_exit2[] =      {0x01,0x43,0x00,0x00, 0x5a,0x5a,0x5a,0x5a, 0x5a};
static const uint8_t CMD_set_mode_and_lock[] =     {0x01,0x44,0x00,0x01, 0x03,0x00,0x00,0x00, 0x00};
static const uint8_t CMD_query_model_and_mode[] =  {0x01,0x45,0x00,0x5a, 0x5a,0x5a,0x5a,0x5a, 0x5a};
static const uint8_t CMD_vibration_enable[] =      {0x01,0x4d,0x00,0x00, 0x01,0xff,0xff,0xff, 0xff};
static const uint8_t CMD_vibration_disnable[] =    {0x01,0x4d,0x00,0xff, 0xff,0xff,0xff,0xff, 0xff};
static const uint8_t CMD_query_DS2_analog_mode[] = {0x01,0x41,0x00,0x5a, 0x5a,0x5a,0x5a,0x5a, 0x5a};
static const uint8_t CMD_set_DS2_native_mode[] =   {0x01,0x4f,0x00,0xff, 0xff,0x03,0x00,0x00, 0x00};
static const uint8_t CMD_read_data[] =             {0x01,0x42,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00};
static const uint8_t CMD_read_data2[] =            {0x01,0x42,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
											0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00};


short PS_DATA[10];
//受信データ
static uint8_t PADRD[30];
#define PAD_WAIT 50

void PadWait(int cnt){
	int i;
	for(i = 0;i < cnt;i++){
		__NOP();
	}
}

uint8_t Ds2rw(const uint8_t *cmd){
	int i;
	uint8_t TempCmd;
	uint8_t data,clk_cnt = 1;

	data=0;
	PS_CLK_raise;
	PS_CMD_raise;
	TempCmd = *cmd;

	PadWait(PAD_WAIT);// for(j=0;j<PAD_WAIT;j++);	//PAD_WAIT

  for(i=0;i<8;i++){
    if( (TempCmd & 0x01) == 0) /* CMDセット */
        PS_CMD_fall;
    else
        PS_CMD_raise;

    TempCmd = TempCmd >> 1;
    PS_CLK_fall;

    PadWait(PAD_WAIT);// for(j=0;j<PAD_WAIT;j++);	//PAD_WAIT

	if(PS_DAT!=0)
    	data+=clk_cnt;
	PS_CLK_raise;

    clk_cnt*=2;

  }
  PS_CMD_raise;
  return data;
}

//各コマンド送受信
void Ds2Com(const uint8_t * cmd,unsigned char txLen){
	int i;
  PS_SEL_fall;

  PadWait(PAD_WAIT);//for(j=0;j<PAD_WAIT;j++);	//PAD_WAIT

  for(i = 0; i < txLen;i++){
	PadWait(PAD_WAIT*3);//for(j=0;j<PAD_WAIT*3;j++);	//PAD_WAIT
	PADRD[i]=Ds2rw(cmd+i);
  }
  PS_SEL_raise;
}

int PS_Set_AN()
{
//	int a;

//	a = 0;
	PadWait(PAD_WAIT*8);//for(i = 0 ; i < PAD_WAIT * 8 ; i++ );
	Ds2Com(CMD_config_mode_enter,sizeof(CMD_config_mode_enter));	//コンフィグモードへ
	PadWait(PAD_WAIT*8);//for(i = 0 ; i < PAD_WAIT * 8 ; i++ );
	Ds2Com(CMD_config_mode_enter,sizeof(CMD_config_mode_enter));	//コンフィグモードへ
	PadWait(PAD_WAIT*8);//for(i = 0 ; i < PAD_WAIT * 8 ; i++ );

	Ds2Com(CMD_query_model_and_mode,sizeof(CMD_query_model_and_mode));
	PadWait(PAD_WAIT*8);//for(i = 0 ; i < PAD_WAIT * 8 ; i++ );
	Ds2Com(CMD_query_model_and_mode,sizeof(CMD_query_model_and_mode));
	PadWait(PAD_WAIT*8);//for(i = 0 ; i < PAD_WAIT * 8 ; i++ );

		Ds2Com(CMD_set_mode_and_lock,sizeof(CMD_set_mode_and_lock));
		PadWait(PAD_WAIT*25);//for(i = 0 ; i < PAD_WAIT * 25 ; i++ );
		Ds2Com(CMD_set_mode_and_lock,sizeof(CMD_set_mode_and_lock));
		PadWait(PAD_WAIT*25);//for(i = 0 ; i < PAD_WAIT * 25 ; i++ );
		Ds2Com(CMD_config_mode_exit,sizeof(CMD_config_mode_exit));
		PadWait(PAD_WAIT*25);//for(i = 0 ; i < PAD_WAIT * 25 ; i++ );
		Ds2Com(CMD_config_mode_exit,sizeof(CMD_config_mode_exit));
		return 1;

	return 0;
}


void updatePAD()
{
//	static unsigned char flag;
	int i;

	Ds2Com(CMD_read_data,sizeof(CMD_read_data));

	if(PADRD[2] == 0x5a)
	{
		if(PADRD[1] == 0x73){
			PS_DATA[0] = ~PADRD[3];
			PS_DATA[1] = ~PADRD[4];
			for(i = 2 ; i < 6 ; i++ )
				PS_DATA[i] = (short)PADRD[3+i] - 128;
		}
		else{
			PS_DATA[0]=~PADRD[3];
			PS_DATA[1]=~PADRD[4];
			for(i=2;i<6;i++)
				PS_DATA[i] = 0;
//			flag = PS_Set_AN();
		}
	}
	else
	{
		for(i=0;i<2;i++)
			PS_DATA[i] = 0;
		for(i=2;i<6;i++)
			PS_DATA[i] = 0;
	}
}

short getPAD(uint8_t num){
	if(num <= PAD_AN_LY && num>= 0)
		return PS_DATA[num];
	else
		return 0;
}

void Wait(unsigned int msec){
	unsigned long cnt = (unsigned long)msec * MainCnt / 1000;

	while(cnt > 0){
		Sync();
		cnt --;
	}
}

//buzzer
void BuzzerSet(BYTE pitch , BYTE vol){
	iBuzzerPitch = 384 * (unsigned int)pitch;
}

void BuzzerStart(){
	if(!isBuzzer()){
		LPC_TMR32B0->MR2 += iBuzzerPitch;	//buzzer
		bBuzzerFlag = 1;
		LPC_TMR32B0->IR = 0;
		NVIC_EnableIRQ(TIMER_32_0_IRQn);
	}
}

void BuzzerStop(){
	LPC_TMR32B0->IR = 0;
    LPC_TMR32B0->MR2 = 0x00013000;	//buzzer
	bBuzzerFlag = 0;
}

BYTE isBuzzer(){
	return bBuzzerFlag;
}

void TIMER32_0_IRQHandler(void){

	//is Timer Reset[
	static int8_t outbitvalue = 0;
	uint32_t tempIR = LPC_TMR32B0->IR;

	if(bBuzzerFlag == 1){
		if(LPC_TMR32B0->MR2 < LPC_TMR32B0->MR3){
			outbitvalue = (outbitvalue+1) & 0x01;
			GPIOSetValue(1,8,(uint32_t)outbitvalue);
			LPC_TMR32B0->MR2 += iBuzzerPitch;
		}
		else if(LPC_TMR32B0->MR2 == LPC_TMR32B0->MR3){
			outbitvalue = (outbitvalue+1) & 0x01;
			GPIOSetValue(1,8,(uint32_t)outbitvalue);
			LPC_TMR32B0->MR2 = iBuzzerPitch;
		}
		else{
			LPC_TMR32B0->MR2 -= LPC_TMR32B0->MR3;
		}
	}
	else{
		NVIC_DisableIRQ(TIMER_32_0_IRQn);
	}
	LPC_TMR32B0->IR = tempIR;
}
long enc_A,enc_B;


void InitEncoder(){
	enc_A = 0;
	enc_B = 0;

	LPC_IOCON->PIO0_2 = 0x20; 	//  Hysteresis Enable , PullUp Disable ,PIO
	LPC_IOCON->PIO1_5 = 0x20; 	//  Hysteresis Enable , PullUp Disable ,PIO
	LPC_IOCON->PIO2_10 = 0x30;	//  Hysteresis Enable , PullDown Disable ,PIO
	LPC_IOCON->PIO2_11 = 0x30;	//  Hysteresis Enable , PullDown Enable ,PIO

	GPIOSetDir(0,2,0);	//set input port pin
	GPIOSetDir(1,5,0);	//set input port pin

	//Use MTR6 GPIO
	GPIOSetDir(2,10,0);	//set input port pin
	//Use MTR6 GPIO
	GPIOSetDir(2,11,0);	//set input port pin

	//Interrupt on pin PIOn_x is configured as edge	sensitive.
	LPC_GPIO0->IS = 0;
	LPC_GPIO1->IS = 0;
	LPC_GPIO2->IS = 0;

	//Both edges trigger an interrupt.
	LPC_GPIO0->IBE |= (1 << 2);
	LPC_GPIO1->IBE |= (1 << 5);
	LPC_GPIO2->IBE |= (1 << 10)|(1 << 11);

	LPC_GPIO0->IE |= (1 << 2);
	LPC_GPIO1->IE |= (1 << 5);
	LPC_GPIO2->IE |= (1 << 10)|(1 << 11);

	LPC_GPIO0->IC = LPC_GPIO0->MIS;
	LPC_GPIO1->IC = LPC_GPIO1->MIS;
	LPC_GPIO2->IC = LPC_GPIO2->MIS;

	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);

}

const signed char enctbl[16] = {
  00, //* 00 -> 00
  +1, //* 00 -> 01
  -1, //* 00 -> 10
  00, //* 00 -> 11
  -1, //* 01 -> 00
  00, //* 01 -> 01
  00, //* 01 -> 10
  +1, //* 01 -> 11
  +1, //* 10 -> 00
  00, //* 10 -> 01
  00, //* 10 -> 10
  -1, //* 10 -> 11
  00, //* 11 -> 00
  -1, //* 11 -> 01
  +1, //* 11 -> 10
  00, //* 11 -> 11
};



void ClearEncoder(){
	enc_B = 0;
	enc_A = 0;
}

void GetEncoder(long *A,long *B){
	*A = enc_A;
	*B = enc_B;
	return;
}
void IntEncoder(void){

	//	PIO0_2			ENC1A
	//	PIO1_5			ENC1B
	//	PIO2_10			M6DOUT/ENC2A
	//	PIO2_11	SCK		M6DOUT/ENC2B

	unsigned char A = 0;
	static unsigned char old_A = 0;
	unsigned char B = 0;
	static unsigned char old_B = 0;


	uint32_t tGPIO0_DATA,tGPIO1_DATA,tGPIO2_DATA;

	LPC_GPIO0->IC = LPC_GPIO0->MIS;
	tGPIO0_DATA = LPC_GPIO0->DATA;
	LPC_GPIO1->IC = LPC_GPIO1->MIS;
	tGPIO1_DATA = LPC_GPIO1->DATA;
	LPC_GPIO2->IC = LPC_GPIO2->MIS;
	tGPIO2_DATA = LPC_GPIO2->DATA;

	if((tGPIO0_DATA & (1<<2)) != 0){
		B += 1;
	}
	if((tGPIO1_DATA & (1<<5)) != 0){
		B += 2;
	}
	if((tGPIO2_DATA & (1<<10)) != 0){
		A += 1;
	}
	if((tGPIO2_DATA & (1<<11)) != 0){
		A += 2;
	}

	if(enctbl[(old_B<<2) | B] != 0){
		enc_B += enctbl[(old_B<<2) | B];
		old_B = B;
	}

	if(enctbl[(old_A<<2) | A] != 0){
		enc_A += enctbl[(old_A<<2) | A];
		old_A = A;
	}
}

void PIOINT0_IRQHandler(){
	IntEncoder();
}
void PIOINT1_IRQHandler(){
	IntEncoder();
}
void PIOINT2_IRQHandler(){
	IntEncoder();
}


