// Nathan Zimmerman
// Hardware_UART
// 1/7/12

// Based on Nathan Zimmerman's code at http://www.43oh.com/forum/viewtopic.php?f=10&p=15345#p15345
// Adapted by Lars Roland

//#include <msp430g2553.h>
#include <msp430.h>
#include "uart.h"

void uartInit(void)
{
   
    BCSCTL1 = CALBC1_1MHZ;            // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ;   
    
    ////////////////USCI setup////////////////
    
    P1SEL = BIT1 + BIT2;            // Set P1.1 to RXD and P1.2 to TXD
    P1SEL2 = BIT1 + BIT2;            //
    UCA0CTL1 |= UCSSEL_2;            // Have USCI use SMCLK AKA 1MHz main CLK
    UCA0BR0 = 104;                  // Baud: 9600, N= CLK/Baud, N= 10^6 / 9600
    UCA0BR1 = 0;                  // Set upper half of baud select to 0 
    UCA0MCTL = UCBRS_1;               // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;             // Start USCI
 
} 

void putc(unsigned char c)
{
    UCA0TXBUF = c; // write c to TX buffer
    //__delay_cycles(10000); //transmission delay
    while(!(IFG2&UCA0TXIFG));
    return;
}

void puts(unsigned char *tx_message)
{

    unsigned int i=0; //Define end of string loop int
    unsigned char *message; // message variable
    unsigned int message_num; // define ascii int version variable
    message = tx_message; // move tx_message into message
    
    while(1)
    {
    
    if(message[i]==0) // If end of input string is reached, break loop. 
    {
       break;
    }
    
    message_num = (int)message[i]; //Cast string char into a int variable
    UCA0TXBUF = message_num; // write INT to TX buffer
    i++; // increase string index
    __delay_cycles(10000); //transmission delay
    if(i>50) //prevent infinite transmit
    {
      //P1OUT |= Error; 
      break;   
    }
       
} 

   
} 
