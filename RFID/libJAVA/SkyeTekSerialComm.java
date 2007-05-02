
/*
 * SkyeTekSerialComm.java
 */

package com.skyetek.comm;

import javax.comm.CommDriver;
import javax.comm.CommPortIdentifier;
import javax.comm.PortInUseException;
import javax.comm.UnsupportedCommOperationException;
import javax.comm.SerialPortEventListener;
import javax.comm.SerialPortEvent;
import javax.comm.SerialPort;

import java.util.Enumeration;
import java.util.Properties;
import java.util.TooManyListenersException;
import java.io.OutputStream;
import java.io.BufferedInputStream;
import java.io.InputStream;
import java.io.IOException;

import com.skyetek.utility.SkyeTekException;
import com.skyetek.messaging.SkyeTekMessaging;
import com.skyetek.reader.SkyeTekTag;
import com.skyetek.utility.CRC16;
import com.skyetek.utility.Debug;

/*
  todo:
    2 - add restart thread, if io error encountered
 */

/**
 *
 * @author  BenGriffin
 */
public class SkyeTekSerialComm implements SerialPortEventListener, SkyeTekCommImpl {

{
    // do this so the driver will load in some environments
    try {
	CommDriver driver = (CommDriver)Class.forName("com.sun.comm.Win32Driver").newInstance();
	//CommDriver driver = (CommDriver)Class.forName("gnu.io.RXTXCommDriver").newInstance();
	driver.initialize();
    } catch (Exception e) {}     

}

    public static final String COM_PORT_PARAM = "SerialPort";
    public static final String BAUD_RATE_PARAM = "BaudRate";

    private static final byte STX = (byte)0x02;
    private CommPortIdentifier _portId = null;
    private int _baudRate = 0;
    private int [] _baudRateList = {9600, 19200, 38400, 57600};
    
    private SerialPort _serialPort;
    private OutputStream _outputStream;
    private BufferedInputStream _inputStream;
    private SkyeTekComm _skyeTekComm = null;
    private boolean _asyncRead = false;

    private boolean _debug = false;

    /** Creates a new instance of SkyeTekComm */
    public SkyeTekSerialComm() throws SkyeTekException {
	initDebugFlag();
	startup();
    }

    /** Creates a new instance of SkyeTekComm */
    public SkyeTekSerialComm(Properties properties) throws SkyeTekException {
	initDebugFlag();
	startup(properties);
    }

    /** Creates a new instance of SkyeTekComm */
    public SkyeTekSerialComm(String portName, int baudRate) throws SkyeTekException {
	initDebugFlag();
	Properties props = new Properties();
	props.setProperty(COM_PORT_PARAM,portName);
	props.setProperty(BAUD_RATE_PARAM,Integer.toString(baudRate));
	startup(props);
    }
    
    private void initDebugFlag() {
	if ( System.getProperty("com.skyetek.debug") != null ) {
	    _debug = true;
	}
    }
    
    /** This method cycles through all valid com ports and all baud rates until
     *  it detects a SkyeTek reader.  Once a reader is found, the com port is 
     * opened and readied for communications
     */
    public void startup() throws SkyeTekException {
	boolean result = searchForReader();
	if ( result ) {
	    if ( _debug ) System.out.println("found reader @ " + _portId.getName() + ", " + _baudRate + " baud");
	} else {
            throw new SkyeTekException("No SkyeTek reader found on serial ports.");
	}
    }

    /** This method opens up a communications port with the properties specified 
     *
     *  @param   properties the specific properties of the comm implementation
     */
    public void startup(Properties properties) throws SkyeTekException {
	String portName = properties.getProperty(COM_PORT_PARAM);
	if ( portName == null ) {
	    //System.out.println(COM_PORT_PARAM + " parameter missing.");
	}
	String baudRateStr = properties.getProperty(BAUD_RATE_PARAM);
	if ( baudRateStr == null ) {
	    //System.out.println(BAUD_RATE_PARAM + " parameter missing.");
	} else {
	    _baudRate = Integer.parseInt(baudRateStr);
	}

	if ( ( _baudRate != 0 ) && ( portName != null ) ) {
	    setupSerialPort(portName,
			    _baudRate);
	} else {
	    if ( _debug ) System.out.println(COM_PORT_PARAM + " or " + BAUD_RATE_PARAM + " not set, searching com ports.");
	    startup();
	}
    }

    /* This method closes the com port */
    public void shutdown() {
        try {
            _serialPort.close();
        } catch (Exception ex) {}
    }
    
    /* This method closes the serial port then reinitializes it. */
    public void restart() throws SkyeTekException {
	shutdown();
        setupSerialPort(_portId,_baudRate);
        
    }

  /**
   * This method sets up the serial port for communications with a SkyeTek
   * reader.
   *
   * @param   portname the com port name which the reader is connected to.
   * @param   baudRate communication speed which the reader is set to 
   *          communicate at 
   */
    private void setupSerialPort(String portName,int baudRate) throws SkyeTekException{
        CommPortIdentifier portId = getPortId(portName);
        if ( portId == null ) {
            throw new SkyeTekException("Port '"+portName+"' not found.");
        }
	setupSerialPort(portId, baudRate);
    }

