/*
 * time.c
 *
 *  Created on: 18/10/2015
 *      Author: user
 */

#include "common.h"
#include "rtc.h"

static RTC_ConfigType  sRTCConfig ={0};
static uint32_t	timeCounter = 0;
void RTC_Task(void);

void initTime()
{
	/* configure RTC to 1Hz interrupt frequency */
	uint16_t u16ModuloValue = 24;
	sRTCConfig.u16ModuloValue = u16ModuloValue;
	sRTCConfig.bInterruptEn   = RTC_INTERRUPT_ENABLE;     /*!< enable interrupt */
	sRTCConfig.bClockSource   = RTC_CLKSRC_BUS;          /*!< clock source is 1khz */
	sRTCConfig.bClockPresaler = RTC_CLK_PRESCALER_1000;    /*!< prescaler is 100 */
	RTC_SetCallback(RTC_Task);
	RTC_Init(&sRTCConfig);
}

void RTC_Task(void)
{
    ++timeCounter;
}

uint32_t GetTimeMs()
{
	return timeCounter;
}
