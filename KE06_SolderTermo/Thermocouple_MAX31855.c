/*
 * MAX31855.c
 *
 *  Created on: 12/10/2015
 *      Author: user
 */

#include "common.h"
//#include "ics.h"
//#include "rtc.h"
#include "Thermocouple_MAX31855.h"
#include "uart.h"
#include "gpio.h"
#include "wait.h"
//#include <memory.h>

static GPIO_PinType p_clk = GPIO_PTC1; 	 // pin 5
static GPIO_PinType p_miso = GPIO_PTF7;  // pin 7
static GPIO_PinType p_cs = GPIO_PTC0;	 // pin 6

void initMAX31855()
{
	GPIO_PinInit(p_clk, GPIO_PinOutput);
	GPIO_PinInit(p_cs, GPIO_PinOutput);
	GPIO_PinInit(p_miso, GPIO_PinInput);

	GPIO_PinClear(p_clk);

	GPIO_PinSet(p_cs);
}


struct Max31855Data readMax31855Data()
{
	union {
		uint32_t dword;
		struct Max31855Data data;
	} u;
	GPIO_PinClear(p_cs);

	for (int i = 0; i < 32; i++)
	{
		u.dword <<= 1;

		GPIO_PinSet(p_clk); /* Set CLK to sample value */

		WAIT_Waitus(1);

		if ( GPIO_Read(GPIOB) & (1l << (p_miso - 32)) )
			u.dword |= 1; /* Read value from MISO */

		GPIO_PinClear(p_clk); /* Set CLK to shift value */
	}

	GPIO_PinSet(p_cs);

	return u.data;
}

static float linInterpol(float x, float x1, float x2, float y1, float y2)
{
	return y1 + (y2 - y1) * ( (x - x1) / (x2 - x1) );
}

float getKThermocoupleTemp(struct Max31855Data data)
{
	return ((float)data.TD14bit) * 0.25;
}

float getThermocoupleTemp(struct Max31855Data data)
{
	//	K-Temp(x)	Hakko Temp(y)
	//	44			20
	//	94			100

	float kTypeTemp = ((float)data.TD14bit) * 0.25;

	float hakkoTemp = linInterpol(kTypeTemp, 44, 94, 20, 100);

	return hakkoTemp;
}

float getJunctionTemp(struct Max31855Data data)
{
	return ((float)data.JD12bit) * 0.0625;
}
