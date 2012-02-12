/* --COPYRIGHT--,BSD
 * Copyright (c) 2011, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//  Description:  Master include file
//
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.1
//
// W. Goh
// Texas Instruments, Inc
// December 2009
// Built with IAR Embedded Workbench Version: 4.20
//******************************************************************************
// Change Log:
//******************************************************************************
// Version:  1.1
// Comments: Added support for various MSP430 development tools
// Version:  1.00
// Comments: Initial Release Version
//******************************************************************************

#include "TI_CC_CC1100-CC2500.h"
#include "TI_CC_spi.h"
#include "CC1100-CC2500.h"

#include "TI_CC_msp430.h"

// Uncomment/Comment out depending on which experimenters board is being used
#include "TI_CC_hardware_board_launchpad.h"
//#include "TI_CC_hardware_board.h"
//#include "TI_CC_hardware_board_eZ430.h"
//#include "TI_CC_hardware_board_EXP4618.h"
//#include "TI_CC_hardware_board_EXP5438.h"

