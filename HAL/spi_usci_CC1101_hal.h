/*
* Author and copyright: Lars Kristian Roland
* License: See README file distributed with this software. 
*/

#include "various.h"

#define WRITE	            0x00
#define BURST               0x40
#define READ                0x80

#define CS_low P2OUT &= ~BIT7;
#define CS_high P2OUT |= BIT7;

uint8 SPI_read_single( uint8 addr );
uint8 SPI_write_single( uint8 addr, uint8 value );

void SPI_write_burst(uint8 addr, uint8 *buffer, uint8 count);
void SPI_read_burst(uint8 addr, uint8 *buffer, uint8 count);

void SPI_strobe( uint8 strobe );
uint8 SPI_read_status( uint8 addr );
void SPI_init( void );
//void SPI_write( unsigned char address, unsigned char * rfBuffer, unsigned char count );
//void SPI_read( unsigned char addr, unsigned char * rfBuffer, unsigned char count );