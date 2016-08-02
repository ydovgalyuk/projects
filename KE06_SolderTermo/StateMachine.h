#ifndef STATE_MACH_HH
#define STATE_MACH_HH

#include "common.h"

struct VRAM
{
	int16 TempSP;
	int16 CurrTemp;
	int16 JunctTemp;
	int16 OffCntDwn;
	int16 PWM_Sp;
	int16 ADCVal;
};

extern struct VRAM vram;

#endif
