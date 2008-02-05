/**
 * This class creates a new SkyeTek Reader object.  This class provides methods
 * for generating the messages or command set specific to the SkyeTek protocol.  
 * The messages generated are used in the SkyeTekReader class in order to 
 * control the SkyeTek family of readers, including reader parameters, tags, 
 * and external peripheral functionality.
 */
   
    /* reader commands */
#define SELECT_TAG     = (byte)0x14;
#define READ_MEM       = (byte)0x21;
#define READ_SYS       = (byte)0x22;
#define READ_TAG       = (byte)0x24;
#define WRITE_MEM      = (byte)0x41;
#define WRITE_SYS      = (byte)0x42;    
#define WRITE_TAG      = (byte)0x44;     
    
    /* memory types */
#define EEPROM         = (byte)0x00;
#define RAM            = (byte)0x01;

    /* command flags */
#define CRC_ONLY       = (byte)0x20;

    /* *********************** */
    /* command specific fields */
    /* *********************** */
     
    /* READER COMMANDS */
    /* reader parameter locations */
#define SERIAL_NUM     = (byte)0x00;
#define FIRM_VERS      = (byte)0x01;
#define READER_ID      = (byte)0x02;
#define BAUD_RATE      = (byte)0x03;
#define OP_MODE        = (byte)0x04;
#define LED            = (byte)0x05;
#define BEEPER         = (byte)0x06;
#define PORT_DIR       = (byte)0x07;
#define PORT_VAL       = (byte)0x08;
    
    /* peripheral parameters */
#define LCD            = (byte)0x0A;
#define EXT_MEM        = (byte)0x0C;
   
    /* baud rates */
#define BR_9600        = (byte)0x00;
#define BR_19200       = (byte)0x01;
#define BR_38400       = (byte)0x02;
#define BR_57600       = (byte)0x03;
   
    /* TAG COMMANDS */
   
    /* tag command flags for select tag command*/
#define SEL_TAG                = (byte)0x20;
#define SEL_TAG_TID            = (byte)0x68;
#define SEL_TAG_INV            = (byte)0x22;
#define SEL_TAG_INV_LOOP       = (byte)0x23;
#define SEL_TAG_AFI            = (byte)0x30;
#define SEL_TAG_INV_AFI        = (byte)0x32;
#define SEL_TAG_INV_AFI_LOOP   = (byte)0x33;
#define SEL_TAG_LOOP           = (byte)0x29;        
    
    /* tag command flags for reading and writing */
#define RW_TAG_SS      = (byte)0x28;
#define RW_TAG_TID     = (byte)0x60;
#define LOCK_TAG_SS    = (byte)0x2C;
#define LOCK_TAG_TID   = (byte)0x64;
    
    /* tag types */
#define AUTO_DETECT    = (byte)0x00;
#define ISO15693       = (byte)0x01;
#define I_CODE1        = (byte)0x02;
#define TAG_IT_HF      = (byte)0x03;
#define ISO14443A      = (byte)0x04;
#define ISO14443B      = (byte)0x05;
#define PICOTAG        = (byte)0x06;
#define GEMWAVE        = (byte)0x06;

    /* reader response codes */
#define SELECT_TAG_PASS          = (byte)0x14;
#define SELECT_TAG_LOOP_ACTIVATE = (byte)0x1C;
#define SELECT_TAG_FAIL          = (byte)0x94;
#define SELECT_TAG_LOOP_CANCEL   = (byte)0x9C;

#define READ_MEM_PASS  = (byte)0x21;
#define READ_SYS_PASS  = (byte)0x22;
#define READ_TAG_PASS  = (byte)0x24;
#define READ_MEM_FAIL  = (byte)0xA1;
#define READ_SYS_FAIL  = (byte)0xA2;
#define READ_TAG_FAIL  = (byte)0xA4;

#define WRITE_MEM_PASS = (byte)0x41;
#define WRITE_SYS_PASS = (byte)0x42;    
#define WRITE_TAG_PASS = (byte)0x44;     
#define WRITE_MEM_FAIL = (byte)0xC1;
#define WRITE_SYS_FAIL = (byte)0xC2;    
#define WRITE_TAG_FAIL = (byte)0xC4;     

#define NON_ASCII_REQUEST         = (byte)0x80;     
#define BAD_CRC                   = (byte)0x81;     
#define FLAGS_DONT_MATCH_COMMAND  = (byte)0x82;     
#define FLAGS_DONT_MATCH_TAG_TYPE = (byte)0x83;     
#define UNKNOWN_COMMAND           = (byte)0x84;     
#define UNKNOWN_TAG_TYPE          = (byte)0x85;     
#define INVALID_STARTING_BLOCK    = (byte)0x86;     
#define INVALID_NUMBER_OF_BLOCKS  = (byte)0x87;     
#define INVALID_MESSAGE_LENGTH    = (byte)0x88;     

    /* flags */
#define LOOP_F       = (byte)0x01;
#define INV_F        = (byte)0x02;
#define LOCK_F       = (byte)0x04;
#define RF_F         = (byte)0x08;
#define AFI_F        = (byte)0x10;
#define CRC_F        = (byte)0x20;
#define TID_F        = (byte)0x40;
#define RID_F        = (byte)0x80;

