
#include <inttypes.h>
#include <stdlib.h>
#include <pin_map.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "sp_task.h"
#include "sp_semaphore.h"
#include "sp_timer.h"
#include "CC1000Const.h"
#include "cc1k_lpl.h"
#include "cc1k.h"
#include "radio_spi.h"
#include "crc.h"
#include "sp_rand.h"
#include "adc.h"
#include "led.h"

//-----------------------------------------------------------------------------
// RADIO CONSTANTS
enum {
  SYNC_BYTE    =  0x33,
  NSYNC_BYTE   =  0xcc,
  SYNC_WORD    =  0x33cc,
  NSYNC_WORD   =  0xcc33,
  ACK_LENGTH   =  16,
  MAX_ACK_WAIT =  18
};

//-----------------------------------------------------------------------------
// RADIO VARIABLES
static uint8_t ack_code[3] = {0xab, 0xba, 0x83};
static uint8_t iRSSIcount;
static uint8_t iSquelchCount;
static uint8_t txbufptr_ack;
static uint8_t lplpower;        //!< Low power listening mode
static uint8_t lplpowertx;      //!< Low power listening transmit mode
static uint16_t preamblelen;    //!< Current length of the preamble
static uint16_t PreambleCount;  //!< Found a valid preamble
static uint8_t SOFCount;
static uint16_t search_word;
static union {
  uint16_t W;
  struct {
	uint8_t LSB;
	uint8_t MSB;
  };
} RxShiftBuf;
static uint8_t RxBitOffset;	    //!< Bit offset for spibus
static uint16_t RxByteCnt;	    //!< Received byte counter
static uint16_t TxByteCnt;
static uint16_t RSSISampleFreq; //!< In Bytes rcvd per sample
static struct {
	uint8_t bInvertRxData :1;
	uint8_t bTxPending :1;
	uint8_t bTxBusy :1;
	uint8_t bAckEnable :1;
	uint8_t bTimeStampEnable :1;
} bFlag;
static uint16_t usRunningCRC;   //!< Running CRC variable
static uint16_t usRSSIVal;      
static uint16_t usSquelchVal;
static uint16_t usTempSquelch;
static uint8_t usSquelchIndex;
static uint16_t usSquelchTable[CC1K_SquelchTableSize];
static int16_t sMacDelay;       //!< MAC delay for the next transmission
static uint16_t RecvPktCRC;
static uint8_t  txbufptr_ack;

static uint8_t* radio_data;
static uint8_t  radio_data_size;
static sp_pid_t radio_data_pid;

static uint8_t incoming_data_size;

static uint8_t  recv_data_size;
static uint8_t* recv_data;
static sp_pid_t recv_data_pid;

static volatile uint8_t isIdle;

//-----------------------------------------------------------------------
// Forward declaration
//
static inline void cc1k_radio_start_radio( void );
static inline void cc1k_radio_stop_radio( void );
static inline int16_t MacBackoff_initialBackoff();
static inline int16_t MacBackoff_congestionBackoff();
static void adjustSquelch( uint16_t data );
static uint8_t cc1k_radio_get_received_byte( uint8_t data_in );
static sp_sem_t radio_idle_sem;

void cc1k_radio_start_send( void* data, uint8_t size )
{
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	radio_data       = data;
	radio_data_size  = size;
	radio_data_pid   = CURRENT_TASK()->pid;
	
	sMacDelay = MacBackoff_initialBackoff();
	bFlag.bTxPending = 1;
	LEAVE_CRITICAL_SECTION();
}

void cc1k_radio_start_recv( void* data, uint8_t size )
{
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	recv_data      = data;
	recv_data_size = size;
	recv_data_pid  = CURRENT_TASK()->pid;
	LEAVE_CRITICAL_SECTION();
}

void cc1k_radio_squelch_task( void )
{
	static sp_timer_t squelch_timer;
	TASK_BEGIN();
	
	if( lplpower == 0 ) {
		if( iSquelchCount > CC1K_SquelchCount ) {		
			sp_timer_open( &squelch_timer, CC1K_SquelchIntervalSlow );
		} else {
			sp_timer_open( &squelch_timer, CC1K_SquelchIntervalFast );
		}
	}
	
	while( 1 ) {
		sp_timer_read(&squelch_timer);
		if( isIdle == 1 ) {
			adc_read(0x00, &usTempSquelch);
			if( isIdle == 1 ) {
				adjustSquelch(usTempSquelch);
			}
		}
	}
	TASK_END();
}

