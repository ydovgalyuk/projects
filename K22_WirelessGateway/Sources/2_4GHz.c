#include "RF24.h"
#include "FRTOS1.h"
#include "Gateway.h"

#define PAYLOAD_SIZE	32
#define CHANNEL_NO		5

/* macros to configure device either for RX or TX operation */
#define TX_POWERUP()   RF24_WriteRegister(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PWR_UP|RF24_PRIM_TX) /* enable 2 byte CRC, power up and set as PTX */
#define RX_POWERUP()   RF24_WriteRegister(RF24_CONFIG, RF24_EN_CRC|RF24_CRCO|RF24_PWR_UP|RF24_PRIM_RX) /* enable 2 byte CRC, power up and set as PRX */

static uint8_t payloadBuffer[PAYLOAD_SIZE];
static const uint8_t TADDR[5] = {0x11, 0x22, 0x33, 0x44, 0x55}; /* device address */
static uint8_t isrFlag = 0;
QueueHandle_t g_2_4GHzSendQueue = NULL;

void _2_4GHz_Entry()
{
	g_2_4GHzSendQueue = FRTOS1_xQueueCreate(MAX_QUEUE_LEN, sizeof(QueueMsg_t));

	RF24_Init();
	RF24_WriteRegister(RF24_RF_SETUP, RF24_RF_SETUP_RF_PWR_0|RF24_RF_SETUP_RF_DR_1000);
	RF24_WriteRegister(RF24_RX_PW_P0, PAYLOAD_SIZE); /* number of payload bytes we want to send and receive */
	RF24_WriteRegister(RF24_RF_CH, CHANNEL_NO); /* set channel */

	/* Set RADDR and TADDR as the transmit address since we also enable auto acknowledgment */
	RF24_WriteRegisterData(RF24_RX_ADDR_P0, (uint8_t*)TADDR, sizeof(TADDR));
	RF24_WriteRegisterData(RF24_TX_ADDR, (uint8_t*)TADDR, sizeof(TADDR));

	/* Enable RX_ADDR_P0 address matching */
	RF24_WriteRegister(RF24_EN_RXADDR, RF24_EN_RXADDR_ERX_P0); /* enable data pipe 0 */

	_2_4GHz_SetRecvMode();

	for(;;)
	{
		if (isrFlag) // first handle interrupt if any
		{
			isrFlag = 0;
			uint8_t status = RF24_GetStatus();
			if (status&RF24_STATUS_RX_DR)
			{ /* data received interrupt */
				_2_4GHz_Recv();
			}
			if (status&RF24_STATUS_TX_DS)
			{ /* data sent interrupt */
				RF24_ResetStatusIRQ(RF24_STATUS_TX_DS); /* clear bit */
			}
			if (status&RF24_STATUS_MAX_RT)
			{ /* retry timeout interrupt */
				RF24_ResetStatusIRQ(RF24_STATUS_MAX_RT); /* clear bit */
			}
		}

		uint8_t queueEmpty = 0;
		do
		{
			QueueMsg_t msg;
			queueEmpty = (FRTOS1_xQueueReceive(g_2_4GHzSendQueue, &msg, 0) == errQUEUE_EMPTY);
			if (!queueEmpty)
			{
				_2_4GHz_SetSendMode();
				_2_4GHz_Send(msg.Msg, msg.MsgLen);
				// Switch back to receive mode
				_2_4GHz_SetRecvMode();
			}
		}
		while(!queueEmpty);

		WAIT1_Waitms(10);
	}
}

void _2_4GHz_SetSendMode()
{
	// Switch back to sender mode
	RF24_WriteRegister(RF24_EN_AA, RF24_EN_AA_ENAA_P0); /* enable auto acknowledge. RX_ADDR_P0 needs to be equal to TX_ADDR! */
	RF24_WriteRegister(RF24_SETUP_RETR, RF24_SETUP_RETR_ARD_750|RF24_SETUP_RETR_ARC_15); /* Important: need 750 us delay between every retry */
	TX_POWERUP();  /* Power up in transmitting mode */
	CE1_ClrVal();   /* Will pulse this later to send data */
}

void _2_4GHz_SetRecvMode()
{
	RX_POWERUP();  /* Power up in receiving mode */
	CE1_SetVal();   /* Listening for packets */

	RF24_ResetStatusIRQ(RF24_STATUS_RX_DR|RF24_STATUS_TX_DS|RF24_STATUS_MAX_RT);
}

void _2_4GHz_Send(uint8_t* payload, uint8_t payloadSize)
{
	// send here
	RF24_TxPayload(payload, payloadSize); /* send data */
}

void _2_4GHz_Recv()
{
	QueueMsg_t msg;

	RF24_RxPayload(msg.Msg, PAYLOAD_SIZE); /* will reset RX_DR bit */
	RF24_ResetStatusIRQ(RF24_STATUS_RX_DR|RF24_STATUS_TX_DS|RF24_STATUS_MAX_RT); /* make sure we reset all flags. Need to have the pipe number too */

	msg.Sender = _2_4GHz;
	msg.MsgLen = PAYLOAD_SIZE;
	if (g_MasterRecvQueue
			&& FRTOS1_xQueueSendToBack( g_MasterRecvQueue, &msg, 0) != pdPASS)
		; // here we need send error
}


void _2_4GHz_ISR()
{
	CE1_ClrVal(); /* stop sending/listening */
	isrFlag = 1;
}

void RADIO_OnInterrupt()
{

}
