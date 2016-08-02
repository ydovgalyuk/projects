#ifndef MAX31855_HH
#define MAX31855_HH

#include "common.h"
#include "gpio.h"

struct Max31855Data
{
	char OC_Fault	: 1;	// 0
	char SCG_Fault	: 1;	// 1
	char SCV_Fault	: 1;	// 2
	char _res2		: 1;	// 3
	int16_t JD12bit : 12;	// 4-16
	char Fault		: 1;	// 17
	char _res1		: 1;	// 18
	int16_t TD14bit : 14;	// 19-31
};

void initMAX31855();
struct Max31855Data readMax31855Data();
float getThermocoupleTemp(struct Max31855Data data);
float getJunctionTemp(struct Max31855Data data);

#endif