void cc1k_radio_squelch_task2( void )
{
	static uint16_t squelch2_adc;
	TASK_BEGIN();
	while( 1 ) {
		TASK_SEM_WAIT(&radio_idle_sem);
		
		if( isIdle == 1 ) {
			adc_read(0x00, &squelch2_adc);
			if( isIdle == 1 ) {
				adjustSquelch(squelch2_adc);
			}
		}
	}
	TASK_END();
}


void cc1k_radio_main_task( void )
{	
	static uint8_t data_in;
	static uint16_t rssi_adc;
	
	TASK_BEGIN();
	spi_enableIntr();
	while( 1 ) {
		//
		// IDLE state
		//
		isIdle = 1;
		radio_spi_read(&data_in);
		
		if (((data_in == (0xaa)) || (data_in == (0x55)))) {
			
			PreambleCount++;
			if (PreambleCount > CC1K_ValidPrecursor) {
				PreambleCount = SOFCount = 0;
				RxBitOffset = RxByteCnt = 0;
				isIdle = 0;
				RxShiftBuf.W = 0;
				RxShiftBuf.LSB = data_in;
				//
				// SYNC_STATE
				//
				while( SOFCount < 20 ) {
					radio_spi_read(&data_in);
					{
						uint8_t i;
						uint16_t usTmp;
						// bit shift the data in with previous sample to find sync
						usTmp = RxShiftBuf.W;                                 
						RxShiftBuf.W <<= 8;                                   
						RxShiftBuf.LSB = data_in;                             
                                                                
						for(i=0;i<8;i++) {                                    
							usTmp <<= 1;                                        
							if(data_in & 0x80)                                  
								usTmp  |=  0x1;                                   
							data_in <<= 1;                                      
							// check for sync bytes                             
							if (usTmp == SYNC_WORD) {                           
								RxBitOffset = 7-i;
								SOFCount = 100;
								adc_read(0x00, &usRSSIVal);
								break;                                            
							}                                                   
						}
					}
					SOFCount++;
					
				} // while( SOFCount < 20 )
				if( SOFCount == 20 ) {
					continue;
				}
				
				//
				// RXSTATE_PREHEADER (read group id)
				//
				radio_spi_read(&data_in);
				data_in = cc1k_radio_get_received_byte( data_in );
				
				usRunningCRC = 0;
				RxByteCnt = 0;
				//
				// RXSTATE_HEADER 
				//
				while( RxByteCnt < 7 ) {
					radio_spi_read(&data_in);
					data_in = cc1k_radio_get_received_byte( data_in );
					usRunningCRC = crcByte(usRunningCRC, data_in);
					RxByteCnt++;
				}
				//
				// Read size
				//
				radio_spi_read(&data_in);
				incoming_data_size = cc1k_radio_get_received_byte( data_in );
				usRunningCRC = crcByte(usRunningCRC, incoming_data_size);

				//
				// RXSTATE_DATA
				//
				RxByteCnt = 0;
				while( RxByteCnt < incoming_data_size && (RxByteCnt < recv_data_size)) {
					radio_spi_read(&data_in);
					data_in = cc1k_radio_get_received_byte( data_in );
					usRunningCRC = crcByte(usRunningCRC, data_in);
					recv_data[RxByteCnt] = data_in;
					RxByteCnt++;
				}
				
				while( RxByteCnt < incoming_data_size ) {
					radio_spi_read(&data_in);
					data_in = cc1k_radio_get_received_byte( data_in );
					usRunningCRC = crcByte(usRunningCRC, data_in);
					RxByteCnt++;
				}
				//
				// RXSTATE_CRC_BYTE1
				//
				radio_spi_read(&data_in);
				RecvPktCRC = (uint8_t)cc1k_radio_get_received_byte( data_in );
				
				//
				// RXSTATE_CRC_BYTE2
				//
				radio_spi_read(&data_in);
				data_in = cc1k_radio_get_received_byte( data_in );
				RecvPktCRC = (((uint16_t) (data_in)) << 8) | RecvPktCRC;
				
				if (RecvPktCRC == usRunningCRC) {
					// 
					// CRC has passed
					//
					if( bFlag.bAckEnable == 1 ) {
						//
						// SENDING_ACK
						//
						RxByteCnt = 0;
						while( RxByteCnt < (ACK_LENGTH - sizeof(ack_code) - 2) ) {
							radio_spi_write(0xaa);
							RxByteCnt++;
						}
						RxByteCnt = 0;
						while( RxByteCnt < sizeof(ack_code) ) {
							radio_spi_write(ack_code[RxByteCnt]);
							RxByteCnt++;
						}
						//
						// XXX Here is a buffer overflow in original SOS implementation
						// it also appeared in TinyOS implementation
						//
						radio_spi_write( 0xaa );
						radio_spi_write( 0xaa );
					}
					//
					// send to application
					//
					if( recv_data_pid != NULL_PID ) {
						recv_data = NULL;
						recv_data_size = 0;
						sp_signal( recv_data_pid );
						recv_data_pid = NULL_PID;
					}
				} // if (RecvPktCRC == usRunningCRC)			
			} // if (PreambleCount > CC1K_ValidPrecursor)
		} else if((bFlag.bTxPending) && (--sMacDelay <= 0)) {
			iRSSIcount = 0;
			PreambleCount = 0;
			isIdle = 0;
			//
			// PRETX_STATE ( the code below is from RSSIADC_dataReady )
			//
			
			while( iRSSIcount < CC1K_MaxRSSISamples ) {
				adc_read(0x00, &rssi_adc);
				if( (rssi_adc > (usSquelchVal + CC1K_SquelchBuffer)) ) {
					// channel is clear, GO GO GO!
					usRSSIVal = rssi_adc;
					
					radio_spi_write(0xaa);
					break;
				} 
				iRSSIcount++;	
			}
			
			if( iRSSIcount < CC1K_MaxRSSISamples ) {
				//
				// TXSTATE_PREAMBLE
				//
				
				TxByteCnt = 0;
				while( TxByteCnt < preamblelen ) {
					radio_spi_write(0xaa);
					TxByteCnt++;
				}
				//
				// TXSTATE_SYNC
				//
				radio_spi_write(SYNC_BYTE);
				radio_spi_write(NSYNC_BYTE);

				//
				// TXSTATE_PREHEADER
				// (transmit SOS group ID)
				//
				radio_spi_write(0x20);
				
				//
				// TXSTATE_HEADER (transmit SOS header)
				//
				TxByteCnt = 0;
				usRunningCRC = 0;
				while( TxByteCnt < 7 ) {
					radio_spi_write( TxByteCnt );
					asm volatile("beforeCRC:");
					usRunningCRC = crcByte(usRunningCRC,TxByteCnt);
					asm volatile("afterCRC:");
					TxByteCnt++;
				}
				//
				// Transmit size
				//
				radio_spi_write( radio_data_size );
				usRunningCRC = crcByte(usRunningCRC,radio_data_size);
				
				//
				// TXSTATE_DATA
				//
				TxByteCnt = 0;
				while( TxByteCnt < radio_data_size ) {
					radio_spi_write( radio_data[TxByteCnt] );
					usRunningCRC = crcByte(usRunningCRC, radio_data[TxByteCnt]);
					TxByteCnt++;
				}
				//
				// TXSTATE_CRC
				//
				radio_spi_write( usRunningCRC );
				usRunningCRC = usRunningCRC >> 8;
				radio_spi_write( usRunningCRC );
				
				//
				// TXSTATE_FLUSH
				//
				
				TxByteCnt = 0;
				while( TxByteCnt <= 1 ) {
					radio_spi_write( 0xaa );
					TxByteCnt++;
				}
				
				if( (bFlag.bAckEnable == 1 ) ) {
					
					search_word = 0;
					txbufptr_ack = 0;
					
					//
					// TXSTATE_WAIT_FOR_ACK
					//
					TxByteCnt = 1;
					while( TxByteCnt <= 3 ) {
						// just read in useless data
						radio_spi_read(&data_in);
						TxByteCnt++;
					}
					//
					// TXSTATE_READ_ACK
					//
					
					while( TxByteCnt < MAX_ACK_WAIT ) {
						radio_spi_read(&data_in);
						{
							uint8_t i;
							for(i = 0; i < 8; i ++){
								search_word <<= 1;
								if(data_in & 0x80) search_word |=  0x1;
								data_in <<= 1;
								if (search_word == 0xba83){
									txbufptr_ack = 1;
									TxByteCnt = MAX_ACK_WAIT;
									break;
								}
							}
						}
					}
				}
				//
				// TXSTATE_DONE
				//
				bFlag.bTxPending = 0;
				radio_data = NULL;
				radio_data_size = 0;
				//
				// Signal 
				//
				TASK_SEM_SIGNAL( &radio_idle_sem );
				sp_signal( radio_data_pid );
				radio_data_pid = NULL_PID;
			} else {
				sMacDelay = MacBackoff_congestionBackoff();
				// Let it fall out and go back to while(1)
			}
		} // IDLE_STATE if-else-if
	} // while( 1 )
	TASK_END();
}



