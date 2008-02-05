/*
 * SkyeTekReader.java
 *
 * Created on October 14, 2003, 5:33 PM
 */

package com.skyetek.reader;

import com.skyetek.comm.SkyeTekComm;
import com.skyetek.comm.SkyeTekCommImpl;
import com.skyetek.comm.SkyeTekSerialComm;
import com.skyetek.messaging.SkyeTekMessaging;
import com.skyetek.reader.SkyeTekTag;
import com.skyetek.utility.Debug;
import com.skyetek.utility.SkyeTekException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Collection;
import java.util.Properties;

/**
 *
 * @author  Chris Chan
 * @author  Ben Griffin
 */


/**
 * This class creates a new SkyeTek Reader object.  This class provides all 
 * the methods for changing SkyeTek reader parameters in both system and memory
 * and also for selecting tags in the field of the reader to read or write to.
 */
public class SkyeTekReader implements SkyeTekListener {
     
    /* memory types */
    public static final byte EEPROM         = (byte)0x00;
    public static final byte RAM            = (byte)0x01;
    
    /* tag types */
    public static final byte AUTO_DETECT    = (byte)0x00;
    public static final byte ISO15693       = (byte)0x01;
    public static final byte I_CODE1        = (byte)0x02;
    public static final byte TAG_IT_HF      = (byte)0x03;
    public static final byte ISO14443A      = (byte)0x04;
    public static final byte ISO14443B      = (byte)0x05;
    public static final byte PICOTAG        = (byte)0x06;
    public static final byte GEMWAVE        = (byte)0x06;
    
   /* tag command flags for select tag command*/
    public static final byte SEL_TAG                = (byte)0x20;
    public static final byte SEL_TAG_TID            = (byte)0x68;
    public static final byte SEL_TAG_INV            = (byte)0x22;
    public static final byte SEL_TAG_INV_LOOP       = (byte)0x23;
    public static final byte SEL_TAG_AFI            = (byte)0x30;
    public static final byte SEL_TAG_INV_AFI        = (byte)0x32;
    public static final byte SEL_TAG_INV_AFI_LOOP   = (byte)0x33;
    public static final byte SEL_TAG_LOOP           = (byte)0x29;     
    
    /* these variables contain the current state of the reader */
    public byte[] serialNumber = new byte[4];
    public byte[] firmware = new byte[2];
    public byte[] readerID = new byte[1];
    public byte[] portDir = new byte[1];
    public byte[] portVal = new byte[1];
  
    
    /* HashMap contains tags read from one of selectTag commands */
    public HashMap tags = new HashMap();
    public String recentTag;

    /* the communication mechanism */
    private SkyeTekComm comm = null;

    private boolean loop_mode = false;
    private byte [] loop_command = null;

    private boolean _debug = false;

    /** 
     * Creates a new instance of SkyeTekReader which can be used to read
     * or modify reader parameter such as serial number or baud rate, and also 
     * select tags to later read or write to.  An exception is thrown if a 
     * SkyeTek reader is not detected. 
     */
    public SkyeTekReader() throws SkyeTekException {
	initDebugFlag();
        // check if reader is connected and if not throw an exceptioin
	comm = new SkyeTekComm(new SkyeTekSerialComm());
	comm.addListener(this);
    }

    /** 
     * Creates a new instance of SkyeTekReader with a set of properties for
     * communication, such as baud rate and com port, which can be used to read
     * or modify reader parameter such as serial number or baud rate, and also 
     * select tags to later read or write to.  An exception is thrown if a 
     * SkyeTek reader is not detected. 
     *
     * @param   properties the specific properties of the comm implementation
     */
    public SkyeTekReader(Properties properties) throws SkyeTekException {
	initDebugFlag();
        // check if reader is connected and if not throw an exceptioin
	comm = new SkyeTekComm(new SkyeTekSerialComm(properties));
        comm.addListener(this);
    }

    /** 
     * This method nitializes the Debug Flag for Debugging
     */
    private void initDebugFlag() {
	if ( System.getProperty("com.skyetek.debug") != null ) {
	    _debug = true;
	}
    }

    /** shutdown the communication mechanism.
     **/
    public void close() {
	comm.shutdown();
	comm = null;
    }
    
