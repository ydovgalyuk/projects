/*
 * Buttons.c
 *
 *  Created on: 15/10/2015
 *      Author: user
 */

#include "common.h"
#include "gpio.h"
#include "kbi.h"

#define VIBRO_SW	GPIO_PTD5
#define BTN_UP		GPIO_PTF6
#define BTN_OK		GPIO_PTE5
#define BTN_DOWN	GPIO_PTC3

void KBI0_Task(void);
void KBI1_Task(void);

void btn_dn_handler(void);
void btn_up_handler(void);
void btn_ok_handler(void);
void activityDetected(void);
uint32_t GetTimeMs();

struct intData
{
	uint32_t lastHigh;
	uint32_t lastLow;
	uint32_t lastEvent;
};

static KBI_ConfigType  sKBIConfig={0};

static struct intData btnDn = {0, 0, 0};
static struct intData btnUp = {0, 0, 0};
static struct intData btnOk = {0, 0, 0};

#define MIN_TIME_BETWEEN_INT	200	// ms
#define MIN_INT_TIME_FRAME		 25	// ms

void initButtons()
{
	GPIO_PinInit(BTN_UP, GPIO_PinInput_InternalPullup); 	//1
	GPIO_PinInit(BTN_OK, GPIO_PinInput_InternalPullup); 	//2
	GPIO_PinInit(BTN_DOWN, GPIO_PinInput_InternalPullup); 	//3
	GPIO_PinInit(VIBRO_SW, GPIO_PinInput); 	//Vibro switch

	// Filter input signal if it is too short (shorter than 4 ms)
	PORT->IOFLT0 = PORT_IOFLT0_FLTDIV3(0b10)	// set Freq 0.25 kHz - 4 ms
			| PORT_IOFLT0_FLTC(0b11) 		// select FLTDIV3 period for port C
			| PORT_IOFLT0_FLTE(0b11)		// select FLTDIV3 period for port E
			| PORT_IOFLT0_FLTF(0b11);		// select FLTDIV3 period for port F

	/* Disable all the KBI pins */
	for (int i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
		sKBIConfig.sPin[i].bEn	 = 0;

	sKBIConfig.sBits.bRstKbsp   = 1;/*Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
	sKBIConfig.sBits.bKbspEn   = 1;/*The latched value in KBxSP register while interrupt flag occur to be read.*/
	sKBIConfig.sBits.bMode   = KBI_MODE_EDGE_ONLY;
	sKBIConfig.sBits.bIntEn  = 1;

	sKBIConfig.sPin[BTN_DOWN].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[BTN_DOWN].bEn   = 1;

	sKBIConfig.sPin[VIBRO_SW].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[VIBRO_SW].bEn   = 1;

	KBI_Init(KBI0, &sKBIConfig);

	/* Disable all the KBI pins */
	for (int i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
		sKBIConfig.sPin[i].bEn	 = 0;

	sKBIConfig.sBits.bRstKbsp   = 1;/*Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
	sKBIConfig.sBits.bKbspEn   = 1;/*The latched value in KBxSP register while interrupt flag occur to be read.*/
	sKBIConfig.sBits.bMode   = KBI_MODE_EDGE_ONLY;
	sKBIConfig.sBits.bIntEn  = 1;

	/*Falling edge/low level select; KBI1_P27 channel enable(SW3 on FRDM board) */
	sKBIConfig.sPin[BTN_UP - 32].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[BTN_UP - 32].bEn   = 1;

	/*Falling edge/low level select; KBI1_P28 channel enable(SW2 on FRDM board) */
	sKBIConfig.sPin[BTN_OK - 32].bEdge = KBI_FALLING_EDGE_LOW_LEVEL;
	sKBIConfig.sPin[BTN_OK - 32].bEn   = 1;


	KBI_Init(KBI1, &sKBIConfig);

	KBI_SetCallback(KBI0, KBI0_Task);
	KBI_SetCallback(KBI1, KBI1_Task);
}

void handleBtnInt(GPIO_Type *pGPIO, int intNo, struct intData* intD, void (*callb)())
{
	if ( intD && (GetTimeMs() - intD->lastEvent) >= MIN_TIME_BETWEEN_INT )
	{
		callb();
		intD->lastEvent = GetTimeMs();
	}
	/*
	if ( GPIO_Read(pGPIO) & (1l << intNo) )
		intD->lastHigh = GetTimeMs();
	else
		intD->lastLow = GetTimeMs();

	if (intD->lastLow && intD->lastHigh )
	{
		if ( (intD->lastLow - intD->lastHigh) >= MIN_INT_TIME_FRAME
				&& intD->lastEvent >= MIN_TIME_BETWEEN_INT )
		{
			callb();
			intD->lastEvent = GetTimeMs();
		}
		intD->lastLow = 0;
		intD->lastHigh = 0;
	}*/

}


/*****************************************************************************//*!
*
* @brief callback routine of KBI driver.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void KBI0_Task(void)
{
	if(KBI_GetSP(KBI0) == (1l << BTN_DOWN))
	{
		handleBtnInt(GPIOA, BTN_DOWN, &btnDn, btn_dn_handler);
	}
	if(KBI_GetSP(KBI0) == (1l << VIBRO_SW))
	{
		activityDetected();
	}
	KBI_RstSP(KBI0);
}

/*****************************************************************************//*!
*
* @brief callback routine of KBI driver.
*
* @param  none.
*
* @return none.
*
* @ Pass/ Fail criteria: none.
*
*****************************************************************************/

void KBI1_Task(void)
{
	if(KBI_GetSP(KBI1) == (1l << (BTN_UP - 32)))
	{
		handleBtnInt(GPIOB, BTN_UP - 32, &btnUp, btn_up_handler);
	}
	else if(KBI_GetSP(KBI1) == (1l << (BTN_OK - 32)))
	{
		handleBtnInt(GPIOB, BTN_OK - 32, &btnOk, btn_ok_handler);
	}

	KBI_RstSP(KBI1);
}
