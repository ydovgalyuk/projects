/*
 * PowerPWM.c
 *
 *  Created on: 12/10/2015
 *      Author: user
 */

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "gpio.h"
#include "ftm.h"

#define SYS_CLOCK	48000000 //Hz
//#define PWM_FREQ	  	1500 // Hz
#define PWM_FREQ	     250 // Hz
#define FTM_CLOCK   (( SYS_CLOCK / 64 ) / (PWM_FREQ - 1)) // 1200 = 10 mS

void FTM2_Task(void);

void initPowerPWM()
{
	SIM_RemapFTM2CH5ToPTB5();

	/* FTM2 is set as edge aligned pwm mode, high true pulse */
	//FTM_PWMInit(FTM2, FTM_PWMMODE_EDGEALLIGNED, FTM_PWM_HIGHTRUEPULSE);
	SIM->SCGC  |= SIM_SCGC_FTM2_MASK;

	/* disable counter */
	FTM2->SC  = 0x0;
	/* update CNT value */
	FTM2->CNT = 0;
	/* update MOD value */
	FTM2->MOD = FTM_CLOCK;

	FTM2->CONTROLS[5].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM2->CONTROLS[5].CnV  = 0;

	/* FTMEN enable */
	FTM_SetFTMEnhanced(FTM2);

	/* set clock source, start counter */
	FTM_ClockSet(FTM2, FTM_CLOCK_NOCLOCK, FTM_CLOCK_PS_DIV1);

	/* enable FTM2 interrupt in NVIC */
	//NVIC_EnableIRQ(FTM2_IRQn);
	/* setup call back function for interrupt */
	//FTM_SetCallback(FTM2, FTM2_Task);
	/* enable FTM2 overflow interrupt */
	//FTM_EnableOverflowInt(FTM2);

	//FTM_SetChannelValue(FTM2, FTM_CHANNEL_CHANNEL4, FTM_CLOCK / 2);
}

void setPowerOff()
{
	/* disable counter */
	FTM2->SC  = 0x0;
}

void setPowerDuty(unsigned int pcnt) //pct like xx.x
{
	volatile float realPcnt = (float)pcnt / 10.0;
	/* disable counter */
	FTM2->SC  = 0x0;

	if ( realPcnt > 0.5 && realPcnt < 100.0 )
	{
		volatile uint32_t u16ChV_new = (realPcnt * FTM_CLOCK) / 100;
		/* update the channel value */
		FTM2->CONTROLS[5].CnV  = u16ChV_new;
		/* set clock source, start counter */
		FTM_ClockSet(FTM2, FTM_CLOCK_SYSTEMCLOCK, FTM_CLOCK_PS_DIV64);
	}
}
