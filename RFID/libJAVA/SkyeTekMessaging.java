/**
 *  This class implements the commands necessary to communicate with the skyetek reader
 **/

package com.skyetek.messaging;

import com.skyetek.utility.CRC16;

/**
 * This class creates a new SkyeTek Reader object.  This class provides methods
 * for generating the messages or command set specific to the SkyeTek protocol.  
 * The messages generated are used in the SkyeTekReader class in order to 
 * control the SkyeTek family of readers, including reader parameters, tags, 
 * and external peripheral functionality.
 */
public class SkyeTekMessaging {
   
    /* reader commands */
    public static final byte SELECT_TAG     = (byte)0x14;
    public static final byte READ_MEM       = (byte)0x21;
    public static final byte READ_SYS       = (byte)0x22;
    public static final byte READ_TAG       = (byte)0x24;
    public static final byte WRITE_MEM      = (byte)0x41;
    public static final byte WRITE_SYS      = (byte)0x42;    
    public static final byte WRITE_TAG      = (byte)0x44;     
    
    /* memory types */
    public static final byte EEPROM         = (byte)0x00;
    public static final byte RAM            = (byte)0x01;

    /* command flags */
    public static final byte CRC_ONLY       = (byte)0x20;

    /* *********************** */
    /* command specific fields */
    /* *********************** */
     
    /* READER COMMANDS */
    /* reader parameter locations */
    public static final byte SERIAL_NUM     = (byte)0x00;
    public static final byte FIRM_VERS      = (byte)0x01;
    public static final byte READER_ID      = (byte)0x02;
    public static final byte BAUD_RATE      = (byte)0x03;
    public static final byte OP_MODE        = (byte)0x04;
    public static final byte LED            = (byte)0x05;
    public static final byte BEEPER         = (byte)0x06;
    public static final byte PORT_DIR       = (byte)0x07;
    public static final byte PORT_VAL       = (byte)0x08;
    
    /* peripheral parameters */
    public static final byte LCD            = (byte)0x0A;
    public static final byte EXT_MEM        = (byte)0x0C;
   
    /* baud rates */
    public static final byte BR_9600        = (byte)0x00;
    public static final byte BR_19200       = (byte)0x01;
    public static final byte BR_38400       = (byte)0x02;
    public static final byte BR_57600       = (byte)0x03;
   
    /* TAG COMMANDS */
   
    /* tag command flags for select tag command*/
    public static final byte SEL_TAG                = (byte)0x20;
    public static final byte SEL_TAG_TID            = (byte)0x68;
    public static final byte SEL_TAG_INV            = (byte)0x22;
    public static final byte SEL_TAG_INV_LOOP       = (byte)0x23;
    public static final byte SEL_TAG_AFI            = (byte)0x30;
    public static final byte SEL_TAG_INV_AFI        = (byte)0x32;
    public static final byte SEL_TAG_INV_AFI_LOOP   = (byte)0x33;
    public static final byte SEL_TAG_LOOP           = (byte)0x29;        
    
    /* tag command flags for reading and writing */
    public static final byte RW_TAG_SS      = (byte)0x28;
    public static final byte RW_TAG_TID     = (byte)0x60;
    public static final byte LOCK_TAG_SS    = (byte)0x2C;
    public static final byte LOCK_TAG_TID   = (byte)0x64;
    
    /* tag types */
    public static final byte AUTO_DETECT    = (byte)0x00;
    public static final byte ISO15693       = (byte)0x01;
    public static final byte I_CODE1        = (byte)0x02;
    public static final byte TAG_IT_HF      = (byte)0x03;
    public static final byte ISO14443A      = (byte)0x04;
    public static final byte ISO14443B      = (byte)0x05;
    public static final byte PICOTAG        = (byte)0x06;
    public static final byte GEMWAVE        = (byte)0x06;

    /* reader response codes */
    public static final byte SELECT_TAG_PASS          = (byte)0x14;
    public static final byte SELECT_TAG_LOOP_ACTIVATE = (byte)0x1C;
    public static final byte SELECT_TAG_FAIL          = (byte)0x94;
    public static final byte SELECT_TAG_LOOP_CANCEL   = (byte)0x9C;

    public static final byte READ_MEM_PASS  = (byte)0x21;
    public static final byte READ_SYS_PASS  = (byte)0x22;
    public static final byte READ_TAG_PASS  = (byte)0x24;
    public static final byte READ_MEM_FAIL  = (byte)0xA1;
    public static final byte READ_SYS_FAIL  = (byte)0xA2;
    public static final byte READ_TAG_FAIL  = (byte)0xA4;

