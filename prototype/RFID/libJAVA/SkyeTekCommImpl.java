/*
 * SkyeTekCommImpl.java
 */

package com.skyetek.comm;

import com.skyetek.utility.SkyeTekException;

/**
 * Communications Implementation Interface.  Defines the methods all 
 * communications must implement in order to communicate with a SkyeTek
 * reader.
 */
public interface SkyeTekCommImpl {
    
    public void startup() throws SkyeTekException;

    public void shutdown();

    public void restart() throws SkyeTekException;

    public void setAsyncReadMode(boolean value);
    
    public void sendRequest(byte [] request) throws SkyeTekException;

    public byte [] getNextResponse() throws SkyeTekException;

    public void registerSkyeTekComm(SkyeTekComm comm);
}
