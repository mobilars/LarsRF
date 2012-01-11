/*
* Author and copyright: Lars Kristian Roland
* License: See README file distributed with this software. 
*/

#include "various.h"

#define RF_SRES RF_strobe( SRES )
#define RF_SIDLE RF_strobe( SIDLE )
#define RF_STX RF_strobe( STX )
#define RF_SRX RF_strobe( SRX )

void RF_strobe( uint8 strobe );
uint8 RF_read_reg( uint8 addr );
void RF_write_reg( uint8 addr, uint8 value);
uint8 RF_read_status( uint8 addr );

void RF_reset();
uint8 RF_RSSI();
void RF_init();

void RF_send_packet(uint8 *txBuffer, uint8 length);
uint8 RF_receive_packet(uint8 *rxBuffer);