void cc1k_radio_init( void )
{
	uint8_t i;
	SET_CC_PALE_DD_OUT();
	SET_CC_PDATA_DD_OUT();	
	SET_CC_PCLK_DD_OUT();
	SET_MISO_DD_IN(); 
	SET_OC1C_DD_IN();

	RxBitOffset = 0;
	iSquelchCount = 0;
	PreambleCount = 0;
	RSSISampleFreq = 0;
	RxShiftBuf.W = 0;
	iRSSIcount = 0;

	bFlag.bTxPending = 0;
	bFlag.bTxBusy = 0;
	bFlag.bAckEnable = 0;
	bFlag.bTimeStampEnable = 0;
	sMacDelay = -1;
	usRSSIVal = -1;
	usSquelchIndex = 0;
	lplpower = lplpowertx = 0;
	usSquelchVal = CC1K_SquelchInit;
	
	for (i = 0; i < CC1K_SquelchTableSize; i++) {                   
		usSquelchTable[(int)i] = CC1K_SquelchInit;
	}         
	
	TASK_SEM_INIT(&radio_idle_sem, 0);
	
	radio_data         = NULL;
	radio_data_size    = 0;
	radio_data_pid     = NULL_PID;

	recv_data          = NULL;
	recv_data_size     = 0;
	recv_data_pid      = NULL_PID;

	radio_spi_init();
	bFlag.bInvertRxData = cc1k_cnt_GetLOStatus();

	sp_create( cc1k_radio_main_task, 254 );
	//
	// These two must run at the same priority
	//
	sp_create( cc1k_radio_squelch_task, 1 );
	sp_create( cc1k_radio_squelch_task2, 1 );
	
} 


