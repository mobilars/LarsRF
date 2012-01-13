/* 
* This file is licensed under BSD. It is originally copyright Texas Instruments, 
* but has been adapted by Lars Kristian Roland
*/

/*
* Put an LED between P1.4 and GND (or VCC). Press the button on the other board,
* and your LED should turn on and off. This demo implements an ACK packet that
* goes back to the recipient. (Both devices use the same address). 
*/

#include "../ti/include.h"
#include "../hal/uart.h"

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
  
  uartInit();
  
  TI_CC_SPISetup();                         // Initialize SPI port

  TI_CC_PowerupResetCCxxxx();               // Reset CCxxxx
  writeRFSettings();                        // Write RF settings to config reg
  TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen);//Write PATABLE

  // Configure ports -- switch inputs, LEDs, GDO0 to RX packet info from CCxxxx
  TI_CC_SW_PxREN |= TI_CC_SW1;               // Enable Pull up resistor
  TI_CC_SW_PxOUT |= TI_CC_SW1;               // Enable pull up resistor
  TI_CC_SW_PxIES |= TI_CC_SW1;               // Int on falling edge
  TI_CC_SW_PxIFG &= ~(TI_CC_SW1);           // Clr flags
  TI_CC_SW_PxIE |= TI_CC_SW1;                // Activate interrupt enables
  TI_CC_LED_PxOUT &= ~(TI_CC_LED1); // Outputs = 0
  TI_CC_LED_PxDIR |= TI_CC_LED1;// LED Direction to Outputs

  TI_CC_GDO0_PxIES |= TI_CC_GDO0_PIN;       // Int on falling edge (end of pkt)
  TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;      // Clear flag
  TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN;        // Enable int on end of packet

  TI_CC_SPIStrobe(TI_CCxxx0_SRX);           // Initialize CCxxxx in RX mode.
                                            // When a pkt is received, it will
                                            // signal on GDO0 and wake CPU

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
    uartWriteString("TX \r\n");
  
    // Build packet
    txBuffer[0] = 11;                        // Packet length
    txBuffer[1] = 0x01;                     // Packet address
    txBuffer[2] = TI_CC_LED1;
    txBuffer[3] = 0x32;
    txBuffer[4] = 0x33;
    txBuffer[5] = 0x34;
    txBuffer[6] = 0x35;
    txBuffer[7] = 0x36;
    txBuffer[8] = 0x37;
    txBuffer[9] = 0x38;
    txBuffer[10] = 0x39;
    txBuffer[11] = 0x40;
    RFSendPacket(txBuffer, 12);              // Send value over RF
    __delay_cycles(5000);                   // Switch debounce
  }
  TI_CC_SW_PxIFG &= ~(TI_CC_SW1);           // Clr flag that caused int
}

// The ISR assumes the interrupt came from GDO0. GDO0 fires indicating that
// CCxxxx received a packet
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    // if GDO fired
  if(TI_CC_GDO0_PxIFG & TI_CC_GDO0_PIN)
  {
    char len=11;                            // Len of pkt to be RXed (only addr
                                            // plus data; size byte not incl b/c
                                            // stripped away within RX function)
    if (RFReceivePacket(rxBuffer,&len))     // Fetch packet from CCxxxx
    TI_CC_LED_PxOUT ^= rxBuffer[1];         // Toggle LEDs according to pkt data
    
    if (rxBuffer[2] != 0x00) {
      
      uartWriteString("RX \r\n");
      
      __delay_cycles(500000);
      // Send ACK
      // Build packet
      txBuffer[0] = 11;                        // Packet length
      txBuffer[1] = 0x01;                     // Packet address
      txBuffer[2] = 0xFF;
      txBuffer[3] = 0x00;
      RFSendPacket(txBuffer, 12);              // Send value over RF
      
      uartWriteString("TX \r\n");
    }
  }

  TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;      // After pkt RX, this flag is set.
}
