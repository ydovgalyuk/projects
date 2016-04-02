/*
 * Gateway.h
 *
 *  Created on: 24 бер. 2016
 *      Author: Yuriy
 */

#ifndef SOURCES_GATEWAY_H_
#define SOURCES_GATEWAY_H_

#include "FRTOS1.h"

enum SenderType {
	_433Mhz,
	_2_4GHz,
	_RFID,
	Master
};

#define MAX_QUEUE_MSG_LEN	32
#define MAX_QUEUE_LEN		10

typedef struct {
	enum SenderType Sender;
	uint8_t Msg[MAX_QUEUE_MSG_LEN];
	uint8_t MsgLen;
} QueueMsg_t;

extern QueueHandle_t g_433MHzSendQueue;
extern QueueHandle_t g_2_4GHzSendQueue;
extern QueueHandle_t g_MasterRecvQueue;



#endif /* SOURCES_GATEWAY_H_ */
