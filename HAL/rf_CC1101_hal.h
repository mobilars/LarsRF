/*
* Author and copyright 2011: Lars Kristian Roland
* LarsRF by Lars Kristian Roland is licensed under a Creative Commons 
* Attribution-ShareAlike 3.0 Unported License.
* Based on a work at github.com.
* Permissions beyond the scope of this license may be available at http://lars.roland.bz/.
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

uint8 RF_receive_packet_ack(uint8 *rxBuffer, uint8 *ackBuffer, uint8 acklength);
uint8 RF_send_packet_ack(uint8 *txBuffer, uint8 length);