//-----------------------------------------------------------
// Helper routines
//
static inline void cc1k_radio_start_radio( void )
{
	cc1k_cnt_start();
    cc1k_cnt_BIASOn();
    spi_rxmode();
    cc1k_cnt_RxMode();
    spi_enableIntr();
}

static inline void cc1k_radio_stop_radio( void )
{
	cc1k_cnt_stop();
	spi_disableIntr();
}

static uint8_t cc1k_radio_get_received_byte( uint8_t data_in )
{
	RxShiftBuf.W <<=8;      
	RxShiftBuf.LSB = data_in;      
	return (RxShiftBuf.W >> RxBitOffset);
}

static void adjustSquelch( uint16_t data ) 
{
	uint16_t tempArray[CC1K_SquelchTableSize];
	uint8_t i,j,min; 
	uint16_t min_value;
	uint32_t tempsquelch;
	uint16_t tempval;
	HAS_CRITICAL_SECTION;
  
	usSquelchTable[usSquelchIndex] = data;
	usSquelchIndex++;
	if (usSquelchIndex >= CC1K_SquelchTableSize) {
		usSquelchIndex = 0;
	}
	if (iSquelchCount <= CC1K_SquelchCount) {
		iSquelchCount++;
	}
  
	for (i=0; i<CC1K_SquelchTableSize; i++) {
		tempArray[(int)i] = usSquelchTable[(int)i];
	}
  
	min = 0;
	//  for (j = 0; j < ((CC1K_SquelchTableSize) >> 1); j++) {
	for (j = 0; j < 3; j++) {
		for (i = 1; i < CC1K_SquelchTableSize; i++) {
			if ((tempArray[(int)i] != 0xFFFF) && 
				((tempArray[(int)i] > tempArray[(int)min]) ||
				(tempArray[(int)min] == 0xFFFF))) {
				min = i;
			}
		}
		min_value = tempArray[(int)min];
		tempArray[(int)min] = 0xFFFF;
	}
	tempsquelch = ((uint32_t)(usSquelchVal << 5) + (uint32_t)(min_value << 1));
	tempval     = (uint16_t)((tempsquelch / 34) & 0x0FFFF);
	ENTER_CRITICAL_SECTION();
	usSquelchVal = tempval;
	LEAVE_CRITICAL_SECTION();
}

//-----------------------------------------------------------------------------
// MAC BACKOFF
static inline int16_t MacBackoff_initialBackoff()
{
  return ((sp_rand() & 0x1F) + 5);
}

static inline int16_t MacBackoff_congestionBackoff()
{
  return ((sp_rand() & 0xF) + 5);
}




