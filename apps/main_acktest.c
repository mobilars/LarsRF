/* 
* This file is licensed under BSD. It is originally copyright Texas Instruments, 
* but has been adapted by Lars Kristian Roland
*/

/*
* Press the button and the green LED should flash on both board. 
* This demo implements an ACK packet that goes back to the recipient. 
* (Both devices use the same address). 
*/

// Sorry... buggy uart code right now... Haven't had time to fix it yet. 
// Feel free to play with it. 

#include "../ti/include.h"
//#include "../hal/uart.h"
//#include "utils.c"

extern char paTable[];
extern char paTableLen;

char txBuffer[12];
char rxBuffer[12];
unsigned int i = 0;

void main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // 5ms delay to compensate for time to startup between MSP430 and CC1100/2500
  __delay_cycles(5000);
  
  RF_init();
    
  // Build packet
  txBuffer[0] = 11;                        // Packet length
  txBuffer[1] = 0x01;                     // Packet address
  txBuffer[2] = 0x00;
  txBuffer[3] = 0x32;
  txBuffer[4] = 0x33;
  txBuffer[5] = 0x34;
  txBuffer[6] = 0x35;
  txBuffer[7] = 0x36;
  txBuffer[8] = 0x37;
  txBuffer[9] = 0x38;
  txBuffer[10] = 0x39;
  txBuffer[11] = 0x40;
    
  //uartInit();
  
  //__bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled

}


// The ISR assumes the interrupt came from a pressed button
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
  
  // If Switch was pressed
  if(TI_CC_SW_PxIFG & TI_CC_SW1)
  {
    __delay_cycles(30000);                   // Switch debounce
    __delay_cycles(30000);                   // Switch debounce
    __delay_cycles(30000);                   // Switch debounce
    __delay_cycles(30000);                   // Switch debounce
    __delay_cycles(30000);                   // Switch debounce
    
    // Using printf isn't very efficient, although if you configure your compiler,
    // you can maybe use a tiny version of it. Also check out:
    // http://www.43oh.com/forum/viewtopic.php?f=10&t=1732&hilit=tiny+printf
    
    //uartprintf("TX PKT:%i\r\n", txBuffer[3]);// An example of what we want to show on serial
          
    RFSendPacket(txBuffer, 12);              // Send value over RF

  }
  //TI_CC_SW_PxIFG &= ~(TI_CC_SW1);           // Clr flag that caused int
  TI_CC_SW_PxIFG = 0x00;
}

// The ISR assumes the interrupt came from GDO0. GDO0 fires indicating that
// CCxxxx received a packet
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    // if GDO fired
  if(TI_CC_GDO0_PxIFG & TI_CC_GDO0_PIN)
  {
    char status[2];
    char len=11;                            // Len of pkt to be RXed (only addr
                                            // plus data; size byte not incl b/c
                                            // stripped away within RX function)
    if (RFReceivePacket(rxBuffer,&len,status))  
    {   
        // Fetch packet from CCxxxx
        //TI_CC_LED_PxOUT ^= rxBuffer[1];         // Toggle LEDs according to pkt data
           
        if (rxBuffer[1] == 0xFF) 
        {
          //puts("RX ACK\r\n");
        }
        else 
        {
          //__delay_cycles(500000);
          // Send ACK
          // Build packet
          txBuffer[0] = 3;                        // Packet length
          txBuffer[1] = 0x01;                     // Packet address
          txBuffer[2] = 0xFF;
          txBuffer[3] = 0x00;

          RFSendPacket(txBuffer, 4);              // Send value over RF
          //uartprintf("RX PKT:%i\r\n", rxBuffer[2]);// An example of what we want to show on serial
          //puts("TX ACK\r\n"); // Similar to printf, but printf probably takes longer
        }
        
     }
  }

  TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;      // After pkt RX, this flag is set.
}
