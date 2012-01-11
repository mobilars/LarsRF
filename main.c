/*
* Author and copyright 2011: Lars Kristian Roland
* LarsRF by Lars Kristian Roland is licensed under a Creative Commons 
* Attribution-ShareAlike 3.0 Unported License.
* Based on a work at github.com.
* Permissions beyond the scope of this license may be available at http://lars.roland.bz/.
*/

/*
Hardware setup for this example:

Put an LED with a 1Kohm resistor in series between VCC and P1.4. VCC is positive, 
and the LED will light up when the P1.4 is low. 

If you press the push-button, the chip will transmit. When it transmits, it
will also flash the LED you installed above. 

While not pressing the push-button, the chip will be trying to receive. On
succesful RX, it will flash the LED you installed above. 

This code is intended for msp430g2553 (or other USCI chips). The library won't work
with the cheaper USI-based chips (yet). 

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
  P1REN = BIT3;             // Pullup for the button
  P1OUT |= BIT3;            // Pullup for the button
  
  blink();
  blink();
  blink();
  
  SPI_init();
        
  RF_reset();
  RF_SIDLE;
  RF_init();

  while (1) {
    if ((P1IN & BIT3) == 0) {
      send_packets();
    }
    else {
      receive_packets();
    }
  }
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
  uint8 buffer[20];
  P1OUT |= BIT4;
  
  uint8 length = RF_receive_packet(buffer);
  if (length > 0) {
      blink();
      blink();
  }
  else {
    // Do nothing. 
  }

}


void send_packets()
{
  uint8 buffer[20] = {1,2,3,4,5,6,7,8,10,11};
  
    if ((P1IN & BIT3) == 0) {
      RF_send_packet(buffer, 10);
      blink();
    }

}

void blink()
{
      P1OUT &= ~BIT4; // on (LED is on when port is 0)
      __delay_cycles(10000); 
      P1OUT |= BIT4; // off
      __delay_cycles(10000); 
}