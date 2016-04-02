/*
 * Arduino.c
 *
 *  Created on: 20 бер. 2016
 *      Author: Yuriy
 */

#include "Bit1.h"
#include "Bit2.h"
#include "WAIT1.h"
#include "LED1.h"
#include "Gateway.h"
//#include <time.h>

static unsigned int micros = 0;

typedef struct {
	byte high;
	byte low;
} HighLow;

typedef struct {
	int pulseLength;
	HighLow syncFactor;
	HighLow zero;
	HighLow one;
	int dataLength;	// bytes
} Protocol;

void (*g_handleInterrupt)() = NULL;

int nReceiverInterrupt;
int nTransmitterPin;
int nRepeatTransmit;

Protocol protocol;

#define RCSWITCH_MAX_CHANGES 66

/*
 * timings[0] contains sync timing, followed by a number of bits
 */
static unsigned int timings[RCSWITCH_MAX_CHANGES];

static const Protocol proto[] = {
		{ 500, { 1, 32 }, { 1, 3 }, { 3, 1 }, 24 },    // protocol 0
		{ 350, { 1, 31 }, { 1, 3 }, { 3, 1 }, 0 }, // protocol 1
		{ 650, { 1, 10 }, { 1, 2 }, { 2, 1 }, 0 },    // protocol 2
		{ 100, { 30, 71 }, { 4, 11 }, { 9, 6 }, 0 },    // protocol 3
		{ 380, { 1, 6 }, { 1, 3 }, { 3, 1 }, 0 },    // protocol 4
		{ 500, { 6, 14 }, { 1, 2 }, { 2, 1 }, 0 },    // protocol 5
		};

#define PROTOCOLS_CNT ( sizeof(proto) / sizeof(proto[0]) )


static inline unsigned int diff(int A, int B) {
    return abs(A - B);
}


#define MAX_MSG_COUNT		4
#define MAX_MSG_LENGTH		66
#define TIME_BUFF_LENGTH	(MAX_MSG_LENGTH * 4)
uint16_t timeBuff[TIME_BUFF_LENGTH];
uint16_t  currPos = TIME_BUFF_LENGTH / 4;
int16_t  g_BuffFullPosNo = -1;

void zeroTimeBuff()
{
	memset(timeBuff, 0, sizeof(timeBuff));
}

void addToTimeBuff(uint16_t time)
{
	if (currPos == TIME_BUFF_LENGTH)
		currPos = TIME_BUFF_LENGTH / 4;

	if ( currPos >= (3 * MAX_MSG_LENGTH) ) // if we are in the last quarter
		timeBuff[currPos - (3 * MAX_MSG_LENGTH)] = time;	// copy data to first quarter
	timeBuff[currPos] = time;
	switch(currPos)
	{
	case (MAX_MSG_LENGTH * 2 - 1):
		g_BuffFullPosNo = 0;
		break;
	case (MAX_MSG_LENGTH * 3 - 1):
		g_BuffFullPosNo = MAX_MSG_LENGTH;
		break;
	case (MAX_MSG_LENGTH * 4 - 1):
		g_BuffFullPosNo = MAX_MSG_LENGTH * 2;
		break;
	}

	++currPos;
}

float tolerance = 0.1;	// 10% tolerance

uint8_t checkPattern(uint16_t pos, uint32_t protHighT, uint32_t protLowT)
{
	return diff(timeBuff[pos], protHighT) <= (protHighT * tolerance)
				 && diff(timeBuff[pos+1], protLowT) <= (protLowT * tolerance);
}


