/*
 * SkyeTekTag.java
 *
 * Created on October 16, 2003, 2:06 PM
 */

package com.skyetek.reader;

import com.skyetek.comm.SkyeTekComm;
import com.skyetek.messaging.SkyeTekMessaging;
import com.skyetek.utility.SkyeTekException;
import com.skyetek.utility.Debug;
/**
 * This class creates a new rfid tag object.  Encapsulating an actual physical
 * RFID tag.  This class contains the methods for controlling the reader's
 * access to manipulate tag data including read, write and lock operations
 * on supported tags.
 */
public class SkyeTekTag {
    
    /* tag types */
    public static final byte AUTO_DETECT    = (byte)0x00;
    public static final byte ISO15693       = (byte)0x01;
    public static final byte I_CODE1        = (byte)0x02;
    public static final byte TAG_IT_HF      = (byte)0x03;
    public static final byte ISO14443A      = (byte)0x04;
    public static final byte ISO14443B      = (byte)0x05;
    public static final byte PICOTAG        = (byte)0x06;
    public static final byte GEMWAVE        = (byte)0x08;

    public static final String TAG_TYPE[] = {"",
					     "ISO15693",
					     "I_CODE1",
					     "TAG_IT_HF",
					     "ISO14443A",
					     "ISO14443B",
					     "PICOTAG",
					     "GEMWAVE"};

    /* tag data */
    public byte[] tagID = null;
    public byte tagType    = (byte)0;
    public byte bytesBlock = (byte)0;
    public byte[] data = null; 
    public int tagSize = 0;

    private SkyeTekComm comm = null;
    
    
  /**
   * Creates a new instance of SkyeTekTag which can then be used to read
   * or write to RFID tags in the field of the reader.  The type argument
   * must specify one of the constant RFID tags types supported by the reader
   * and defined in the SkyeTekTag class.  The ID specifier must specify
   * the unique tagID of a real world physical tag.
   *
   * @param  type    a tag type identifier specified in the SkyeTekTag class
   * @param  tagid   a unique RFID tagID usually obtained via a selectTag 
   * command    
   */    
    public SkyeTekTag(SkyeTekComm comm, byte type, byte[] ID) {
	this.comm = comm;
        
        tagType = type;
        tagID = ID;
         switch (tagType) {
                    case 1:  
                        //ISO15693
                        //second byte for manufacturer 2nd byte
                        //System.out.println(Debug.debugBytes(tag.tagID));
                        switch(tagID[1]){
                            case 2: tagSize = 64;   break; 
                            case 7: tagSize = 256;  break;
                            case 5:
                                if(tagID[2] == 2)
                                {
                                   tagSize = 232;
                                }
                                else
                                {
                                  tagSize = 1000;
                                }
                                break;       
                        }
                        break;
                    case 2:  
                        //I_CODE1
                        tagSize = 64;
                        break;
                    case 3:  
                        //TAG_IT_HF
                        tagSize = 32;
                        break;
                    case 4:  
                        //14443A, no reads and writes supported
                        break;
                        
                    case 5:  
                        //14443B, no reads or writes
                        break;
                    case 6:
                        //Picotag
                        //FILL THIS IN LATER
                        break;
                    case 7:
                        //Gemwave no reads and writes supported
                        break;
         }
        if( ((ID[0] == 0x60) && (ID[1] == 0x05) ) || (type == 0x06)) {
            bytesBlock = 0x08;
        } else {
            bytesBlock = 0x04;
        }
        
    }
    
  /**
   * This method appends the tag type onto the tag ID.
   */ 
    public String toString() {
	StringBuffer result = new StringBuffer();
	result.append(tagIdAsString());
	result.append("(");
	result.append(TAG_TYPE[tagType]);
	result.append(")");
	return result.toString();
    }

  /**
   * This creates a tagID string from a byte array returned by a SkyeTek 
   * reader.
   */ 
    public String tagIdAsString() {
	StringBuffer result = new StringBuffer();
        
	for ( int i = 0 ; i < tagID.length; i ++ ) {
             
	    int value = (0xff & tagID[i]);
	    if ( value < 0x0f ) {
		result.append("0");
		result.append(Integer.toHexString(value));
	    } else {
		result.append(Integer.toHexString(value));
	    }
	}
	return result.toString();
    }
    
