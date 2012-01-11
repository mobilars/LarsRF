/*
* Author and copyright: Lars Kristian Roland
* License: See README file distributed with this software. 
*/

#include "msp430g2553.h"
#include "spi_usci_CC1101_hal.h"
#include "various.h"

volatile unsigned char read_result[11];
volatile unsigned char txBuffer[11];
volatile unsigned char read_count;
volatile unsigned char target_count;

void SPI_init( void )
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  
  UCB0CTL1 |= UCSWRST;
  
  //P1OUT = BIT3;                     // Pull up for 1.3
  //P1DIR = BIT5;                     //
  P1SEL = BIT5 + BIT6 + BIT7;
  P1SEL2 = BIT5 + BIT6 + BIT7;
  //P1REN = BIT3;
    
  P2OUT |= BIT7;
  P2DIR |= BIT7;
  P2SEL = 0x00;
  
  UCB0CTL0 |= UCCKPH | UCMST | UCSYNC | UCMSB;
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x01; 
  UCB0BR1 = 0x00;                              //
  //UCB0MCTL = 0;                             // No modulation
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  //IE2 |= UCB0RXIE;                          // Enable USCI0 RX interrupt

  __delay_cycles(75);                 // Wait for slave to initialize

  //_BIS_SR(GIE); 

}

uint8 SPI_cmd( uint8 value )
{
  IFG2 &= ~UCB0RXIFG; // reset the RX flag 
  UCB0TXBUF = value;                      
  while(!(IFG2 & UCB0RXIFG));
  return UCB0RXBUF;
}

void SPI_strobe( uint8 strobe )
{
  CS_low; // Reset CS (active low)
  SPI_cmd( strobe | WRITE );
  CS_high; // Reset CS (active low)
}

uint8 SPI_read_status( uint8 addr )
{
  CS_low; // Reset CS (active low)
  SPI_cmd( addr | READ | BURST );
  uint8 retval = SPI_cmd( 0x00 ); 
  CS_high; // Reset CS (active low)
  return retval;
}

uint8 SPI_read_single( uint8 addr )
{

  CS_low; // Reset CS (active low)
  SPI_cmd( addr | READ );
  uint8 retval = SPI_cmd( 0x00 ); 
  CS_high; // Reset CS (active low)
  return retval;
  
}

uint8 SPI_write_single( uint8 addr, uint8 value )
{
  CS_low; // Reset CS (active low)
  SPI_cmd( addr | WRITE );
  uint8 retval = SPI_cmd( value ); 
  CS_high; // Reset CS (active low)
  return retval;
}

void SPI_write_burst(uint8 addr, uint8 *buffer, uint8 count)
{
	uint8 i;
	CS_low; 
	SPI_cmd( addr | WRITE | BURST );
	for(i=0;i<count;i++)
	{
	  SPI_cmd(buffer[i]);
          __delay_cycles(10); 
	}
	CS_high;
}

void SPI_read_burst(uint8 addr, uint8 *buffer, uint8 count)
{
	uint8 i;
	CS_low; 

	SPI_cmd( addr | READ | BURST );
	for(i=0;i<count;i++)
	{
	  buffer[i] = SPI_cmd( 0x00 );
	}
	CS_high;
}

/*

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
{
  // Not used. 
}                                           

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIA0TX_ISR(void)
{
  // Not used. 
}

*/