    /** 
     * This method lists the number of unique tags in the table
     *
     * @return  a collection of tag IDs that have been added to the table
     */
    public Collection getTagList() {
	return tags.values();
    }
    
    /** 
     * This method clears table of all tag objects
     */
    public void clearTagList() {
	tags.clear();
    }
  
  /**
   * This method reads the unique serial number of a SkyeTek reader.
   *   
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
    public byte querySerialNumber() throws SkyeTekException{
        byte[] responseBuffer = {SkyeTekMessaging.READ_SYS_PASS, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x6A, (byte)0xC8};
        
        //generate command from SkyeTekMessaging
        byte[] commandBuffer = SkyeTekMessaging.getSerialNumber(RAM);
       
	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
	
	byte responseCode = responseBuffer[0];
	//Parse valid response
	if(responseCode == SkyeTekMessaging.READ_SYS_PASS) {
	    for(int i = 1; i < responseBuffer.length; i++) {
		serialNumber[i - 1] = responseBuffer[i];
	    }
	} else {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
	}
        
        return responseCode;
   }
   
  /**
   * This method reads the firmware version of a SkyeTek reader.
   *   
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte queryFirmware() throws SkyeTekException{
        byte[] responseBuffer = { SkyeTekMessaging.READ_SYS_PASS, (byte)0x00, (byte)0x1C, (byte)0x02, (byte)0x39};
        
        //generate command from SkyeTekMessaging
        byte[] commandBuffer = SkyeTekMessaging.getFirmware(RAM);
        
	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
        //Parse response
        if(responseCode == SkyeTekMessaging.READ_SYS_PASS) {
            for(int i = 1; i < responseBuffer.length; i++) {
               firmware[i - 1] = responseBuffer[i];
            }
        } else {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
        }
        
        return responseCode; 
   }
         
  /**
   * This method reads the reader ID from a SkyeTek reader.  The memType
   * argument must specify what location, system(RAM), or  memory(EEPROM) to 
   * read the reader ID from.
   *
   * @param   memType the location to read the reader ID from, either RAM or
   * EEPROM
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte queryReaderID(byte memType) throws SkyeTekException{
        byte[] responseBuffer = {SkyeTekMessaging.READ_SYS_PASS, (byte)0xFF, (byte)0x7C, (byte)0x9A};
    
        //generate command from SkyeTekMessaging
        byte[] commandBuffer = SkyeTekMessaging.getReaderID(memType);

	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
        //Parse response
        if((responseCode == SkyeTekMessaging.READ_SYS_PASS) || 
	   (responseCode == SkyeTekMessaging.READ_MEM_PASS)) {
            for(int i = 1; i < responseBuffer.length; i++) {
		readerID[i - 1] = responseBuffer[i];
            }
        } else {
	    throw new SkyeTekException(responseCode,
				       "SkyeTek Reader Error Condition");
        }
           
        return responseCode;   
   }
   
   
  /**
   * This method reads the general purpose I/O port direction of a SkyeTek
   * reader.  The memType argument must specify what location, system(RAM), or 
   * memory(EEPROM) read the port direction from.
   *
   * @param   memType the location to read the port direction from, either RAM
   * or EEPROM
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte queryPortDir(byte memType) throws SkyeTekException {
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x05, (byte)0x7C, (byte)0x9A};
           
        //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.getPortDir(memType);

	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
       //Parse Response
        if((responseCode == SkyeTekMessaging.READ_SYS_PASS) || 
	   (responseCode == SkyeTekMessaging.READ_MEM_PASS)) {
           for(int i = 1; i < responseBuffer.length; i++) {
               portDir[i - 1] = responseBuffer[i];
           }
        } else {
            throw new SkyeTekException(responseCode,
				       "SkyeTek Reader Error Condition");
        }
        
        return responseCode;         
   }
   
   
  
  /**
   * This method reads the general purpose I/O port value of a SkyeTek
   * reader.  The memType argument must specify what location, system(RAM), or 
   * memory(EEPROM) read the port value from.
   *
   * @param   memType the location to read the port value from, either RAM or
   * EEPROM
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte queryPortVal(byte memType) throws SkyeTekException{
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
           
        //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.getPortVal(memType);

	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
       //Parse Response
        if((responseCode == SkyeTekMessaging.READ_MEM_PASS) || 
	   (responseCode == SkyeTekMessaging.READ_SYS_PASS)) {
           for(int i = 1; i < responseBuffer.length; i++) {
               portVal[i - 1] = responseBuffer[i];
           }
        } else {
            throw new SkyeTekException(responseCode,
				       "SkyeTek Reader Error Condition");
        }
       
       return responseCode;            
   }
   
  /**
   * This method sets the reader ID value of a SkyeTek reader.  The memType 
   * argument must specify what location, system(RAM), or memory(EEPROM) to
   * write the new reader ID to.
   *
   * @param   memType the location to write the new reader ID to, either RAM or
   * EEPROM
   * @param   newID a new reader ID value to write to the reader
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte updateReaderID(byte memType, byte newID) throws SkyeTekException{
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
       
        //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.setReaderID(memType, newID);

	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
       //Parse Response
       if((responseCode == SkyeTekMessaging.WRITE_MEM_PASS) || 
	  (responseCode == SkyeTekMessaging.WRITE_SYS_PASS))  {
            readerID[0] = newID;
       } else {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
       }
        
       return responseCode;
   }
        
  /**
   * This method sets the baud rate value of a SkyeTek reader.  The memType 
   * argument must specify what location, system(RAM), or memory(EEPROM) to
   * write the new baud rate to.  The newBaud argument specifies the new 
   * baud rate of the reader.
   *
   * @param   memType the location to write the new reader ID to, either RAM or
   * EEPROM
   * @param   newBaud value of new baud rate to write to the reader
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte updateBaudRate(byte memType, byte newBaud) throws SkyeTekException {
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
       //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.setBaudRate(memType, newBaud);

       //Send command and receive response
       responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
       return responseBuffer[0];
   }
   
  /**
   * This method sets the port direction of the general purpose I/O ports
   * of a SkyeTek reader.  The memType argument must specify what location, 
   * system(RAM), or memory(EEPROM) to write the new port direction to.  The 
   * newDir argument specifies the new port direction value of the reader.
   *
   * @param   memType the location to write the new reader ID to, either RAM or
   * EEPROM
   * @param   newDir value of new port direction to write to the reader
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte updatePortDir(byte memType, byte newDir) throws SkyeTekException{
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
 
       //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.setPortDir(memType, newDir);
        
	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
       //Parse Response
       if((responseCode == SkyeTekMessaging.WRITE_MEM_PASS) || 
	  (responseCode == SkyeTekMessaging.WRITE_SYS_PASS))  {
            portDir[0] = newDir;
       } else {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
       }
       
       return responseCode;
   }
   
  /**
   * This method sets the port value of the general purpose I/O ports
   * of a SkyeTek reader.  The memType argument must specify what location, 
   * system(RAM), or memory(EEPROM) to write the new port value to.  The 
   * newVal argument specifies the new port value of the reader.
   *
   * @param   memType the location to write the new reader ID to, either RAM or
   * EEPROM
   * @param   newDir value to set SkyeTek reader GP/IO pins to
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte updatePortVal(byte memType, byte newVal) throws SkyeTekException{
       
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
       //clear hash table of any old tags
       tags.clear();
       //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.setPortVal(memType, newVal);

       //Send command and receive response
       responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
       byte responseCode = responseBuffer[0];
       //Parse Response
       if((responseCode == SkyeTekMessaging.WRITE_MEM_PASS) || 
	  (responseCode == SkyeTekMessaging.WRITE_SYS_PASS))  {
            portDir[0] = newVal;
       } else {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
       }
       
        return responseCode;
   }
   
  /**
   * This method finds RFID tags within the RF field of the reader.  This
   * method provides all support for the selectTag reader command including
   * inventory, loop mode, and selected state operation.  The tagType 
   * argument must specify the type of RFID tag as defined by the SkyeTek 
   * protocol and in this class.  The flags argument must specify the type of 
   * flags to send in the reader command as defined by the SkyeTek protocol and 
   * defined in this class.  The AFI argument must specify the AFI field of 
   * a particular tag to send in the reader command.  The tagID argument must
   * be an empty buffer of size 0 if a tag is to be selected not in the 
   * selected state.  
   *
   * @param   tagType the type of tag to select in the RF field of the reader
   * @param   flags the flags to send in the reader command specifying command
   * and parameters to send to the reader
   * @param   AFI a unique tag field specifier
   * @param   tagID a unique tag ID which can be either specified for selected
   * state operation or an empty buffer for finding unique tags in the field
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
    public byte selectTag(byte tagType, byte flags, byte AFI, byte[] tagID, byte readerId) throws SkyeTekException {
	byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
	int tagIdIndex;
      
	if(tagType == AUTO_DETECT)
	    tagIdIndex = 2;
	else
	    tagIdIndex = 1;
         recentTag = ""; 
	//generate command from SkyeTekMessaging
	byte[] commandBuffer = SkyeTekMessaging.selectTag(tagType, flags, AFI, tagID, readerId);
        //System.out.println("before sending response");
	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
       // System.out.println("after response");    
	//if ( _debug ) System.out.println("select tag response:"+Debug.debugBytes(responseBuffer));

	byte responseCode = responseBuffer[0];
	if((responseCode != SkyeTekMessaging.SELECT_TAG_PASS) && 
	   (responseCode != SkyeTekMessaging.SELECT_TAG_LOOP_ACTIVATE) &&
	   ( ((flags & SkyeTekMessaging.INV_F) == SkyeTekMessaging.INV_F ) && 
	     (responseCode != SkyeTekMessaging.SELECT_TAG_FAIL) ) ) {
	    throw new SkyeTekException(responseCode,
				       "SkyeTek Reader Error Condition");
	}
        
        if ( (flags & SkyeTekMessaging.LOOP_F) == SkyeTekMessaging.LOOP_F ) {
            
	    // activate listen mode
	    // For asynchronous operation
            // comm.setAsyncReadMode(true);
	    // loop_mode = true;
	    // loop_command = commandBuffer;
            
             while ( responseBuffer[0] != SkyeTekMessaging.SELECT_TAG_LOOP_CANCEL ){
               
                try { responseBuffer = comm.getNextResponse();  
                      
                } catch (SkyeTekException ex) {continue;}

		byte tag_type = tagType;
		if (tagType == AUTO_DETECT) {
		    tag_type = responseBuffer[1];
		}
		int id_length = responseBuffer.length - tagIdIndex;
		byte [] id = new byte[id_length];
                
		System.arraycopy(responseBuffer,tagIdIndex,id,0,id_length);
          //      System.out.println("Tag ID:"+Debug.debugBytes(id));
		SkyeTekTag tag = new SkyeTekTag(comm,tag_type,id);
		tags.put(id,tag);
            } 
        }
        else{
	    //for select tag do this once, for inventory tag, do it for each response until 94, for loop mode do it indefinitely
	    //parse response for ID and tagType if AUTODETECT and doing selectTag with no inventory or loop

	    while ( responseBuffer[0] == SkyeTekMessaging.SELECT_TAG_PASS ) {
		byte tag_type = tagType;
		if (tagType == AUTO_DETECT) {
		    tag_type = responseBuffer[1];
		}
		int id_length = responseBuffer.length - tagIdIndex;
		byte [] id = new byte[id_length];
              
                id_length = responseBuffer.length - tagIdIndex;
		id = new byte[id_length];

		System.arraycopy(responseBuffer,tagIdIndex,id,0,id_length);
                
                StringBuffer result = new StringBuffer();
        
                for( int i = 0 ; i < id.length; i ++ ) {
                    
                    int value = (0xff & id[i]);
                    if ( value < 0x0f ) {
                     result.append("0");
                     result.append(Integer.toHexString(value));
                    } else {
                        result.append(Integer.toHexString(value));
                    }
                    
                }
                result.toString();
                
                recentTag = new String(result);
            //    System.out.println(recentTag);
              //  System.out.println("Tag ID:"+ Debug.debugBytes(id));
		SkyeTekTag tag = new SkyeTekTag(comm,tag_type,id);
		tags.put(result,tag);
                if(flags == SkyeTekMessaging.SEL_TAG)
                {
                    break;
                    
                }
		responseBuffer = comm.getNextResponse();
           
	    }
		
	    //if ( _debug ) System.out.println("tag set size = " + tags.size());
	}
        
        if((responseCode != SkyeTekMessaging.SELECT_TAG_PASS) && 
           (responseCode != SkyeTekMessaging.SELECT_TAG_LOOP_ACTIVATE)) {
           throw new SkyeTekException(responseCode,
                                      "SkyeTek Reader Error Condition");
        }

        
        return responseCode;
    }

  /** 
   * This method executes a command with the flag loop_f set to true, and 
   * anticipates the 1c result indicating the the select tag looping has been
   * cancelled.
   */
    private void resendLoopCommand() throws SkyeTekException {
	byte [] commandBuffer = loop_command;
       byte[] responseBuffer = null;
       byte flags = commandBuffer[0];

       //Send command and receive response
       responseBuffer = comm.sendBlockingRequest(commandBuffer);

       if ( _debug ) System.out.println("select tag response:"+Debug.debugBytes(responseBuffer));
       if ( (flags & SkyeTekMessaging.LOOP_F) == SkyeTekMessaging.LOOP_F ) {
	   // activate listen mode
	   comm.setAsyncReadMode(true);
	   loop_mode = true;
	   loop_command = commandBuffer;
       }

	byte responseCode = responseBuffer[0];
        if((responseCode != SkyeTekMessaging.SELECT_TAG_PASS) && 
	   (responseCode != SkyeTekMessaging.SELECT_TAG_LOOP_ACTIVATE)) {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
        }
    }
    
