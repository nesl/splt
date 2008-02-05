/*
 * SkyeTekException.java
 */

package com.skyetek.utility;

import java.io.PrintStream;

public class SkyeTekException extends java.lang.Exception {

    private byte responseCode = 0x00;
    private Throwable _cause = null;
    
    /**
     * Creates a new instance of <code>SkyeTekException</code> without detail message.
     */
    public SkyeTekException() {
	super();
    }
    
    
    /**
     * Constructs an instance of <code>SkyeTekException</code> with the specified detail message.
     * @param msg the detail message.
     */
    public SkyeTekException(String msg) {
        super(msg);
    }

    /**
     * Constructs an instance of <code>SkyeTekException</code> with a 
     * detail message and a underlying error.
     * @param msg the detail message.
     */
    public SkyeTekException(Throwable ex, String msg) {
        super(msg);
	_cause = ex;
    }

    public SkyeTekException(byte responseCode) {
        super();
	this.responseCode = responseCode;
    }

    public SkyeTekException(byte responseCode, String msg) {
        super(msg);
	this.responseCode = responseCode;
    }

    public String getMessage() {
	if ( responseCode != 0 ) {
	    return super.getMessage() + ": reader response code:" + Integer.toHexString(responseCode&0xff);
	} else if (_cause != null) {
	    return super.getMessage() + ":Underlying Error=" + _cause.getMessage();
	} else {
	    return super.getMessage();
	}
    }
    
    public void printStackTrace(PrintStream s) {
	super.printStackTrace(s);
	if ( _cause != null ) {
	    _cause.printStackTrace(s);
	}
    }
}
