/*
 * SkyeTekListener.java
 *
 */

package com.skyetek.reader;

/**
 * Listener interface definition.  Defines the methods a SkyeTek reader class 
 * must implement in order to communicate with an RFID tag object.
 */
public interface SkyeTekListener {
    void notify(SkyeTekTag tag);
}

