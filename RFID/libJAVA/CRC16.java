/*
 * @(#)CRC16.java	1.12 00/02/02
 *
 * Copyright 1994-2000 Sun Microsystems, Inc. All Rights Reserved.
 * 
 * This software is the proprietary information of Sun Microsystems, Inc.  
 * Use is subject to license terms.
 * 
 */

package com.skyetek.utility;

/**
 * The CRC-16 class calculates a 16 bit cyclic redundancy check of a set
 * of bytes. This error detecting code is used to determine if bit rot
 * has occured in a byte stream.
 */

public class CRC16 {

    /** value contains the currently computed CRC, set it to 0 initally */
    public int value;

    public CRC16() {
	value = 0;
    }
    public void update(byte[] buffer, int n)
    {
        int i, j;     /* byte coutner, bit counter */
        int crc_16;    /* calculation */

        crc_16 = 0x0000;         /* PRESET value */

        for(i = 0; i < n; i++)
        {
            crc_16 ^= Numbers.byteToPositiveInt(buffer[i]);

            for(j = 0; j < 8; j++) /* test each bit in the byte */
            {
                if((crc_16 & 0x0001) != 0)
                {
                    crc_16 >>= 1;
                    crc_16 ^= 0x8408;
                }
                else
                {
                    crc_16 >>= 1;
                }

            }

        }
        value = crc_16;
        return;  /* returns calculated crc (16 bits) */
    }


    /** reset CRC value to 0 */
    public void reset() {
	value = 0;
    }
}
