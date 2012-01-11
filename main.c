
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
  P1REN = BIT3;
  P1OUT |= BIT3;
  
  blink();
  blink();
  blink();
  
  SPI_init();
        
  RF_reset();
  __delay_cycles(100); 
  RF_SIDLE;
  __delay_cycles(100); 
  RF_init();
  __delay_cycles(100); 
  
  P1DIR = BIT4;            // P1.4 set as output

  if ((P1IN & BIT3) == 0) {
    send_packets();
  }
  else {
    receive_packets();
  }
  //check_RSSI();
}

void check_RSSI()
{
  RF_strobe( SRX );
  P1OUT |= BIT4;
  
  while (1) {
    uint8 rssi = RF_RSSI();
    if (rssi > 0xC0) {
      P1OUT &= ~BIT4;
      __delay_cycles(10000);
    }
    else {
      P1OUT |= BIT4;
    }
  }
}


void receive_packets()
{
  uint8 buffer[20];
  P1OUT |= BIT4;
  
  while (1) {
      uint8 length = RF_receive_packet(buffer);
      if (length > 0) {
          blink();
          blink();
          blink();
          __delay_cycles(10000);
      }
      else {
        //P1OUT |= BIT4;
      }
  }

}


void send_packets()
{
  uint8 buffer[20] = {1,2,3,4,5,6,7,8,10,11};
  
  while (1) {
    if ((P1IN & BIT3) == 0) {
      RF_send_packet(buffer, 10);
      blink();
    }
  }

}

void blink()
{

      P1OUT &= ~BIT4; // on (LED is on when port is 0)
      __delay_cycles(10000); 
      P1OUT |= BIT4; // off
      __delay_cycles(10000); 

}