  /**
   * This method executes a command with the flag loop_f set to false, and 
   * anticipates the 9c result indicating the the select tag looping has been
   * cancelled.
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte cancelLoop() throws SkyeTekException{
;
       byte[] responseBuffer;
 
       //generate command from SkyeTekMessaging
       byte[] commandBuffer = SkyeTekMessaging.getSerialNumber(RAM);
        
	comm.setAsyncReadMode(false);

	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
	byte responseCode = responseBuffer[0];
       //Parse Response
	if((responseCode != SkyeTekMessaging.SELECT_TAG_LOOP_CANCEL) &&
	   (responseCode != SkyeTekMessaging.READ_SYS_PASS)) {
	   throw new SkyeTekException(responseCode,
				      "SkyeTek Reader Error Condition");
       }
	loop_mode = false;
       return responseCode;
   }
   
  /**
   * This method finds an RFID tag within the RF field of the reader.  The 
   * selectOneTag method does not support inventory or anticollision 
   * reader operation modes.  The tagType argument must specify the type 
   * of RFID tag as defined by the SkyeTek protocol and defined in this class.  
   * a particular tag to send in the reader command.  The tagID argument must
   * be an empty buffer of size 0 if a tag is to be selected not in the 
   * selected state.  
   *
   * @param   tagType the type of tag to select in the RF field of the reader
   * @param   tagID a unique tag ID which can be either specified for selected
   * state operation or an empty buffer for finding unique tags in the field
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte selectOneTag(byte tagType, byte[] tagID) throws SkyeTekException{
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
       byte[] _tagID = tagID;
       byte _tagType = tagType;
      
       int tagIdIndex;
      
        if(tagType == AUTO_DETECT)
	    tagIdIndex = 2;
	else
	    tagIdIndex = 1;
     
       //clear hash table of any old tags
       tags.clear();
       
       //generate command from SkyeTekMessaging
      
       byte[] commandBuffer = SkyeTekMessaging.selectOneTag(tagType, tagID);
       
        
       	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        System.out.println("response to Select Tag no TID:"+Debug.debugBytes(responseBuffer));
        
        
        //Parse out tag ID and tag type from response
        byte responseCode = responseBuffer[0];
        int id_length = responseBuffer.length - tagIdIndex;
        if(tagType == AUTO_DETECT)
            _tagType = responseBuffer[1];
        byte [] id = new byte[id_length];
        System.arraycopy(responseBuffer,tagIdIndex,id,0,id_length);
        System.out.println("Tag ID:"+Debug.debugBytes(id));
        
        //Create new tag object and store
        SkyeTekTag tag = new SkyeTekTag(comm,_tagType,id);
        tags.put(id,tag);
        return responseCode;
       
        
   }

   

  /**
   * This method finds multiple RFID tags within the RF field of the reader.  
   * The tagType argument must specify the type of RFID tag as defined by 
   * the SkyeTek protocol and defined in this class.  The AFI argument must 
   * specify the AFI field of a particular tag to send in the reader command.
   * The tagID argument must be an empty buffer of size 0 if a tag is to be 
   * selected not in the selected state.  The loop argument specifies the 
   * operation mode of the reader where true invokes the reader's loop
   * mode operation.
   *
   * @param   tagType the type of tag to select in the RF field of the reader
   * @param   AFI a unique tag field specifier
   * @param   tagID a unique tag ID which can be either specified for selected
   * state operation or an empty buffer for finding unique tags in the field
   * @param   loop specifies whether the reader is operating in loop mode
   *
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */ 
   public byte inventoryTag(byte tagType, byte[] AFI, byte[] tagID, boolean loop) throws SkyeTekException {
       int tagIdIndex;
       byte[] responseBuffer = {(byte)0x02, (byte)0x04, SkyeTekMessaging.READ_SYS_PASS, (byte)0x08, (byte)0x7C, (byte)0x9A};
       byte[] _tagID = tagID;
       byte _tagType = tagType;

       //clear hash table of any old tags
       tags.clear();
      
	if(tagType == AUTO_DETECT)
	    tagIdIndex = 2;
	else
	    tagIdIndex = 1;
     
	//generate command from SkyeTekMessaging
	byte[] commandBuffer = SkyeTekMessaging.inventoryTag(tagType, AFI, tagID, (byte)0xff, loop);
        
	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);

