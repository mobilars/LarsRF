/*
* Author and copyright 2011: Lars Kristian Roland
* LarsRF by Lars Kristian Roland is licensed under a Creative Commons 
* Attribution-ShareAlike 3.0 Unported License.
* Based on a work at github.com.
* Permissions beyond the scope of this license may be available at http://lars.roland.bz/.
*/

/*

This is not yet a working example. Leave it out of your compilation. 

*/

//#include <msp430g2553.h>
#include <msp430.h>
#include "HAL/commands.h"
#include "HAL/spi_usci_CC1101_hal.h"
#include "HAL/rf_CC1101_hal.h"

void check_RSSI();
void send_packets();
void receive_packets();
void blink();

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= BIT4;            // P1.4 set as output
  P1DIR |= BIT1;            // P1.1 set as output
  P1REN = BIT3;             // Pullup for the button
  P1OUT |= BIT3;            // Pullup for the button
  P1OUT |= BIT1;
  P2DIR |= BIT2;
  
  blink();
  blink();
  blink();
  
  SPI_init();
        
  RF_reset();
  RF_SIDLE;
  RF_init_slow();

  P1IE |= BIT3;                             // P1.4 interrupt enabled
  P1IES |= BIT3;                            // P1.4 Hi/lo edge
  P1IFG &= ~BIT3;                           // P1.4 IFG cleared

  _BIS_SR(GIE);                 // Enter LPM4 w/interrupt
  
  while (1);
  /*
  while (1) {
    if ((P1IN & BIT3) == 0) {
      send_packets();
    }
    else {
      receive_packets();
    }
  }*/
}

void check_RSSI()
{
  RF_strobe( SRX );
  
  while (1) {
    uint8 rssi = RF_RSSI();
    if (rssi > 0xC0) {
      P1OUT &= ~BIT4;
    }
    else {
      P1OUT |= BIT4;
    }
  }
}


void receive_packets()
{
  SPI_strobe(SFTX);
  SPI_strobe(SFRX);
  
  uint8 buffer[20];
  uint8 ackBuffer[10] = {21,22,23,24,25,26,27,28,30,31};
  P1OUT |= BIT4;
  
  uint8 length = RF_receive_packet_ack(buffer, ackBuffer, 2);
  if (length > 0) {
      blink();
  }
  else {
    // Do nothing. 
  }

}


void send_packets()
{
  //SPI_strobe(SFTX);
  //SPI_strobe(SFRX);
  
  uint8 buffer[20] = {1,2,3,4,5,6,7,8,10,11};
  
    if ((P1IN & BIT3) == 0) {
      uint8 length = RF_send_packet_ack(buffer, 8);
      if (length > 0) {
        blink();
      }
    }
    
    // Need to add some delay... Dont' send too often. It's low baud rate
    //__delay_cycles(1000000);
}

void blink()
{
      P1OUT &= ~BIT4; // on (LED is on when port is 0)
      __delay_cycles(10000); 
      P1OUT |= BIT4; // off
      __delay_cycles(10000); 
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   P1IFG &= ~BIT3;
  send_packets();
}