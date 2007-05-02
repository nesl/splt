/*
 * SkyeTekComm.java
 *
 */

package com.skyetek.comm;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Properties;
import com.skyetek.utility.SkyeTekException;
import com.skyetek.reader.SkyeTekListener;
import com.skyetek.reader.SkyeTekTag;
import com.skyetek.utility.Debug;


/**
 * This class creates a new SkyeTek Comm object which provides a host the means
 * with communicating with a SkyeTek reader.  This class provides all 
 * the methods for that are common to all implementations of the communications
 * class, including adding and removing event listeners, getting responses
 * from the reader, and all synchronous and asynchronous communications 
 * protocols.  
 *
 */
public class SkyeTekComm {

    private SkyeTekCommImpl impl = null;
    
    private ArrayList _listeners = new ArrayList();
    
    /** Creates a new instance of SkyeTekComm */
    public SkyeTekComm(Properties properties) throws SkyeTekException {
        String className = properties.getProperty("SkyeTekCommImpl");
        
     
        if ( className != null ) {
	    try {
		Class c = Class.forName(className);
		Class [] paramTypes = {Properties.class};
		Constructor constructor = c.getDeclaredConstructor(paramTypes);
		Object [] params = {properties};
		SkyeTekCommImpl impl = (SkyeTekCommImpl)constructor.newInstance(params);
		this.impl = impl;
		impl.registerSkyeTekComm(this);
	    } catch ( ClassNotFoundException ex) {
		throw new SkyeTekException(ex,"Error instantiating " + className);
	    } catch ( NoSuchMethodException nsme ) {
		throw new SkyeTekException(nsme,"Error instantiating " + className);
	    } catch ( IllegalAccessException iae ) {
		throw new SkyeTekException(iae,"Error instantiating " + className);
	    } catch ( InvocationTargetException ite ) {
		throw new SkyeTekException(ite,"Error instantiating " + className);
	    } catch ( InstantiationException ie ) {
		throw new SkyeTekException(ie,"Error instantiating " + className);
	    } 
	} else {
	    throw new SkyeTekException("No SkyeTekCommImpl specified.");
	}
    }

    /** Creates a new instance of SkyeTekComm */
    public SkyeTekComm(SkyeTekCommImpl impl) throws SkyeTekException {
	this.impl = impl;
	impl.registerSkyeTekComm(this);
    }

    /** Terminate the communication connection.
     **/
    public void shutdown() {
	impl.shutdown();
    }

    /** Attempt to restart the connection.  Usually entails closing and 
     * reopening the connection.
     **/
    public void restart() throws SkyeTekException {
	impl.restart();
    }


    /** Sends the request bytes through to the reader and returns imediately.
     **/
    public void sendAsyncRequest(byte [] request) throws SkyeTekException {
	impl.sendRequest(request);
    }

    /** Sends the request bytes to the reader and blocks until a response 
     *  message is received.
     **/
    public byte [] sendBlockingRequest(byte [] request) throws SkyeTekException {
	byte [] data = null;
	boolean retry = false;
	do {
	    retry = false;
	    impl.sendRequest(request);
             try {Thread.sleep(10);} catch (InterruptedException e) {}
	    data = impl.getNextResponse();

	    try { 
		int x = 20;
		//System.out.println("Sleeping for " + x + "ms");
		Thread.sleep(x);  
	    } catch (InterruptedException ex) {}

	if ( (data.length > 0) && (data[0] == (byte)(0x84) ) ) {
	    System.out.println("got an 84, retrying... ==========================");
	    try {Thread.sleep(10);} catch (InterruptedException e) {}
	    //retry = true;
	}

	} while (retry);
	return data;
    }

    /** Reads available data from reader.  This is dependent on implementation
     *  of the actual communications class.
     **/
    public byte [] getNextResponse() throws SkyeTekException{
	return impl.getNextResponse();
    }
    
    /** Sets up the communications layer for Asynchronous reads.  This is ideal
     *  For loop mode operation.
     **/
    public void setAsyncReadMode(boolean value) {
	impl.setAsyncReadMode(value);
    }


    /** A method for implementations to notify listeners of a new tag read.
     **/
    protected void notifyListeners( SkyeTekTag tag ) {
        synchronized (_listeners) {
            Object array[] = _listeners.toArray();
            for ( int i = 0; i < array.length; i++ ) {
                ((SkyeTekListener)array[i]).notify(tag);
            }
        }
    }
    
    /** Registers a new Tag Listener 
     **/
    public void addListener( SkyeTekListener listener ) {
        synchronized (_listeners) {
            _listeners.add(listener);
        }
    }
    
    /** Unregisters a tag listener
     **/
    public synchronized void removeListener( SkyeTekListener listener ) {
        synchronized (_listeners) {
            for ( int i = 0; i < _listeners.size(); i++ ) {
                if ( _listeners.get(i) == listener ) {
                    _listeners.remove(i);
                }
            }
        }
    }
}
