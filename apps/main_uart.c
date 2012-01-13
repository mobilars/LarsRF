/* 
* BSD License. Copyright 2012 Lars Kristian Roland
*/

#include <msp430.h>
#include "../hal/uart.h"

void main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  P1REN |= BIT3;               // Enable Pull up resistor
  P1OUT |= BIT3;               // Enable pull up resistor
  P1IES |= BIT3;               // Int on falling edge
  P1IFG &= ~(BIT3);           // Clr flags
  P1IE |= BIT3;                // Activate interrupt enables

  uartInit();
  __bis_SR_register(LPM0_bits + GIE);
}


// The ISR assumes the interrupt came from a pressed button
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
  // If Switch was pressed
  if(P1IFG & BIT3)
  {
    uartWriteString("Hello... \r\n");
  }
  
  P1IFG &= ~(BIT3);
}