  /**
   * This method returns the tag ID as a string.
   */ 
    public String getTagID() {
	return tagIdAsString();
    }
    
  /**
   * This method reads data from RFID tags in the RF field of the reader.
   * The startBlock argument must specify the first block to read tag data
   * from.  The numBlocks argument specifies the number of blocks to read
   * data from the tag starting from the starting block specified.
   *
   * @param   startBlock the first block of an RFID tag to read from
   * @param   numBlocks the number of blocks of data to read from    
   * 
   * @return  the unique reader response code as defined in SkyeTek Protocol
   */  
   public byte readTag(byte startBlock, byte numBlocks) throws SkyeTekException {
        byte[] responseBuffer = {(byte)0x02, (byte)0x07, (byte)0x22, (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, (byte)0x7C, (byte)0x9A};
        
        //generate command from SkyeteKMessaging
        byte[] commandBuffer = SkyeTekMessaging.readTag(tagType, tagID, startBlock, numBlocks);
         
	//Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
       
	byte responseCode = responseBuffer[0];

	if ( (responseCode == SkyeTekMessaging.READ_TAG_PASS) ||
	     (responseCode == SkyeTekMessaging.SELECT_TAG_PASS)) {
	    //Get Read Data
	    byte[] tmp = new byte[responseBuffer.length - 1];
	    for(int j = 1; j < (bytesBlock * numBlocks) + 1; j++)  {
		tmp[j - 1] = responseBuffer[j];
	    } 
        
	    data = tmp;
	} else {
	    throw new SkyeTekException(responseCode,"Error while reading tag data.");
	}
        
        return responseCode;
   }

  /**
   * This method writes data to the memory of an RFID tag in the RF field of 
   * the reader.  The startBlock argument must specify the first block to write 
   * tag data to.  The numBlocks argument specifies the number of blocks to 
   * write starting from the specified starting block.  This parameter is 
   *.directly effected by tag memory size and also size of data.
   * 
   * @param   startBlock the first block of tag memory to write to
   * @param   numBlocks the number of blocks to write to the tag memory
   * @param   data to write to tag memory
   * 
   * @return  the unique reader response code as defined in SkyeTek Protocol
   * 
   */  
   public byte writeTag(byte startBlock, byte numBlocks, byte[] data) throws SkyeTekException {
        byte[] responseBuffer = {(byte)0x02, (byte)0x07, (byte)0x22, (byte)0x22, (byte)0x22, (byte)0x22, (byte)0x08, (byte)0x7C, (byte)0x9A};
        
       //generate command from SkyeTekMessaging
        byte[] commandBuffer = SkyeTekMessaging.writeTag(tagType, tagID, startBlock, numBlocks, data);

        //Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
        byte responseCode = responseBuffer[0];
        
        if( (responseCode == SkyeTekMessaging.WRITE_TAG_PASS) || 
            (responseCode == SkyeTekMessaging.SELECT_TAG_PASS)) {
            
                    
        }
        else
        {
             throw new SkyeTekException(responseCode,"Error while writing tag Data."); 
        }
        
        return responseCode;
   }
  
  /**
   * This method locks specified blocks of memory in an RFID tag in the  
   * field of the reader.  The startBlock argument must specify the first block 
   * to lock.  The numBlocks argument specifies the number of blocks to 
   * lock starting from the specified starting block.  This parameter is 
   *.directly effected by tag memory size.
   * 
   * @param   startBlock the first block of tag memory to lock
   * @param   numBlocks the number of blocks of tag memory to lock 
   * 
   * @return  the unique reader response code as defined in SkyeTek Protocol
   * 
   */ 
   public byte lockTag(byte startBlock, byte numBlocks) throws SkyeTekException {
        byte[] responseBuffer = {(byte)0x02, (byte)0x04, (byte)0x22, (byte)0x08, (byte)0x7C, (byte)0x9A};
      
        //generate command from SkyeTekMessaging
        byte[] commandBuffer = SkyeTekMessaging.lockTag(tagType, tagID, startBlock, numBlocks);
        
        //Send command and receive response
	responseBuffer = comm.sendBlockingRequest(commandBuffer);
        
        return responseBuffer[0];
        
   }

   
}