    public static final byte WRITE_MEM_PASS = (byte)0x41;
    public static final byte WRITE_SYS_PASS = (byte)0x42;    
    public static final byte WRITE_TAG_PASS = (byte)0x44;     
    public static final byte WRITE_MEM_FAIL = (byte)0xC1;
    public static final byte WRITE_SYS_FAIL = (byte)0xC2;    
    public static final byte WRITE_TAG_FAIL = (byte)0xC4;     

    public static final byte NON_ASCII_REQUEST         = (byte)0x80;     
    public static final byte BAD_CRC                   = (byte)0x81;     
    public static final byte FLAGS_DONT_MATCH_COMMAND  = (byte)0x82;     
    public static final byte FLAGS_DONT_MATCH_TAG_TYPE = (byte)0x83;     
    public static final byte UNKNOWN_COMMAND           = (byte)0x84;     
    public static final byte UNKNOWN_TAG_TYPE          = (byte)0x85;     
    public static final byte INVALID_STARTING_BLOCK    = (byte)0x86;     
    public static final byte INVALID_NUMBER_OF_BLOCKS  = (byte)0x87;     
    public static final byte INVALID_MESSAGE_LENGTH    = (byte)0x88;     

    /* flags */
    public static final byte LOOP_F       = (byte)0x01;
    public static final byte INV_F        = (byte)0x02;
    public static final byte LOCK_F       = (byte)0x04;
    public static final byte RF_F         = (byte)0x08;
    public static final byte AFI_F        = (byte)0x10;
    public static final byte CRC_F        = (byte)0x20;
    public static final byte TID_F        = (byte)0x40;
    public static final byte RID_F        = (byte)0x80;


    public static CRC16 crcCheck = new CRC16();

     /* Constructor */
    public SkyeTekMessaging() {
        // check if reader is connected and if not throw an exceptioin
    }

    
    /* START STANDARD READER COMMANDS */
    
    
  /**
   * This method generates the unique SkyeTek protocol message to read the 
   * serial number of a SkyeTek reader.   The memType
   * argument must specify what location, system(RAM), or  memory(EEPROM) to 
   * read the serial number from.
   *
   * @param   memType the memory location to read the serial number from
   *   
   * @return  buffer containing the generated message
   */ 
    public static byte[] getSerialNumber(byte memType) {
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[7];
        
        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = READ_SYS;
        else if(memType == EEPROM)
            buffer[i++] = READ_MEM;
        /* starting block */
        buffer[i++] = SERIAL_NUM;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* msg Length */
        buffer[0] = (byte)(i + 1);        
        
        /* calculate CRC */
	appendCRC(buffer,i);
        
        return buffer; 
    }

    /** Calculates the CRC for the given buffer and size, then sets the 
     * following two bytes to the 16 bit crc.
     **/
    protected static int appendCRC(byte [] buffer, int i) {
        crcCheck.reset();
        crcCheck.update(buffer, i);
        buffer[i++] = (byte)(crcCheck.value >> 8);
        buffer[i++] = (byte)(crcCheck.value & 0x00FF);
	return i;
    }
     
  /**
   * This method generates the unique SkyeTek protocol message to read the 
   * firmware of a SkyeTek reader.  The memType argument must specify what
   * location, system(RAM), or  memory(EEPROM) to read the firmware from.
   *   
   * @param   memType the memory location to read the firmware from
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] getFirmware(byte memType) {
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[7];
        
        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = READ_SYS;
        else if(memType == EEPROM)
            buffer[i++] = READ_MEM;
        /* starting block */
        buffer[i++] = FIRM_VERS; 
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* msg Length */
        buffer[0] = (byte)(i + 1);
        
        /* check CRC */
	appendCRC(buffer,i);
       