uint8_t decodeProt(const Protocol *prot, uint16_t startPos, uint32_t *msg, uint32_t *msgLen, uint8_t maxMsgCnt)
{
	uint32_t messages[MAX_MSG_COUNT] = {0,0,0,0};
	uint8_t  msgPos = 0;
	uint8_t  msgCount = 0;

	uint8_t msgBitLen = 0;
	uint8_t msgValid = 0;
	int8_t syncFound = -1;
	for(int pos = startPos; pos < startPos + TIME_BUFF_LENGTH / 2;)
	{
		if (syncFound >= 0)
		{
			if ( checkPattern(pos, prot->pulseLength * prot->one.high,
					prot->pulseLength * prot->one.low) )
			{
				msg[msgPos] <<= 1;
				msg[msgPos] |= 1;
				++msgBitLen;
				pos += 2;	// advance to next pair
			}
			else if ( checkPattern(pos, prot->pulseLength * prot->zero.high,
					prot->pulseLength * prot->zero.low) )
			{
				msg[msgPos] <<= 1;
				++msgBitLen;
				pos += 2;	// advance to next pair
			}
			else
			{
				// failure of sequence. may be end of protocol
				if ( msgBitLen > 0)
				{
					if (msgBitLen == prot->dataLength
						|| (prot->dataLength == 0
							&& msgBitLen >= 4
							&& msgBitLen <= 32 ) )
					{
						// conclude that current protocol is complete, advance to next message
						msgLen[msgPos] = msgBitLen;
						++msgPos;
						++msgCount;
						if (maxMsgCnt == msgCount)
							break;
						syncFound = -1;	// try to check the same data for sync
					}
					else  // collected data are invalid
						msg[msgPos] = 0;
				}
				syncFound = -1;
			}
		}
		else
		{
			if ( checkPattern(pos, prot->pulseLength * prot->syncFactor.high,
					prot->pulseLength * prot->syncFactor.low) )
			{
				syncFound = pos; // sync found, remember sync position
				pos += 2; // advance 2 position, skipping preambule
			}
			else
				++pos;	// sync not found, advance one position more
		}
	}
	return msgCount;
}


typedef struct {
	uint32_t Msgs[MAX_MSG_COUNT];
	uint32_t MsgLen[MAX_MSG_COUNT];
	uint8_t DecodedMessages;
	uint8_t Proto;
} Messages_t ;

Messages_t	g_Messages[PROTOCOLS_CNT];
uint16_t  g_TotalMsgDecoded = 0;

void scanTimeBuffer(uint16_t startPos)
{
	g_TotalMsgDecoded = 0;
	memset(g_Messages, 0, sizeof(g_Messages));
	for(uint16_t i = 0; i < PROTOCOLS_CNT; ++i) // for each protocol
	{
		g_Messages[i].Proto = i;
		g_Messages[i].DecodedMessages =
				decodeProt( &proto[i], startPos, &g_Messages[i].Msgs, &g_Messages[i].MsgLen, MAX_MSG_COUNT);
		g_TotalMsgDecoded += g_Messages[i].DecodedMessages;
	}
}

uint8_t stopSampling = 0;

void _433MHz_sniffer()
{
	//while(1)
	{
		if (g_BuffFullPosNo >= 0)
		{
			LED1_Off();
			scanTimeBuffer(g_BuffFullPosNo);
			if (g_TotalMsgDecoded)
				LED1_On();

			QueueMsg_t msg;
			for(int p = 0; p < PROTOCOLS_CNT; ++p )
			{
				for(int m = 0; m < g_Messages[p].DecodedMessages; ++m )
				{
					msg.Sender = _433Mhz;
					msg.MsgLen = g_Messages[p].MsgLen[m];
					memcpy(msg.Msg, g_Messages[p].Msgs[m], sizeof(g_Messages[p].Msgs[m]));

					if (g_MasterRecvQueue
							&& FRTOS1_xQueueSendToBack( g_MasterRecvQueue, &msg, 0) != pdPASS)
							; // here we need send error
				}
			}

			g_BuffFullPosNo = -1;
		}
		WAIT1_Waitms(5);
	}
}


uint8_t lastBit2 = 0;
uint32_t lastChangeTime = 0;
void uS_Timer_ISR()
{
	int value = FTM0_SC;
	FTM0_SC &= ~(FTM_SC_TOF_MASK);

	micros += 10;

	uint8_t bit2 = Bit2_GetVal();
	if (bit2 != lastBit2)
	{
		if (!stopSampling)
			addToTimeBuff(micros - lastChangeTime);
		lastChangeTime = micros;
		lastBit2 = bit2;
	}
}
