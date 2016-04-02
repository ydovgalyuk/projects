/*
 * Gateway.c
 *
 *  Created on: 24 бер. 2016
 *      Author: Yuriy
 */

#include "Gateway.h"
#include "LED1.h"
#include "LED2.h"
#include "LED3.h"
#include "CDC1.h"

extern LDD_TDeviceData *ASerialLdd1_DeviceDataPtr;
QueueHandle_t g_MasterRecvQueue = NULL;

void GatewayEntry()
{
	g_MasterRecvQueue = FRTOS1_xQueueCreate(MAX_QUEUE_LEN * 2, sizeof(QueueMsg_t));
	while(1)
	{
		uint8_t queueEmpty = 0;
		do
		{
			QueueMsg_t msg;
			queueEmpty = (FRTOS1_xQueueReceive(g_MasterRecvQueue, &msg, 0) == errQUEUE_EMPTY);
			if (!queueEmpty)
			{
				handleIncomingMessage(&msg);
			}

		}
		while(!queueEmpty);

		WAIT1_Waitms(10);
	}
}


void handleIncomingMessage(QueueMsg_t *msg)
{
	uint8_t masterSendBuff[ 3 + MAX_QUEUE_MSG_LEN ];

	if (!msg)
		return;

			switch(msg->Sender)
			{
			case _433Mhz:
				masterSendBuff[0] = '4';
				masterSendBuff[1] = '3';
				masterSendBuff[2] = '3';
				LED1_On();
				break;
			case _2_4GHz:
				masterSendBuff[0] = '2';
				masterSendBuff[1] = '.';
				masterSendBuff[2] = '4';
				LED2_On();
				break;
			case _RFID:
				masterSendBuff[0] = 'R';
				masterSendBuff[1] = 'F';
				masterSendBuff[2] = 'I';
				LED3_On();
				break;
			default:
				masterSendBuff[0] = 'U';
				masterSendBuff[1] = 'N';
				masterSendBuff[2] = 'K';
				break;
			}

			memcpy(masterSendBuff + 3, msg->Msg, msg->MsgLen);
			word sent = 0;
			AS1_SendBlock(masterSendBuff, msg->MsgLen + 3, &sent);

			CDC1_SendBlock(masterSendBuff, msg->MsgLen + 3);

			LED1_Off();
			LED2_Off();
			LED3_Off();
}