        return buffer;
        
    }
 
  /**
   * This method generates the unique SkyeTek protocol message to read the 
   * reader ID of a SkyeTek reader.  The memType argument must specify what
   * location, system(RAM), or  memory(EEPROM) to read the reader ID from.
   *   
   * @param   memType the memory location to read the reader ID from
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] getReaderID(byte memType) {
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[7];
        
        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = READ_SYS;
        else if(memType == EEPROM)
            buffer[i++] = READ_MEM;
        /* starting block */
        buffer[i++] = READER_ID;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* msg Length */
        buffer[0] = (byte)(i + 1);
        
        /* check CRC */
	appendCRC(buffer,i);

        return buffer;
        
    }
    
 
  /**
   * This method generates the unique SkyeTek protocol message to write the 
   * reader ID of a SkyeTek reader.  The memType argument must specify what
   * location, system(RAM), or memory(EEPROM) to write the reader ID to.  The 
   * readerID argument must specify the new reader ID to write to the reader.
   *   
   * @param   memType the memory location to read the reader ID from
   * @param   readerID the new reader ID to write to the reader
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] setReaderID(byte memType, byte readerID) {
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[8];
        
        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = WRITE_SYS;
        else if(memType == EEPROM)
            buffer[i++] = WRITE_MEM;
        /* starting block */
        buffer[i++] = READER_ID;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* reader ID */
        buffer[i++] = readerID;
        /* msg Length */
        buffer[0] = (byte)(i + 1);
          
        /* check CRC */
	appendCRC(buffer,i);

        return buffer;      
    }
  
  /**
   * This method generates the unique SkyeTek protocol message to write the 
   * baud rate of a SkyeTek reader.  The memType argument must specify what
   * location, system(RAM), or memory(EEPROM) to write the baud rate to.  The 
   * baudRate argument must specify the new baud rate to write to the reader.
   *   
   * @param   memType the memory location to read the reader ID from
   * @param   baudRate the new baud rate to write to the reader
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] setBaudRate(byte memType, byte baudRate) {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[8];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = WRITE_SYS;
        else if(memType == EEPROM)
            buffer[i++] = WRITE_MEM;
        /* starting block */
        buffer[i++] = BAUD_RATE;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* baud rate */
        buffer[i++] = baudRate;
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	appendCRC(buffer,i);
        
        return buffer;      
    }
  
  /**
   * This method generates the unique SkyeTek protocol message to read the 
   * port direction of a SkyeTek reader.  The memType argument must specify 
   * what location, system(RAM), or  memory(EEPROM) to read the port direction
   * from.
   *   
   * @param   memType the memory location to read the reader ID from
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] getPortDir(byte memType) {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[7];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = READ_SYS;
        else if(memType == EEPROM)
            buffer[i++] = READ_MEM;
        /* starting block */
        buffer[i++] = PORT_DIR;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	appendCRC(buffer,i);
           
        return buffer;      
    }     
    
    
  /**
   * This method generates the unique SkyeTek protocol message to write the 
   * port direction of a SkyeTek reader.  The memType argument must specify 
   * what location, system(RAM), or memory(EEPROM) to write the port direction
   * to.  The direction argument must specify the new port direction of the 
   * GP I/O pins of the reader.
   *   
   * @param   memType the memory location to read the reader ID from
   * @param   direction the new port direction to write to the reader
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] setPortDir(byte memType, byte direction) {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[8];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = WRITE_SYS;
        else if(memType == EEPROM)
            buffer[i++] = WRITE_MEM;
        /* starting block */
        buffer[i++] = PORT_DIR;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* set direction */
        buffer[i++] = direction;
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	appendCRC(buffer,i);
       
        return buffer;      
    }  
    
  /**
   * This method generates the unique SkyeTek protocol message to read the 
   * port value of a SkyeTek reader.  The memType argument must specify what
   * location, system(RAM), or  memory(EEPROM) to read the port value from.
   *   
   * @param   memType the memory location to read the port value from
   *
   * @return  buffer containing the generated message
   */       
    public static byte[] getPortVal(byte memType) {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[7];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = READ_SYS;
        else if(memType == EEPROM)
            buffer[i++] = READ_MEM;
        /* starting block */
        buffer[i++] = PORT_VAL;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* msg Length */
        buffer[0] = (byte)(i + 1);
        
        /* check CRC */
	appendCRC(buffer,i);
        
        return buffer;      
    }     
    
      
  /**
   * This method generates the unique SkyeTek protocol message to write the 
   * port value of a SkyeTek reader.  The memType argument must specify what
   * location, system(RAM), or memory(EEPROM) to write the port value to.  The 
   * value argument must specify the new GP I/O port value to write to the
   * reader.
   *   
   * @param   memType the memory location to read the reader ID from
   * @param   value the new port value to write to the reader
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] setPortVal(byte memType, byte value) {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[8];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
       /* command */
        if(memType == RAM)
            buffer[i++] = WRITE_SYS;
        else if(memType == EEPROM)
            buffer[i++] = WRITE_MEM;
        /* starting block */
        buffer[i++] = PORT_VAL;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* set direction */
        buffer[i++] = value;
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	appendCRC(buffer,i);
  
        return buffer;      
    }    
  
  /**
   * This method generates the unique SkyeTek protocol command to put the 
   * reader in sleep mode.  
   *
   * @return  buffer containing the generated message
   */   
    public static byte[] sleep() {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[8];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
        /* command */
        buffer[i++] = WRITE_SYS;
        /* starting block */
        buffer[i++] = OP_MODE;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* put reader to sleep */
        buffer[i++] = 0x00;
        /* msg Length */
        buffer[0] = (byte)(i + 1);
        
        /* check CRC */
	appendCRC(buffer,i);
        
        return buffer;  
    }        
    
  /**
   * This method generates the unique SkyeTek protocol command to wake the 
   * reader from sleep mode.  
   *
   * @return  buffer containing the generated message
   */   
    public static byte[] wake() {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[8];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
        /* command */
        buffer[i++] = WRITE_SYS;
        /* starting block */
        buffer[i++] = OP_MODE;
        /* number of blocks to read */
        buffer[i++] = 0x01;
        /* wake reader */
        buffer[i++] = (byte)0xFF;
      
        /* msg Length */
        buffer[0] = (byte)(i + 1);
        
        /* check CRC */
	appendCRC(buffer,i);

        return buffer;  
    }        
   
  /**
   * This method generates the unique SkyeTek protocol message to write to the 
   * extended memory of a SkyeTek reader.  The startAddr argument must specify
   * the first location of extended memory to write to.  The data argument
   * must specify the data to write to extended memory. 

   * @param   startAddr the starting extended memory location to write to
   * @param   data the data to write to extended memory
   *
   * @return  buffer containing the generated message
   */  
    public static byte[] writeExtMem(short startAddr, byte[] data) {
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80];

        /* command format: flags|command|startblock|numblocks|crc */    
        /* msg length */
        buffer[i++] = CRC_ONLY;
        /* command */
        buffer[i++] = WRITE_SYS;
        /* starting block */
        buffer[i++] =  EXT_MEM;
        /* number of blocks to write */
        buffer[i++] = 0x01;
        /* start addr */
        buffer[i++] = (byte)(startAddr >> 8);
        buffer[i++] = (byte)(startAddr & 0x00FF);
        /* bytes to write */
        buffer[i++] = (byte)data.length;
        /* data */       
        for(int j = 0; j < data.length; j++)
        {
            buffer[i++] = data[j];
        }
 
        /* msg Length */
        buffer[0] = (byte)(i + 1);
        
        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    }        
    
  /**
   * This method generates the unique SkyeTek protocol message to read from the 
   * extended memory of a SkyeTek reader.  The startAddr argument must specify
   * the first location of extended memory to read from.  The numBytes argument
   * must specify the number of bytes of data to read from extended memory.

   * @param   startAddr the starting extended memory location to write to
   * @param   numBytes the amount of data in bytes to read from extended memory
   *
   * @return  buffer containing the generated message
   */  
    public static byte[] readExtMem(short startAddr, byte numBytes) {    
    
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80]; 
    
        /* command format: flags|command|startblock|numblocks|crc */
        /* set flags */
        buffer[i++] = CRC_ONLY;
        /* set command */
        buffer[i++] = READ_SYS;
        /* set starting block */
        buffer[i++] = EXT_MEM;
        /* set number blocks to 1 */
        buffer[i++] = 0x01;

        /* see what happens passing 7ff and 07ff */
        buffer[i++] = (byte)(startAddr >> 8);
        buffer[i++] = (byte)(startAddr & 0x00FF);
        /* set number of bytes to read */
        buffer[i++] = numBytes;
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    }

  /**
   * This method generates the unique SkyeTek protocol message to write to the 
   * LCD of a SkyeTek reader.  The lineNum argument specifies the line number 
   * or row number to write to on the LCD.  The LCDLen argument specifies the   
   * the length of each line in the LCD determined by the LCD hardware.  The 
   * data argument must specify the data to write to the LCD. 
   * 
   * @param   lineNum the starting extended memory location to write to
   * @param   LCDLen the data to write to extended memory
   * @param   data the specific data to write to the LCD of the reader
   *
   * @return  buffer containing the generated message
   */  
    public static byte[] writeLCD(byte lineNum, byte LCDLen, byte[] data) {        
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80];     
        
        /* command format: flags|command|startblock|numblocks|crc */
        /* set flags */
        buffer[i++] = CRC_ONLY;
        /* set command */
        buffer[i++] = WRITE_SYS;
        /* set starting block */
        buffer[i++] = LCD;
         /* set number blocks to 1 */
        buffer[i++] = 0x01;
        /* set line width */
        buffer[i++] = LCDLen;
        /* set line number to write to */
        buffer[i++] = lineNum;
        /* set data to write to LCD */
        for(int j = 0; j < data.length; j++)
        {
            buffer[i++] = data[j];
        }

        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
     
    }
    
  /**
   * This method generates the unique SkyeTek protocol message to select a tag 
   * in the RF field of SkyeTek reader.  This method provides all support for
   * the selectTag reader command including inventory, loop mode, and selected
   * state operation.  The tagType argument must specify the type of RFID tag
   * as defined by the SkyeTek protocol and in this class.  The flags argument 
   * must specify the type of flags to send in the reader command as defined by
   * the SkyeTek protocol and defined in this class.  The AFI argument must 
   * specify the AFI field of a particular tag to select.  The tagID argument
   * must be an empty buffer of size 0 if a tag is to be selected not in the 
   * selected state.  
   *
   * @param   tagType the type of tag to select in the RF field of the reader
   * @param   flags the flags to send in the reader command specifying command
   * and parameters to send to the reader
   * @param   AFI a unique tag field specifier
   * @param   tagID a unique tag ID which can be either specified for selected
   * state operation or an empty buffer for finding unique tags in the field
   *
   * @return  buffer containing the generated message
   */  
    public static byte[] selectTag(byte tagType, byte flags, byte AFI, byte[] tagID, byte readerId){
        
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80];    
        
         /* command format: flags|command|startblock|numblocks|crc */
        /* set flags */
        buffer[i++] = flags;
        /* set command */
        buffer[i++] = SELECT_TAG;

	if ( (flags & 0x80) != 0 ) {
	    buffer[i++] = readerId;
	}

        /* set tag type */
        buffer[i++] = tagType;
        
        /* check if tag ID flag was set and adds tag ID to select tag command */
    	if((flags & 0x40) != 0)
        {
            if(tagType == TAG_IT_HF)
            {
                for(i = 4; i < 8; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
            else if(tagType == GEMWAVE)
            {	
                for(i = 4; i < 14; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
            else 
            {
                for(i = 4; i < 12; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
	}
        /* see if AFI flag is set and include if appropriate */
        if(((flags == SEL_TAG_AFI) || (flags == SEL_TAG_INV_AFI)  || (flags == SEL_TAG_INV_AFI_LOOP)))
        {
		buffer[i++] = AFI;
	}

        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    
    }
    
    /* This method generates the unique SkyeTek protocol message to select a
     * tag in the RF field of the reader.  The selectOneTag method does not 
     * generate a command for anticollision or inventory mode operation.  The 
     * tagType argument must specify the type of RFID tag as defined by the 
     * SkyeTek protocol and defined in this class.  The tagID argument must be
     * an empty buffer of size 0 if a tag is to be selected no in the selected
     * state.
     *
     * @param   tagType the type of tag to select in the RF field of the reader
     * @param   tagID a unique tag ID which can be either specified for selected
     * state operation or an empty buffer for finding unique tags in the field
     *
     * @return  buffer containing the generated message
     */  
    public static byte[] selectOneTag(byte tagType, byte[] tagID)
    {
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80]; 
        byte flags;
        
        /* command format: flags|command|startblock|numblocks|crc */
        /* set flags */
        if(tagID.length == 0)
	{	
            /* tag ID not provided, rf_f = 0, no selected state, tid_f = 0 no Tid */
            flags  = SEL_TAG;
	}
        else
	{
            /* tag ID provided, rf_f = 1 for selected state, tid_f = 1 for Tid */
            flags = SEL_TAG_TID;
	}
        /* set flags */
        buffer[i++] = flags;
        /* set command */
        buffer[i++] = SELECT_TAG;
        /* set tag type */
        buffer[i++] = tagType;
        
         /* set tagID */
	if((flags & 0x40) != 0)
        {
            if(tagType == TAG_IT_HF)
            {
                for(i = 4; i < 8; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
            else if(tagType == GEMWAVE)
            {	
                for(i = 4; i < 14; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
            else 
            {


                for(i = 4; i < 12; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
	
        }
        
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    }
    
  /**
   * This method generates the unique SkyeTek protocol message to select tags
   * in inventory mode operation of the reader.  The loop mode command can also 
   * be generated.  The tagType argument must specify the type of RFID tag as 
   * defined by the SkyeTek protocol and defined in this class.  The AFI 
   * argument must specify the AFI field of a particular tag to send in the 
   * reader command.  The tagID argument must be an empty buffer of size 0 if a 
   * tag is to be selected not in the selected state.  The loop argument 
   * specifies the operation mode of the reader where true invokes the reader's 
   * loop mode operation.
   *
   * @param   tagType the type of tag to select in the RF field of the reader
   * @param   AFI a unique tag field specifier
   * @param   tagID a unique tag ID which can be either specified for selected
   * state operation or an empty buffer for finding unique tags in the field
   * @param   loop specifies whether the reader is operating in loop mode
   *
   * @return  buffer containing the generated message
   */ 
    public static byte[] inventoryTag(byte tagType, byte[] AFI, byte[] tagID, byte readerId, boolean loop){
    
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80]; 
        byte flags;
        
        if(AFI.length == 0) {
            if(loop) {
                flags = SEL_TAG_INV_LOOP;	
            } else {
                flags = SEL_TAG_INV;
            }
	} else {
            if(loop) {
                flags = SEL_TAG_INV_AFI;
            }            else            {
                flags = SEL_TAG_INV_AFI_LOOP;
            }
	}
  
        buffer[i++] = flags;
	/* set command */
	buffer[i++] = SELECT_TAG;
	/* set reader ID */
	if ( readerId != (byte)0xff ) {
	    buffer[i++] = readerId;
	}

	/* set tag type */
	buffer[i++] = tagType;
	/* set tag ID */
	
	/* Checks to see if the tag ID flag was set and adds tag ID to command to be sent*/
	if((flags & TID_F) == TID_F) {
            if(tagType == TAG_IT_HF) {
                for(i = 4; i < 8; i++) {
                    buffer[i] = tagID[i - 4];
                }
            } else if(tagType == GEMWAVE) {	
                for(i = 4; i < 14; i++) {
                    buffer[i] = tagID[i - 4];
                }
            } else {
                for(i = 4; i < 12; i++) {
                    buffer[i] = tagID[i - 4];
                }
            }
	}

	/* set AFI */
	if (((flags == SEL_TAG_AFI) || 
	     (flags == SEL_TAG_INV_AFI)  || 
	     (flags == SEL_TAG_INV_AFI_LOOP))) {
		buffer[i++] = AFI[0];
	}
	
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    }
        
  /**
   * This method generates the command to read data from RFID tags in the RF 
   * field of the reader.  The startBlock argument must specify the first block 
   * to read tag data from.  The numBlocks argument specifies the number of 
   * blocks to read data from the tag starting from the starting block 
   * specified.
   *
   * @param   startBlock the first block of an RFID tag to read from
   * @param   numBlocks the number of blocks of data to read from    
   * 
   * @return  buffer containing the generated message
   */  
    public static byte[] readTag(byte tagType, byte[] tagID, byte startBlock, byte numBlocks){
               
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80]; 
        byte flags;
        
        if(tagID.length == 0)
        {
            /* tag ID not provided, rf_f = 1,selected state, tid_f = 0 no Tid */
            flags  = RW_TAG_SS;
        }
        else
        {
            /* tag ID provided, rf_f = 0 no selected state, tid_f = 1 for Tid */
            flags = RW_TAG_TID;
        }

        /* binary mode command format: msgLen|flags|command|type|tid|AFI|crc */
        /* set flags */
        buffer[i++] = flags;
        /* set command */
        buffer[i++] = READ_TAG;
        /* set tag type */
        buffer[i++] = tagType;
  
        /* set tagID if tag ID flag is set*/
        if(flags == RW_TAG_TID)
        {
            /* set tag ID */
            if(tagType == TAG_IT_HF)
            {
                for(i = 4; i < 8; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
            else
            {
                for(i = 4; i < 12; i++)
                {
                    buffer[i] = tagID[i - 4];
                }
            }
        }
        /* set starting block */
        buffer[i++] = startBlock;
        /* set number of blocks to read */
        buffer[i++] = numBlocks;
    
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    }
      
  /**
   * This method generates the command to write data to the memory of an RFID 
   * tag in the RF field of  the reader.  The startBlock argument must specify
   * the first block to write tag data to.  The numBlocks argument specifies 
   * the number of blocks to write starting from the specified starting block.  
   * This parameter is directly effected by tag memory size and also size of 
   * data.
   * 
   * @param   startBlock the first block of tag memory to write to
   * @param   numBlocks the number of blocks to write to the tag memory
   * @param   data to write to tag memory
   * 
   * @return  buffer containing the generated message
   * 
   */ 
    public static byte[] writeTag(byte tagType, byte[] tagID, byte startBlock, byte numBlocks, byte[] data){
               
        int bytesBlock, i = 1;
        short crc_check;
        byte[] buffer = new byte[80]; 
        byte flags;
        
        bytesBlock = 0x04;
        if(tagID.length == 0)
        {
            /* tag ID not provided, rf_f = 1,selected state, tid_f = 0 no Tid */
            flags  = RW_TAG_SS;
        }
        else
        {
            /* tag ID provided, rf_f = 0 no selected state, tid_f = 1 for Tid */
            flags = RW_TAG_TID;
        }

        /* binary mode command format: msgLen|flags|command|type|tid|AFI|crc */
        /* set flags */
        buffer[i++] = flags;
        /* set command */
        buffer[i++] = WRITE_TAG;
        /* set tag type */
        buffer[i++] = tagType;
          
        /* set tagID if tag ID flag is set*/
        if(flags == RW_TAG_TID)
        {
            /* set tag ID */
            if(tagType == TAG_IT_HF)
            {
                for(i = 4; i < 8; i++)
                {
                    buffer[i] = tagID[i - 4];
                }

            }
            else
            {
                for(i = 4; i < 12; i++)
                {
                    buffer[i] = tagID[i - 4];
                }
            }
        }
        /* set starting block */
        buffer[i++] = startBlock;
        /* set number of blocks to read */
        buffer[i++] = numBlocks;
        
        //bytes_per_block = getBytes(tagType or tagID)
        for(int j = 0; j < numBlocks * bytesBlock; j++)
        {
            buffer[i++] = data[j];
        }
    
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    }
        
  /**
   * This method generates to reader command to lock specified blocks of memory 
   * in an RFID tag in the field of the reader.  The startBlock argument must 
   * specify the first block to lock.  The numBlocks argument specifies the 
   * number of blocks to lock starting from the specified starting block. 
   * This parameter is directly effected by tag memory size.
   * 
   * @param   startBlock the first block of tag memory to lock
   * @param   numBlocks the number of blocks of tag memory to lock 
   * 
   * @return  buffer containing the generated message
   * 
   */ 
     public static byte[] lockTag(byte tagType, byte[] tagID, byte startBlock, byte numBlocks){
   
        int i = 1;
        short crc_check;
        byte[] buffer = new byte[80]; 
        byte flags;
        
        if(tagID.length == 0)
        {
            /* tag ID not provided, rf_f = 1,selected state, tid_f = 0 no Tid */
            flags  = LOCK_TAG_SS;
        }
        else
        {
            /* tag ID provided, rf_f = 0 no selected state, tid_f = 1 for Tid */
            flags = LOCK_TAG_TID;
        }
      
        /* binary mode command format: msgLen|flags|command|type|tid|AFI|crc */
        /* set flags */
        buffer[i++] = flags;
        /* set command */
        buffer[i++] = WRITE_TAG;
        /* set tag type */
        buffer[i++] = tagType;

     
         /* set tagID if no SS */
        if(flags == LOCK_TAG_TID)
        {
            /* set tag ID */
            if(tagType == TAG_IT_HF)
            {
                for(i = 4; i < 8; i++)
                {
                    buffer[i] = tagID[i - 4];
                }   
            }
            else
            {
                for(i = 4; i < 12; i++)
                {
                    buffer[i] = tagID[i - 4];

                }
            }
         }

        /* set starting block */
        buffer[i++] = startBlock;
        /* set number of blocks to read */
        buffer[i++] = numBlocks;
        
        /* msg Length */
        buffer[0] = (byte)(i + 1);

        /* check CRC */
	i = appendCRC(buffer,i);

	byte [] shortBuffer = new byte[i];
	System.arraycopy(buffer,0,shortBuffer,0,i);
        
        return shortBuffer;
    
    }
         
 
}
