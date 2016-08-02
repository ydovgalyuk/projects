/*
 * Thermocouple_ADC.c
 *
 *  Created on: 28 лист. 2015
 *      Author: Yuriy
 */

#include "common.h"
#include "adc.h"
#include <math.h>

unsigned int kalmanFilter(unsigned int data);

#define THERMOCOUPLE_PIN	ADC_CHANNEL_AD15

ADC_ConfigType sADC_Config = { 0 };

void initThermocoupleADC() {
	/* initiaze ADC module */
	//sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_4;
	//sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK;
	//sADC_Config.sSetting.bLongSampleEn = 1;
	sADC_Config.sSetting.bContinuousEn = 1;
	sADC_Config.u8ClockDiv = ADC_ADIV_DIVIDE_8;
	sADC_Config.u8ClockSource = CLOCK_SOURCE_BUS_CLOCK_DIVIDE_2;
	sADC_Config.u8Mode = ADC_MODE_12BIT;

	ADC_Init( ADC, &sADC_Config);
}

static float linInterpol(float x, float x1, float x2, float y1, float y2) {
	return y1 + (y2 - y1) * ((x - x1) / (x2 - x1));
}

int16_t getThermocoupleADCValue() {
	return ADC_PollRead(ADC, THERMOCOUPLE_PIN);
}

unsigned int lastADC = 0;
#define ADC_AVG_BUFF_SZ	5
int32_t getThermocoupleADCTemp()
{
	unsigned int adc = ADC_PollRead(ADC, THERMOCOUPLE_PIN);
	for (int i = 0; i < ADC_AVG_BUFF_SZ - 1; ++i)
		adc += ADC_PollRead(ADC, THERMOCOUPLE_PIN);
	adc /= ADC_AVG_BUFF_SZ;
	//unsigned int avgData = (adc + lastADC) / 2;
	//lastADC = adc;
	unsigned int avgData = kalmanFilter( adc );
	//**********************
	// 100 - 80
	// 200 - 95
	// 500 - 145
	// 600 - 158
	float Temp = linInterpol(avgData, 100.0, 600.0, 80.0, 160.0);
	return (int32_t) (Temp * 10.0); /* xxx.x */
}

int32_t getJunctionADCTemp() {
	// 12 bits = 0xFFF
	unsigned int adcData = ADC_PollRead(ADC, ADC_CHANNEL_AD22_TEMPSENSOR);

	float Temp;
	Temp = log(10000.0 * (0xFFF / adcData - 1));
	Temp = 1
			/ (0.001129148
					+ (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
	Temp = Temp - 273.15;            // Convert Kelvin to Celcius

	return (int32_t) (Temp * 10.0);
}


float kalmanState = 1;
float kalmanCovariance = 1;

unsigned int kalmanFilter(unsigned int data)
{
	float kalmanF = 1; // factor of real value to previous real value
	float kalmanQ = 2; // measurement noise
	float kalmanH = 1; // factor of measured value to real value
	float kalmanR = 5; // environment noise

	//time update - prediction
	float kalmanX0 = kalmanF * kalmanState;
	float kalmanP0 = kalmanF * kalmanCovariance * kalmanF + kalmanQ;

	//measurement update - correction
	float divCoef = (kalmanH * kalmanP0 * kalmanH + kalmanR);
	if (divCoef == 0.0)
		return kalmanState;
	float K = kalmanH * kalmanP0 / divCoef;
	kalmanState = kalmanX0 + K * (data - kalmanH * kalmanX0);
	kalmanCovariance = (1.0f - K * kalmanH) * kalmanP0;

	return kalmanState;
}

