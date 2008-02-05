
/*
 * Debug.java
 */

package com.skyetek.utility;

/**
 *
 * @author  BenGriffin
 */
public class Debug {

    public static String debugBytes(byte [] bytes ) {
	return debugBytes(bytes,bytes.length);
    }
    public static String debugBytes(byte [] bytes, int length ) {
	StringBuffer buffer = new StringBuffer();
	for ( int i = 0 ; i < length; i ++ ) {
	    int value = (0xff & bytes[i]);
	    if ( value < 0x0f ) {
		buffer.append("[0x0");
		buffer.append(Integer.toHexString(value));
		buffer.append("]");
	    } else {
		buffer.append("[0x");
		buffer.append(Integer.toHexString(value));
		buffer.append("]");
	    }
	}
	return buffer.toString();
    }
}
