
#include "LPC13xx.h"			/* LPC134x Peripheral Registers */
#include "type.h"
#include "ixbus.h"

volatile uint32_t I2CMasterState = I2C_IDLE;
volatile uint32_t I2CSlaveState = I2C_IDLE;

volatile uint32_t I2CMode;


volatile uint32_t I2CCount = 0;
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;

volatile uint8_t I2CMasterBuffer[IIC_BUFSIZE];
uint8_t I2CSlaveBuffer[IIC_BUFSIZE];

static uint8_t StatB[80];
static uint8_t statCnt = 0;
static uint8_t SendCnt = 0;
static uint8_t RcvCnt = 0;

uint8_t I2CEndFlag = 0;

uint16_t adtemp[8];

//addr = 0x90,0x92,0x94,0x96
unsigned char Get_IX008(unsigned char Addr,unsigned int *retdata){
	int i,j;
//	uint16_t temp;
	const uint8_t commandByte[] = {0x8C,0xCC,0x9C,0xDC,0xAC,0xEC,0xBC,0xFC};

	for(j = 0;j < 8;j++ ){
	  for ( i = 0; i < IIC_BUFSIZE; i++ )
	  {
		I2CSlaveBuffer[i] = 0x00;
	  }

	  /* Write SLA(W), address, SLA(R), and read one byte back. */

	  I2CWriteLength = 2;
	  I2CReadLength = 2;
	  I2CMasterBuffer[0] = Addr;
	  I2CMasterBuffer[1] = commandByte[j];
	  I2CMasterBuffer[2] = Addr+1;

	  I2CEndFlag = 0;
	  I2CEngine();

	  //for ( i = 0; i < 0x200; i++ );
	  i = 0;
	  while(I2CEndFlag == 0){
		  i ++;
		  if(i > 200)
			  return 0;
	  }
	  retdata[j] = ((uint16_t)(I2CSlaveBuffer[0])<<8)+ I2CSlaveBuffer[1];
	}
	  return 1;
}

void I2C_IRQHandler(void) 
{
  uint8_t StatValue;
  static uint8_t cnt;

  /* this handler deals with master read and master write only */
  StatValue = LPC_I2C->STAT;
  if(StatValue == 0x08){
	  statCnt++;
	  for(;cnt != 0;cnt --)
		  StatB[cnt] = 0;
	  cnt = 0;
	  SendCnt = 0;
	  RcvCnt = 0;
  }
  StatB[cnt] = StatValue;
  cnt++;
  switch ( StatValue )
  {
	case 0x08:			/* A Start condition is issued. */
	WrIndex = 0;
	LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
	SendCnt++;
	LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_STARTED;

	break;
	
	case 0x10:			/* A repeated started is issued */
	RdIndex = 0;
	/* Send SLA with R bit set, */
	LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
	SendCnt++;
	LPC_I2C->CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
	I2CMasterState = I2C_RESTARTED;
	break;
	
	case 0x18:			/* Regardless, it's a ACK */
	if ( I2CMasterState == I2C_STARTED )
	{
	  LPC_I2C->DAT = I2CMasterBuffer[WrIndex++];
		SendCnt++;
	  I2CMasterState = DATA_ACK;
	}
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
	case 0x30:
	if ( WrIndex < I2CWriteLength )
	{   
	  LPC_I2C->DAT = I2CMasterBuffer[WrIndex++]; /* this should be the last one */
		SendCnt++;
	  I2CMasterState = DATA_ACK;
	}
	else
	{
	  if ( I2CReadLength != 0 )
	  {
		LPC_I2C->CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
		I2CMasterState = I2C_REPEATED_START;
	  }
	  else
	  {
		I2CMasterState = DATA_NACK;
		LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */
	  }
	}
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	break;
	
	case 0x40:	/* Master Receive, SLA_R has been sent */

	 LPC_I2C->CONSET = I2CONSET_AA;	/* assert ACK after data is received */
	 LPC_I2C->CONCLR = (I2CONCLR_SIC);
	break;
	
	case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
	I2CSlaveBuffer[RdIndex] = LPC_I2C->DAT;
	RdIndex++;
	RcvCnt ++;

	  LPC_I2C->CONCLR = (I2CONCLR_AAC);	// assert NACK on last byte

	  LPC_I2C->CONCLR = I2CONCLR_SIC;

	break;
	
	case 0x58:
	I2CSlaveBuffer[RdIndex] = LPC_I2C->DAT;
	I2CMasterState = DATA_NACK;

	LPC_I2C->CONSET = (I2CONSET_STO);	/* Set Stop flag */
	LPC_I2C->CONCLR = (I2CONCLR_SIC);	/* Clear SI flag */

	I2CEndFlag = 1;

	break;

	case 0x20:		/* regardless, it's a NACK */
	case 0x48:
	LPC_I2C->CONCLR = I2CONCLR_SIC;
	I2CMasterState = DATA_NACK;
	break;
	
	case 0x38:		/* Arbitration lost, in this example, we don't
					deal with multiple master situation */
	default:
	LPC_I2C->CONCLR = I2CONCLR_SIC;	
	break;
  }
  return;
}