	if ( _debug ) System.out.println("select tag response:"+Debug.debugBytes(responseBuffer));

	byte responseCode = responseBuffer[0];
	if((responseCode != SkyeTekMessaging.SELECT_TAG_PASS) && 
	   (responseCode != SkyeTekMessaging.SELECT_TAG_LOOP_ACTIVATE) &&
	   (responseCode != SkyeTekMessaging.SELECT_TAG_FAIL) ) {
	    throw new SkyeTekException(responseCode,
				       "SkyeTek Reader Error Condition");
	}

        if ( loop ) {
	    // activate listen mode
	    // comm.setAsyncReadMode(true);
	    // loop_mode = true;
	    // loop_command = commandBuffer;
            
            while ( responseBuffer[0] != SkyeTekMessaging.SELECT_TAG_LOOP_CANCEL ){
               
                try { responseBuffer = comm.getNextResponse();  
                      
                } catch (SkyeTekException ex) {continue;}
                        
		byte tag_type = tagType;
		if (tagType == AUTO_DETECT) {
		    tag_type = responseBuffer[1];
		}
		int id_length = responseBuffer.length - tagIdIndex;
		byte [] id = new byte[id_length];
                
		System.arraycopy(responseBuffer,tagIdIndex,id,0,id_length);
                System.out.println("Tag ID:"+Debug.debugBytes(id));
		SkyeTekTag tag = new SkyeTekTag(comm,tag_type,id);
		tags.put(id,tag);
         	
	} 
        }else {
	    //for select tag do this once, for inventory tag, do it for each response until 94, for loop mode do it indefinitely
	    //parse response for ID and tagType if AUTODETECT and doing selectTag with no inventory or loop

	    while ( (responseBuffer[0] == SkyeTekMessaging.SELECT_TAG_PASS) ) {
                
		byte tag_type = tagType;
		if (tagType == AUTO_DETECT) {
		    tag_type = responseBuffer[1];
		}
		int id_length = responseBuffer.length - tagIdIndex;
		byte [] id = new byte[id_length];
		System.arraycopy(responseBuffer,tagIdIndex,id,0,id_length);
		   
		SkyeTekTag tag = new SkyeTekTag(comm,tag_type,id);
		tags.put(id,tag);
		responseBuffer = comm.getNextResponse();
                
	    }
		
	    if ( _debug ) System.out.println("tag set size = " + tags.size());
	}
        return responseCode;
   }

  /**
   * Performs read operations on all tags in a table and updates their data
   * variable.
   *
   * @return  the updated tag collecetion
   */   
    public Collection updateTags() {
	try {
	Iterator tagIterator = tags.values().iterator();
	while ( tagIterator.hasNext() ) {
	    SkyeTekTag tag = (SkyeTekTag)tagIterator.next();
	    if ( _debug ) System.out.println("reading tag: " + tag);
	    try { Thread.sleep(100);  } catch (InterruptedException ex) {}
            byte[] tmp = {(byte)0xFF, (byte)0xFF, (byte)0xFF, (byte)0xFF};
            byte response = tag.readTag((byte)0, (byte)1);
            // byte response = tag.writeTag((byte)0, (byte)1, tmp);
	    if ( _debug ) System.out.println("Tag Data:"+Debug.debugBytes(tag.tagID));
            
	}
	} catch (SkyeTekException ex) {
	    ex.printStackTrace();
	}
	return tags.values();
    }

  /**
   * For notifying savant software 
   */
    public void notify(SkyeTekTag tag){
	if ( _debug ) System.out.println("Got tag = " +tag);
	try {
	    boolean old_loop_mode = loop_mode;
	    if ( loop_mode ) {
		if ( _debug ) System.out.println("(tag):cancelLoop()");
		cancelLoop();
	    }
	    byte response = tag.readTag((byte)0,(byte)2);
	    if ( _debug ) System.out.println("Tag Data:"+Debug.debugBytes(tag.data));
	    if ( old_loop_mode ) {
		resendLoopCommand();
	    }
	} catch (SkyeTekException ex) {
	    ex.printStackTrace();
	}
    }


    /** Test Harness **/
    /* Example Usage */
    public static void main(String args[]) {
	SkyeTekReader reader = null;
	Properties props = new Properties();
        byte [] empty = new byte[0];
	props.setProperty("SkyeTekImpl","com.skyetek.comm.SkyeTekSerialComm");
	System.setProperty("com.skyetek.debug","true");
      
        try {
	    reader = new SkyeTekReader(props);            
	    Thread.sleep(20);
      
	    System.out.println("Started reader...");
	    System.out.println("querySerialNumber()");
	    reader.querySerialNumber();
	    System.out.println("queryFirmware() ");
	    reader.queryFirmware();
	    System.out.println("queryReaderID(RAM)");
	    reader.queryReaderID(RAM);
	    
            reader.updateReaderID(RAM, (byte)0xFF);
            
            System.out.println("queryPortDir(RAM)");
	    reader.queryPortDir(RAM);
            
	    System.out.println("queryPortVal(RAM)");
	    reader.queryPortVal(RAM);
	    
	    System.out.println("SerialNumber:"+Debug.debugBytes(reader.serialNumber));
	    System.out.println("Firmware:"+Debug.debugBytes(reader.firmware));
	    System.out.println("ReaderID:"+Debug.debugBytes(reader.readerID));
	    System.out.println("PortDir:"+Debug.debugBytes(reader.portDir));
	    System.out.println("PortVal:"+Debug.debugBytes(reader.portVal));
 
	    
                 reader.inventoryTag(SkyeTekMessaging.ISO15693,
			     empty,
			     new byte[0],
				false); 
	    int count = 0;
	   while ( count++ < 1000 ) 
	   {
             
                /* Insert delay here.  Recommended delay of > 6ms as reader can 
                 * only accept 1 host request every 5ms */
               // Thread.sleep(6);
                /*reader.selectTag(SkyeTekMessaging.AUTO_DETECT,
	    			     SkyeTekMessaging.SEL_TAG,
	    			     (byte)0x00,
	    			     new byte[0],
	    			     (byte)0xFF);*/
            /*  reader.inventoryTag(SkyeTekMessaging.ISO15693,
			     empty,
                                new byte[0],
				false);*/
               //reader.updateTags();
            }
            
            System.out.println("thats all folks....");
        } catch (Exception ex) {
            ex.printStackTrace();
            System.out.println(ex);
        }
	reader.close();
    }
}
 
