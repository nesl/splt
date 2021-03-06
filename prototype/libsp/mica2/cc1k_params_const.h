/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4: */
/*									tab:4
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.  By
 *  downloading, copying, installing or using the software you agree to
 *  this license.  If you do not agree to this license, do not download,
 *  install, copy or use the software.
 *
 *  Intel Open Source License 
 *
 *  Copyright (c) 2002 Intel Corporation 
 *  All rights reserved. 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 * 
 *	Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *	Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *      Neither the name of the Intel Corporation nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE INTEL OR ITS
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/**
 * @author Phil Buonadonna
 * @author Simon Han
 *   Based on CC1000.h
 *   Port to SOS
 */

#ifndef _SOS_CC1K_PARAMS_CONST
#define _SOS_CC1K_PARAMS_CONST

static const prog_uchar CC1K_Params[6][31] = {
  // (0) 433.002 MHz channel, 19.2 Kbps data, Manchester Encoding, High Side LO
  { // MAIN   0x00
    0x31,
    // FREQ2A,FREQ1A,FREQ0A  0x01-0x03
    0x58,0x00,0x00,					
    // FREQ2B,FREQ1B,FREQ0B  0x04-0x06
    0x57,0xf6,0x85,    //XBOW
    // FSEP1, FSEP0     0x07-0x08
    0X03,0x55,
    // CURRENT (RX MODE VALUE)   0x09 (also see below)
    ((4<<CC1K_VCO_CURRENT) | (1<<CC1K_LO_DRIVE)),	
    // FRONT_END  0x0a
    ((1<<CC1K_IF_RSSI)),
    // PA_POW  0x0b
    ((0xf<<CC1K_PA_HIGHPOWER) | (0xf<<CC1K_PA_LOWPOWER)), 
    // PLL  0x0c
    ((12<<CC1K_REFDIV)),		
    // LOCK  0x0d
    ((0xe<<CC1K_LOCK_SELECT)),
    // CAL  0x0e
    ((1<<CC1K_CAL_WAIT) | (6<<CC1K_CAL_ITERATE)),	
    // MODEM2  0x0f
    ((0<<CC1K_PEAKDETECT) | (28<<CC1K_PEAK_LEVEL_OFFSET)),
    // MODEM1  0x10
    ((3<<CC1K_MLIMIT) | (1<<CC1K_LOCK_AVG_MODE) | (CC1K_Settling<<CC1K_SETTLING) | (1<<CC1K_MODEM_RESET_N)), 
    // MODEM0  0x11
    ((5<<CC1K_BAUDRATE) | (1<<CC1K_DATA_FORMAT) | (1<<CC1K_XOSC_FREQ)),
    // MATCH  0x12
    ((0x7<<CC1K_RX_MATCH) | (0x0<<CC1K_TX_MATCH)),
    // FSCTRL 0x13
    ((1<<CC1K_FS_RESET_N)),			
    // FSHAPE7 - FSHAPE1   0x14-0x1a
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
    // FSDELAY   0x1b
    0x00,	
    // PRESCALER    0x1c
    0x00,
    // CURRENT (TX MODE VALUE)  0x1d
    ((8<<CC1K_VCO_CURRENT) | (1<<CC1K_PA_DRIVE)),
    // High side LO  0x1e (i.e. do we need to invert the data?)
    1
  },

  // (1) 914.9988 MHz channel, 19.2 Kbps data, Manchester Encoding, High Side LO
  { // MAIN   0x00 
    0x31,
    // FREQ2A,FREQ1A,FREQ0A  0x01-0x03
    0x7c,0x00,0x00,					
    // FREQ2B,FREQ1B,FREQ0B  0x04-0x06
    0x7b,0xf9,0xae,					
    // FSEP1, FSEP0     0x07-0x8
    0x02,0x38,
    // CURRENT (RX MODE VALUE)   0x09 (also see below)
    ((8<<CC1K_VCO_CURRENT) | (3<<CC1K_LO_DRIVE)),
    //0x8C,	
    // FRONT_END  0x0a
    ((1<<CC1K_BUF_CURRENT) | (2<<CC1K_LNA_CURRENT) | (1<<CC1K_IF_RSSI)),
    //0x32,
    // PA_POW  0x0b
    ((0x8<<CC1K_PA_HIGHPOWER) | (0x0<<CC1K_PA_LOWPOWER)), 
    //0xff,
    // PLL  0xc
    ((8<<CC1K_REFDIV)),		
    //0x40,
    // LOCK  0xd
    ((0x1<<CC1K_LOCK_SELECT)),
    //0x10,
    // CAL  0xe
    ((1<<CC1K_CAL_WAIT) | (6<<CC1K_CAL_ITERATE)),	
    //0x26,
    // MODEM2  0xf
    ((1<<CC1K_PEAKDETECT) | (33<<CC1K_PEAK_LEVEL_OFFSET)),
    //0xA1,
    // MODEM1  0x10
    ((3<<CC1K_MLIMIT) | (1<<CC1K_LOCK_AVG_MODE) | (CC1K_Settling<<CC1K_SETTLING) | (1<<CC1K_MODEM_RESET_N)), 
    //0x6f, 
    // MODEM0  0x11
    ((5<<CC1K_BAUDRATE) | (1<<CC1K_DATA_FORMAT) | (1<<CC1K_XOSC_FREQ)),
    //0x55,
    // MATCH 0x12
    ((0x1<<CC1K_RX_MATCH) | (0x0<<CC1K_TX_MATCH)),
    //0x10,
    // FSCTRL  0x13
    ((1<<CC1K_FS_RESET_N)),			
    //0x01,
    // FSHAPE7 - FSHAPE1   0x14..0x1a
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
    // FSDELAY   0x1b
    0x00,	
    // PRESCALER    0x1c
    0x00,
    // CURRENT (TX MODE VALUE)  0x1d
    ((15<<CC1K_VCO_CURRENT) | (3<<CC1K_PA_DRIVE)),
    //0xf3,
    // High side LO  0x1e (i.e. do we need to invert the data?)
    1
  },

  // (2) 434.845200 MHz channel, 19.2 Kbps data, Manchester Encoding, High Side LO
  { // MAIN   0x00
    0x31,
    // FREQ2A,FREQ1A,FREQ0A  0x01-0x03
    0x51,0x00,0x00,					
    // FREQ2B,FREQ1B,FREQ0B  0x04-0x06
    0x50,0xf7,0x4F,    //XBOW
    // FSEP1, FSEP0     0x07-0x08
    0X03,0x0E,
    // CURRENT (RX MODE VALUE)   0x09 (also see below)
    ((4<<CC1K_VCO_CURRENT) | (1<<CC1K_LO_DRIVE)),	
    // FRONT_END  0x0a
    ((1<<CC1K_IF_RSSI)),
    // PA_POW  0x0b
    ((0x0<<CC1K_PA_HIGHPOWER) | (0xf<<CC1K_PA_LOWPOWER)), 
    // PLL  0x0c
    ((11<<CC1K_REFDIV)),		
    // LOCK  0x0d
    ((0xe<<CC1K_LOCK_SELECT)),
    // CAL  0x0e
    ((1<<CC1K_CAL_WAIT) | (6<<CC1K_CAL_ITERATE)),	
    // MODEM2  0x0f
    ((1<<CC1K_PEAKDETECT) | (33<<CC1K_PEAK_LEVEL_OFFSET)),
    // MODEM1  0x10
    ((3<<CC1K_MLIMIT) | (1<<CC1K_LOCK_AVG_MODE) | (CC1K_Settling<<CC1K_SETTLING) | (1<<CC1K_MODEM_RESET_N)), 
    // MODEM0  0x11
    ((5<<CC1K_BAUDRATE) | (1<<CC1K_DATA_FORMAT) | (1<<CC1K_XOSC_FREQ)),
    // MATCH  0x12
    ((0x7<<CC1K_RX_MATCH) | (0x0<<CC1K_TX_MATCH)),
    // FSCTRL 0x13
    ((1<<CC1K_FS_RESET_N)),			
    // FSHAPE7 - FSHAPE1   0x14-0x1a
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
    // FSDELAY   0x1b
    0x00,	
    // PRESCALER    0x1c
    0x00,
    // CURRENT (TX MODE VALUE)  0x1d
    ((8<<CC1K_VCO_CURRENT) | (1<<CC1K_PA_DRIVE)),
    // High side LO  0x1e (i.e. do we need to invert the data?)
    1
  },

 
  // (3) 914.077 MHz channel, 19.2 Kbps data, Manchester Encoding, High Side LO
  { // MAIN   0x00 
    0x31,
    // FREQ2A,FREQ1A,FREQ0A  0x01-0x03
    0x5c,0xe0,0x00,					
    // FREQ2B,FREQ1B,FREQ0B  0x04-0x06
    0x5c,0xdb,0x42,					
    // FSEP1, FSEP0     0x07-0x8
    0x01,0xAA,
    // CURRENT (RX MODE VALUE)   0x09 (also see below)
    ((8<<CC1K_VCO_CURRENT) | (3<<CC1K_LO_DRIVE)),
    //0x8C,	
    // FRONT_END  0x0a
    ((1<<CC1K_BUF_CURRENT) | (2<<CC1K_LNA_CURRENT) | (1<<CC1K_IF_RSSI)),
    //0x32,
    // PA_POW  0x0b
    ((0x8<<CC1K_PA_HIGHPOWER) | (0x0<<CC1K_PA_LOWPOWER)), 
    //0xff,
    // PLL  0xc
    ((6<<CC1K_REFDIV)),		
    //0x40,
    // LOCK  0xd
    ((0x1<<CC1K_LOCK_SELECT)),
    //0x10,
    // CAL  0xe
    ((1<<CC1K_CAL_WAIT) | (6<<CC1K_CAL_ITERATE)),	
    //0x26,
    // MODEM2  0xf
    ((1<<CC1K_PEAKDETECT) | (33<<CC1K_PEAK_LEVEL_OFFSET)),
    //0xA1,
    // MODEM1  0x10
    ((3<<CC1K_MLIMIT) | (1<<CC1K_LOCK_AVG_MODE) | (CC1K_Settling<<CC1K_SETTLING) | (1<<CC1K_MODEM_RESET_N)), 
    //0x6f, 
    // MODEM0  0x11
    ((5<<CC1K_BAUDRATE) | (1<<CC1K_DATA_FORMAT) | (1<<CC1K_XOSC_FREQ)),
    //0x55,
    // MATCH 0x12
    ((0x1<<CC1K_RX_MATCH) | (0x0<<CC1K_TX_MATCH)),
    //0x10,
    // FSCTRL  0x13
    ((1<<CC1K_FS_RESET_N)),			
    //0x01,
    // FSHAPE7 - FSHAPE1   0x14..0x1a
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
    // FSDELAY   0x1b
    0x00,	
    // PRESCALER    0x1c
    0x00,
    // CURRENT (TX MODE VALUE)  0x1d
    ((15<<CC1K_VCO_CURRENT) | (3<<CC1K_PA_DRIVE)),
    //0xf3,
    // High side LO  0x1e (i.e. do we need to invert the data?)
    1
  },

  // (4) 315.178985 MHz channel, 38.4 Kbps data, Manchester Encoding, High Side LO
  { // MAIN   0x00
    0x31,
    // FREQ2A,FREQ1A,FREQ0A  0x01-0x03
    0x45,0x60,0x00,					
    // FREQ2B,FREQ1B,FREQ0B  0x04-0x06
    0x45,0x55,0xBB,
    // FSEP1, FSEP0     0x07-0x08
    0X03,0x9C,
    // CURRENT (RX MODE VALUE)   0x09 (also see below)
    ((8<<CC1K_VCO_CURRENT) | (0<<CC1K_LO_DRIVE)),	
    // FRONT_END  0x0a
    ((1<<CC1K_IF_RSSI)),
    // PA_POW  0x0b
    ((0x0<<CC1K_PA_HIGHPOWER) | (0xf<<CC1K_PA_LOWPOWER)), 
    // PLL  0x0c
    ((13<<CC1K_REFDIV)),		
    // LOCK  0x0d
    ((0xe<<CC1K_LOCK_SELECT)),
    // CAL  0x0e
    ((1<<CC1K_CAL_WAIT) | (6<<CC1K_CAL_ITERATE)),	
    // MODEM2  0x0f
    ((1<<CC1K_PEAKDETECT) | (33<<CC1K_PEAK_LEVEL_OFFSET)),
    // MODEM1  0x10
    ((3<<CC1K_MLIMIT) | (1<<CC1K_LOCK_AVG_MODE) | (CC1K_Settling<<CC1K_SETTLING) | (1<<CC1K_MODEM_RESET_N)), 
    // MODEM0  0x11
    ((5<<CC1K_BAUDRATE) | (1<<CC1K_DATA_FORMAT) | (0<<CC1K_XOSC_FREQ)),
    // MATCH  0x12
    ((0x7<<CC1K_RX_MATCH) | (0x0<<CC1K_TX_MATCH)),
    // FSCTRL 0x13
    ((1<<CC1K_FS_RESET_N)),			
    // FSHAPE7 - FSHAPE1   0x14-0x1a
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
    // FSDELAY   0x1b
    0x00,	
    // PRESCALER    0x1c
    0x00,
    // CURRENT (TX MODE VALUE)  0x1d
    ((8<<CC1K_VCO_CURRENT) | (1<<CC1K_PA_DRIVE)),
    // High side LO  0x1e (i.e. do we need to invert the data?)
    1
  },

  // (5) Spare
  { // MAIN   0x00
    0x31,
    // FREQ2A,FREQ1A,FREQ0A  0x01-0x03
    0x58,0x00,0x00,					
    // FREQ2B,FREQ1B,FREQ0B  0x04-0x06
    0x57,0xf6,0x85,    //XBOW
    // FSEP1, FSEP0     0x07-0x08
    0X03,0x55,
    // CURRENT (RX MODE VALUE)   0x09 (also see below)
    ((8<<CC1K_VCO_CURRENT) | (4<<CC1K_LO_DRIVE)),	
    // FRONT_END  0x0a
    ((1<<CC1K_IF_RSSI)),
    // PA_POW  0x0b
    ((0x0<<CC1K_PA_HIGHPOWER) | (0xf<<CC1K_PA_LOWPOWER)), 
    // PLL  0x0c
    ((12<<CC1K_REFDIV)),		
    // LOCK  0x0d
    ((0xe<<CC1K_LOCK_SELECT)),
    // CAL  0x0e
    ((1<<CC1K_CAL_WAIT) | (6<<CC1K_CAL_ITERATE)),	
    // MODEM2  0x0f
    ((1<<CC1K_PEAKDETECT) | (33<<CC1K_PEAK_LEVEL_OFFSET)),
    // MODEM1  0x10
    ((3<<CC1K_MLIMIT) | (1<<CC1K_LOCK_AVG_MODE) | (CC1K_Settling<<CC1K_SETTLING) | (1<<CC1K_MODEM_RESET_N)),    // MODEM0  0x11
    ((5<<CC1K_BAUDRATE) | (1<<CC1K_DATA_FORMAT) | (1<<CC1K_XOSC_FREQ)),
    // MATCH  0x12
    ((0x7<<CC1K_RX_MATCH) | (0x0<<CC1K_TX_MATCH)),
    // FSCTRL 0x13
    ((1<<CC1K_FS_RESET_N)),			
    // FSHAPE7 - FSHAPE1   0x14-0x1a
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
    // FSDELAY   0x1b
    0x00,	
    // PRESCALER    0x1c
    0x00,
    // CURRENT (TX MODE VALUE)  0x1d
    ((8<<CC1K_VCO_CURRENT) | (1<<CC1K_PA_DRIVE)),
    // High side LO  0x1e (i.e. do we need to invert the data?)
    1
  },
};

#endif // _SOS_CC1K_PARAMS_CONST