uint32_t I2CStart( void )
{
  uint32_t timeout = 0;
  uint32_t retVal = FALSE;
 
  /*--- Issue a start condition ---*/
  LPC_I2C->CONSET = I2CONSET_STA;	/* Set Start flag */
    
  /*--- Wait until START transmitted ---*/
  while( 1 )
  {
	if ( I2CMasterState == I2C_STARTED )
	{
	  retVal = TRUE;
	  break;	
	}
	if ( timeout >= MAX_TIMEOUT )
	{
	  retVal = FALSE;
	  break;
	}
	timeout++;
  }
  return( retVal );
}

uint32_t I2CStop( void )
{
  LPC_I2C->CONSET = I2CONSET_STO;      /* Set Stop flag */ 
  LPC_I2C->CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
            
  /*--- Wait for STOP detected ---*/
  while( LPC_I2C->CONSET & I2CONSET_STO );
  return TRUE;
}

uint32_t I2C_init()
{
#if 1
  LPC_SYSCON->PRESETCTRL |= (0x1<<1);
#else
  LPC_SYSCON->PRESETCTRL |= (0x1<<0);
#endif
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);
  LPC_IOCON->PIO0_4 &= ~0x3F;	/*  I2C I/O config */
  LPC_IOCON->PIO0_4 |= 0x01;		/* I2C SCL Standard mode/ Fast-mode I2C*/
  LPC_IOCON->PIO0_5 &= ~0x3F;	
  LPC_IOCON->PIO0_5 |= 0x01;		/* I2C SDA Standard mode/ Fast-mode I2C*/

  /*--- Clear flags ---*/
  LPC_I2C->CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
#if FAST_MODE_PLUS
  LPC_IOCON->PIO0_4 |= (0x1<<9);
  LPC_IOCON->PIO0_5 |= (0x1<<9);
  LPC_I2C->SCLL   = I2SCLL_HS_SCLL;
  LPC_I2C->SCLH   = I2SCLH_HS_SCLH;
#else
  LPC_I2C->SCLL   = I2SCLL_SCLL;
  LPC_I2C->SCLH   = I2SCLH_SCLH;
#endif


  /* Enable the I2C Interrupt */
  NVIC_EnableIRQ(I2C_IRQn);

  LPC_I2C->CONSET = I2CONSET_I2EN;
  return( TRUE );
}

uint32_t I2CEngine( void ) 
{
  I2CMasterState = I2C_IDLE;
  RdIndex = 0;
  WrIndex = 0;
  if ( I2CStart() != TRUE )
  {
	I2CStop();
	return ( FALSE );
  }

  while ( 1 )
  {
	if ( I2CMasterState == DATA_NACK )
	{
	  I2CStop();
	  break;
	}
  }    
  return ( TRUE );      
}


