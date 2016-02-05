
#ifndef __I2XBUS_H
#define __I2XBUS_H

#define FAST_MODE_PLUS	0

#define IIC_BUFSIZE		6
#define MAX_TIMEOUT		0x00FFFFFF

#define I2CMASTER		0x01
#define I2CSLAVE		0x02

//#define READ_WRITE		0x01

#define RD_BIT			0x01

#define I2C_IDLE			0
#define I2C_STARTED			1
#define I2C_RESTARTED		2
#define I2C_REPEATED_START	3
#define DATA_ACK			4
#define DATA_NACK			5

#define I2CONSET_I2EN		0x00000040  /* I2C Control Set Register */
#define I2CONSET_AA			0x00000004
#define I2CONSET_SI			0x00000008
#define I2CONSET_STO		0x00000010
#define I2CONSET_STA		0x00000020

#define I2CONCLR_AAC		0x00000004  /* I2C Control clear Register */
#define I2CONCLR_SIC		0x00000008
#define I2CONCLR_STAC		0x00000020
#define I2CONCLR_I2ENC		0x00000040

#define I2DAT_I2C			0x00000000  /* I2C Data Reg */
#define I2ADR_I2C			0x00000000  /* I2C Slave Address Reg */

#define I2SCLH_SCLH			0x00000120  /* I2C SCL Duty Cycle High Reg */
#define I2SCLL_SCLL			0x00000120  /* I2C SCL Duty Cycle Low Reg */

#define I2SCLH_HS_SCLH		0x00000020  /* Fast Plus I2C SCL Duty Cycle High Reg */
#define I2SCLL_HS_SCLL		0x00000020  /* Fast Plus I2C SCL Duty Cycle Low Reg */


extern void I2C_IRQHandler( void );
extern uint32_t I2C_init();
extern uint32_t I2CStart( void );
extern uint32_t I2CStop( void );
extern uint32_t I2CEngine( void );

uint8_t* GetIXAN();
//addr = 0x90,0x92,0x94,0x96
unsigned char Get_IX008(unsigned char Addr,unsigned int *retdata);

#endif