  /**
   * This method sets up the serial port for communications with a SkyeTek
   * reader.
   *
   * @param   portId the com port name which the reader is connected to.
   * @param   baudRate communication speed which the reader is set to 
   *          communicate at 
   * 
   */
    private void setupSerialPort(CommPortIdentifier portId,int baudRate) throws SkyeTekException{
        try {
            _serialPort = (SerialPort)portId.open("SkyeTekReader", 2000);
            _outputStream = _serialPort.getOutputStream();
            _inputStream = new BufferedInputStream(_serialPort.getInputStream());
            _serialPort.addEventListener(this);
	    _serialPort.notifyOnDataAvailable(true);
            
            _serialPort.setSerialPortParams(baudRate,
					    SerialPort.DATABITS_8,
					    SerialPort.STOPBITS_1,
					    SerialPort.PARITY_NONE);
        } catch (PortInUseException e) {
            throw new SkyeTekException(e,"Port '"+ portId.getName()+"' in use.");
        } catch (IOException ioErr) {
            throw new SkyeTekException(ioErr,"Error while setting up serial port.");
        } catch (UnsupportedCommOperationException ucoe) {
            throw new SkyeTekException(ucoe,"Error while setting up serial port.");
        } catch (TooManyListenersException tmle) {
	    throw new SkyeTekException(tmle,"Too Many Listeners on comm port '"+portId.getName()+"'");
	} 
    }
    
    
  /**
   * This method cycles through available com ports and baud rates for each
   * port and detects a reader.  If a reader is detected, the serial port is 
   * setup for communication.
   */
    private boolean searchForReader() {
        Enumeration portList = CommPortIdentifier.getPortIdentifiers();
     
        CommPortIdentifier portId;

        while (portList.hasMoreElements()) {
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
		// found a serial port, now try all the baud rates.
		for ( int i = 0; i< _baudRateList.length; i++ ) {
		    //if ( _debug ) System.out.println("Trying " +portId.getName() + " @ " + _baudRateList[i]);
		    try {
			setupSerialPort(portId,_baudRateList[i]);
			sendRequest(SkyeTekMessaging.getSerialNumber(SkyeTekMessaging.RAM));
			// need to read result
			try {
			    byte buffer[] = readBuffer(400);
                          
			    if ( buffer != null && buffer.length == 5 ) {
				// success !
				this._portId = portId;
				this._baudRate = _baudRateList[i];
				return true;
			    } else {
				_serialPort.close();
			    }
			} catch (SkyeTekException ex) {
                            
			    _serialPort.close();
			} 
		    } catch ( SkyeTekException ex ) {
			ex.printStackTrace();
			System.out.println(ex);
			_serialPort.close();
		    }
		}
            }
        }
	return false;
    }


  /**
   * This method returns the port ID of a com port name.
   *
   * @param   portName string that specifies the com port
   */
    private CommPortIdentifier getPortId( String portName ) {
        Enumeration portList = CommPortIdentifier.getPortIdentifiers();
        CommPortIdentifier portId;
        while (portList.hasMoreElements()) {
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                if (portId.getName().equals(portName)) {
                    return portId;
                }
            }
        }
        return null;
    }
    
    
  /**
   * This method sends an actual request to a SkyeTek reader.  
   *
   * @param   request the byte command to be sent to the reader according
   *          to SkyeTek protocol v2.0
   */
    public void sendRequest(byte [] request) throws SkyeTekException {
	try { 
	    //if ( _debug ) System.out.println("sending:"+Debug.debugBytes(request));
	    byte [] sendBuffer = new byte[request.length + 1];
	    System.arraycopy(request,0,sendBuffer,1,request.length);
	    sendBuffer[0] = STX;

//	    _outputStream.write(STX);
//	    _outputStream.write(request);
	    _outputStream.write(sendBuffer);
	    _outputStream.flush();
	} catch ( IOException ex ) {
	    // might have lost connection, go into restart mode
	    throw(new SkyeTekException(ex,"Error while sending request."));
	}
    }

    
    
  /**
   * This method detects serial port events and broadcasts when data is available
   * to all listeners.
   *
   * @param   event the serial port event to act on
   */
    public synchronized void serialEvent(SerialPortEvent event) {
        switch(event.getEventType()) {
            case SerialPortEvent.BI:
            case SerialPortEvent.OE:
            case SerialPortEvent.FE:
            case SerialPortEvent.PE:
            case SerialPortEvent.CD:
            case SerialPortEvent.CTS:
            case SerialPortEvent.DSR:
            case SerialPortEvent.RI:
            case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
                break;
            case SerialPortEvent.DATA_AVAILABLE:
		if ( _asyncRead ) {
		    try { 
			while ( _inputStream.available() != 0 ) {
			    broadcastResponse(readBuffer(0));
			}
		    } catch ( SkyeTekException ex ) {
			System.out.println(ex);
		    } catch ( IOException ex ) {
			System.out.println(ex);
		    }
		}
		this.notifyAll();
                break;
        }
    }

       
  /**
   * This method sets up the host for reading serial data asynchronously.
   *
   * @param   value this value turns asynchronous read mode on and off
   */
    public void setAsyncReadMode( boolean value ) {
	_asyncRead = value;
    }
    
    
        
  /**
   * This method reads available data from the serial port when data is available.
   * 
   * @param   waitTime amount of time in ms that this function will block until
   *          reading data available on the serial port 
   *
   * @return  the reader response without the beginning STX and without the 
   *          CRC  
   */
    protected synchronized byte [] readBuffer(int waitTime) throws SkyeTekException {
	byte [] result = null;
	try {
	    int pos = 0;
	    
	    if ( _inputStream.available() == 0 ) {
		try {
		    this.wait(1000);
		} 
		catch (InterruptedException ex ) {
		}
	    }

	    if ( _inputStream.available() == 0 ) {
               
		throw new SkyeTekException("No data available to read");
	    }

	    byte start = (byte)(_inputStream.read() & 0xff);  // read the STX

	    if ( start != STX ) {
		System.out.println("NOT STX:" + Integer.toHexString(start & 0xff));
		start = (byte)(_inputStream.read() & 0xff);  // read the STX
		if ( start != STX ) {
		    System.out.println("NOT STX:" + Integer.toHexString(start & 0xff));
		    throw new SkyeTekException("NO STX at beginnning of stream [" + Integer.toHexString(start & 0xff) + "]");
		}
	    }
	    byte length = (byte)(_inputStream.read() & 0xff);  // read the message length
	    byte [] readBuffer = new byte[length+1];
	    readBuffer[0] = length;
	    int totalRead = 0;
	    while (totalRead != length ){
		int actualBytes = _inputStream.read(readBuffer,totalRead+1,length-totalRead);
		totalRead += actualBytes;
	    }

	    CRC16 crc = new CRC16();
	    crc.update(readBuffer,readBuffer.length-2);

	    int sentCRC = ((readBuffer[readBuffer.length-2]& 0xff) << 8) | (readBuffer[readBuffer.length-1]& 0xff);

	    if ( sentCRC != crc.value ) {
		throw new SkyeTekException("BAD CRC in reader response message");
	    }
	    
	    //if ( _debug ) System.out.println("Just read:"+Debug.debugBytes(readBuffer,length +1));
	    byte [] clippedArray = new byte[length-2];
	    System.arraycopy(readBuffer,1,clippedArray,0,length-2);
	    result = clippedArray;
         
	} catch (IOException e) {
	    throw new SkyeTekException(e,"IO Error occured during readBuffer");
	}

	return result;
    }

        
  /**
   * This method calls readBuffer and gets the next available response.
   */
    public  synchronized  byte [] getNextResponse() throws SkyeTekException{
	byte [] response = (byte [])readBuffer(500);
	return response;
    }

  /**
   * This method broadcasts the reader response to all registered listeners.
   * This includes the tagType, idLength, and also tagID.  
   * 
   * @param   response this is the response from reading the input stream of the
   *          serial port
   */    
    private void broadcastResponse( byte [] response ) {
        
        if ( response.length == 0 ) {
            return;
        }
	byte responseCode = response[0];
        if ( responseCode == 0x14 ) {
	    byte tagType = response[1];
	    int idLength = response.length - 2;
	    byte [] id = new byte[idLength];
	    System.arraycopy(response,2,id,0,idLength);

	    SkyeTekTag tag = new SkyeTekTag(_skyeTekComm,tagType,id);
            _skyeTekComm.notifyListeners(tag);
        }
    }

    public void registerSkyeTekComm(SkyeTekComm skyeTekComm) {
	this._skyeTekComm = skyeTekComm;
    }
   
    
    /* Serial Communications Test Harness */
    /* Example SerialComm Usage */
    
    public static void main(String args[]) {
        try {
	    System.setProperty("com.skyetek.debug","true");
          
	//  SkyeTekSerialComm comm = new SkyeTekSerialComm("COM1",9600);
	    SkyeTekSerialComm comm = new SkyeTekSerialComm();
        //  SkyeTekSerialComm comm = new SkyeTekSerialComm("/dev/ttyS0",19200);
            int x = 0;
            Thread.sleep(200);
            while(x < 1000)
            {
                
                x++;
                //selectTag(byte tagType, byte flags, byte AFI, byte[] tagID, byte readerId)
                byte[] buffer = new byte[7];
                comm.sendRequest(SkyeTekMessaging.selectTag(SkyeTekMessaging.ISO15693, SkyeTekMessaging.SEL_TAG, (byte)0x00, buffer, (byte)0x00));
                System.out.println("Response"+Debug.debugBytes(comm.getNextResponse()));
                Thread.sleep(10);
            }
            
            
	    //	    try {Thread.sleep(1000);} catch (InterruptedException e) {}
	    //	    comm.sendRequest(SkyeTekMessaging.getReadSerialNumberCommand());
	    //    try {Thread.sleep(20000);} catch (InterruptedException e) {}

            System.out.println("thats all folks....");
            comm.shutdown();
        } catch (Exception ex) {
            ex.printStackTrace();
            System.out.println(ex);
        }
    }
    
}
     
