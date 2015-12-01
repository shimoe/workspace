
#include "LPC13xx.h"			/* LPC13xx Peripheral Registers */
#include "gpio.h"


/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			port num, bit position, direction (1 out, 0 input)
** Returned value:		None
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
  switch ( portNum )
  {
	case PORT0:
	  if ( !(LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO0->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO0->DIR &= ~(0x1<<bitPosi);
	break;
 	case PORT1:
	  if ( !(LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO1->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO1->DIR &= ~(0x1<<bitPosi);
	break;
	case PORT2:
	  if ( !(LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO2->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO2->DIR &= ~(0x1<<bitPosi);
	break;
	case PORT3:
	  if ( !(LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 1) )
		LPC_GPIO3->DIR |= (0x1<<bitPosi);
	  else if ( (LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 0) )
		LPC_GPIO3->DIR &= ~(0x1<<bitPosi);
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOSetValue
**
** Descriptions:		Set/clear a bitvalue in a specific bit position
**						in GPIO portX(X is the port number.)
**
** parameters:			port num, bit position, bit value
** Returned value:		None
**
*****************************************************************************/
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
	// Check bitVal is a binary value - 0 or 1
	if (bitVal <2 )
	{
  switch ( portNum )
	  {
		case PORT0:
				 LPC_GPIO0->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
		break;
		case PORT1:
				 LPC_GPIO1->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
		break;
		case PORT2:
				 LPC_GPIO2->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
		break;
		case PORT3:
				 LPC_GPIO3->MASKED_ACCESS[(1<<bitPosi)] = (bitVal<<bitPosi);
		break;
		default:
		  break;
	  }
	}
  return;
}
/*****************************************************************************
** Function name:		GPIOGetValueBit
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
char GPIOGetValueBit( uint32_t portNum, uint32_t bitPosi )
{
	  switch ( portNum )
	  {
		case PORT0:
			return LPC_GPIO0->MASKED_ACCESS[(1<<bitPosi)];
		break;
		case PORT1:
			return LPC_GPIO1->MASKED_ACCESS[(1<<bitPosi)];
		break;
		case PORT2:
			return LPC_GPIO2->MASKED_ACCESS[(1<<bitPosi)];
		break;
		case PORT3:
			return LPC_GPIO3->MASKED_ACCESS[(1<<bitPosi)];
		break;
		default:
		  break;
	  }
  return -1;
}



/*****************************************************************************
 ** Function name:		GPIOGetValue
 ** parameters:			port num
 ** Returned value:		None
 **
 *****************************************************************************/
uint32_t GPIOGetValue( uint32_t portNum)
 {
 	  switch ( portNum )
 	  {
 		case PORT0:
 			return *(uint32_t*)LPC_GPIO0;//->MASKED_ACCESS[(1<<bitPosi)];
 		break;
 		case PORT1:
 			return *(uint32_t*)LPC_GPIO0;//->MASKED_ACCESS[(1<<bitPosi)];
 		break;
 		case PORT2:
 			return *(uint32_t*)LPC_GPIO0;//->MASKED_ACCESS[(1<<bitPosi)];
 		break;
 		case PORT3:
 			return *(uint32_t*)LPC_GPIO0;//->MASKED_ACCESS[(1<<bitPosi)];
 		break;
 		default:
 		  break;
 	  }
   return 0;
 }

/*****************************************************************************
** Function name:		GPIOSetInterrupt
**
** Descriptions:		Set interrupt sense, event, etc.
**						edge or level, 0 is edge, 1 is level
**						single or double edge, 0 is single, 1 is double 
**						active high or low, etc.
**
** parameters:			port num, bit position, sense, single/doube, polarity
** Returned value:		None
** 
*****************************************************************************/
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
			uint32_t single, uint32_t event )
{
  switch ( portNum )
  {
	case PORT0:
	  if ( sense == 0 )
	  {
		LPC_GPIO0->IS &= ~(0x1<<bitPosi);
		/* single or double only applies when sense is 0(edge trigger). */
		if ( single == 0 )
		  LPC_GPIO0->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO0->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO0->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO0->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO0->IEV |= (0x1<<bitPosi);
	break;
 	case PORT1:
	  if ( sense == 0 )
	  {
		LPC_GPIO1->IS &= ~(0x1<<bitPosi);
		/* single or double only applies when sense is 0(edge trigger). */
		if ( single == 0 )
		  LPC_GPIO1->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO1->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO1->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO1->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO1->IEV |= (0x1<<bitPosi);  
	break;
	case PORT2:
	  if ( sense == 0 )
	  {
		LPC_GPIO2->IS &= ~(0x1<<bitPosi);
		/* single or double only applies when sense is 0(edge trigger). */
		if ( single == 0 )
		  LPC_GPIO2->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO2->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO2->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO2->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO2->IEV |= (0x1<<bitPosi);  
	break;
	case PORT3:
	  if ( sense == 0 )
	  {
		LPC_GPIO3->IS &= ~(0x1<<bitPosi);
		/* single or double only applies when sense is 0(edge trigger). */
		if ( single == 0 )
		  LPC_GPIO3->IBE &= ~(0x1<<bitPosi);
		else
		  LPC_GPIO3->IBE |= (0x1<<bitPosi);
	  }
	  else
	  	LPC_GPIO3->IS |= (0x1<<bitPosi);
	  if ( event == 0 )
		LPC_GPIO3->IEV &= ~(0x1<<bitPosi);
	  else
		LPC_GPIO3->IEV |= (0x1<<bitPosi);	  
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOIntEnable
**
** Descriptions:		Enable Interrupt Mask for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
** 
*****************************************************************************/
void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
	case PORT0:
	  LPC_GPIO0->IE |= (0x1<<bitPosi); 
	break;
 	case PORT1:
	  LPC_GPIO1->IE |= (0x1<<bitPosi);	
	break;
	case PORT2:
	  LPC_GPIO2->IE |= (0x1<<bitPosi);	    
	break;
	case PORT3:
	  LPC_GPIO3->IE |= (0x1<<bitPosi);	    
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOIntDisable
**
** Descriptions:		Disable Interrupt Mask for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
** 
*****************************************************************************/
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
	case PORT0:
	  LPC_GPIO0->IE &= ~(0x1<<bitPosi); 
	break;
 	case PORT1:
	  LPC_GPIO1->IE &= ~(0x1<<bitPosi);	
	break;
	case PORT2:
	  LPC_GPIO2->IE &= ~(0x1<<bitPosi);	    
	break;
	case PORT3:
	  LPC_GPIO3->IE &= ~(0x1<<bitPosi);	    
	break;
	default:
	  break;
  }
  return;
}

/*****************************************************************************
** Function name:		GPIOIntStatus
**
** Descriptions:		Get Interrupt status for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
** 
*****************************************************************************/
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi )
{
  uint32_t regVal = 0;

  switch ( portNum )
  {
	case PORT0:
	  if ( LPC_GPIO0->MIS & (0x1<<bitPosi) )
		regVal = 1;
	break;
 	case PORT1:
	  if ( LPC_GPIO1->MIS & (0x1<<bitPosi) )
		regVal = 1;	
	break;
	case PORT2:
	  if ( LPC_GPIO2->MIS & (0x1<<bitPosi) )
		regVal = 1;		    
	break;
	case PORT3:
	  if ( LPC_GPIO3->MIS & (0x1<<bitPosi) )
		regVal = 1;		    
	break;
	default:
	  break;
  }
  return ( regVal );
}

/*****************************************************************************
** Function name:		GPIOIntClear
**
** Descriptions:		Clear Interrupt for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
** 
*****************************************************************************/
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi )
{
  switch ( portNum )
  {
	case PORT0:
	  LPC_GPIO0->IC |= (0x1<<bitPosi); 
	break;
 	case PORT1:
	  LPC_GPIO1->IC |= (0x1<<bitPosi);	
	break;
	case PORT2:
	  LPC_GPIO2->IC |= (0x1<<bitPosi);	    
	break;
	case PORT3:
	  LPC_GPIO3->IC |= (0x1<<bitPosi);	    
	break;
	default:
	  break;
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
