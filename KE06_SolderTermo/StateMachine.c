#include "common.h"
#include "nvram.h"
#include "Thermocouple_MAX31855.h"
#include "StateMachine.h"
#include "pit.h"

struct VRAM vram;

enum STATEStyp
{
	S_BOOTUP,
	S_OPERATION,
	S_OFF,
	S_OPEN_LOOP,
	S_SETUP,
	S_START_BL
};

void setPowerDuty(unsigned int pcnt);
int16_t PID(float setpoint, float actual_position);

#define PID_MAX_OUT	95
#define PID_MIN_OUT	0

#define STEP_TIME_MS	25		// 25 ms per cycle
#define ms2steps(x)	(x / STEP_TIME_MS)

#define OPENLOOP_TEMP_VALUE	650
#define LOOP_DETECTION_TIME	160

uint8_t actitivityDetectionEnabled = 0;
uint8_t setupMode = 0;

static int g_InitState = 0;
static enum STATEStyp g_State = S_BOOTUP;

void btn_ok_handler()
{
	if (g_State != S_OFF)
		menu_btn_ok();
	else
		resetOffTimer();
}

void btn_dn_handler()
{
	if (g_State != S_OFF)
		menu_btn_down();
}

void btn_up_handler()
{
	if (g_State == S_BOOTUP)
		setupMode = 1;
	else if (g_State != S_OFF)
		menu_btn_up();
}

void resetOffTimer()
{
	vram.OffCntDwn = nvram.OffTimeout * ms2steps(1000);
}

void exitSetupMode()
{
	setupMode = 0;
}

void activityDetected()
{
	//if (actitivityDetectionEnabled)
	resetOffTimer();
}

static void updateTemp()
{
	vram.ADCVal = getThermocoupleADCValue();
	vram.JunctTemp = getJunctionADCTemp();
	vram.CurrTemp = getThermocoupleADCTemp();
}

static void heatControl(float setpoint)
{
	static uint16_t stepCounter = 0;
	switch(stepCounter)
	{
		case ms2steps(250):
			setPowerOff();
			break;
		case ms2steps(275):
			actitivityDetectionEnabled = 1;
			break;
		case ms2steps(300):
			updateTemp();
			vram.PWM_Sp = PID(setpoint, (float)vram.CurrTemp / 10);
			if (vram.PWM_Sp > 0)
			{
				actitivityDetectionEnabled = 0;
				setPowerDuty(vram.PWM_Sp);
			}
			stepCounter = 0;
			break;
	}
	++stepCounter;
}

static uint32_t loopDetectCntr = 0;
void controlStateMachine()
{	
	enum STATEStyp nextState = g_State;
	static uint32_t stepCounter = 0;

	switch(g_State)
	{
		case S_BOOTUP:			
			setPowerOff();
			resetOffTimer();
			setString("boot", 4, 0, 0);

			// set initial temp setpoint
			vram.TempSP = nvram.Tsp;

			if ( stepCounter > ms2steps(1000) ) // after 1s we go to operation
			{
				if (setupMode)
					nextState = S_SETUP;
				else if (nvram.IsOpenLoop == 1)
					nextState = S_OPEN_LOOP;
				else
					nextState = S_OPERATION;
			}
			break;
		case S_SETUP:
			if (g_InitState)
				vram.TempSP = nvram.Tsp;

			updateTemp();

			if (!setupMode)
			{
				if (nvram.IsOpenLoop == 1)
					nextState = S_OPEN_LOOP;
				else
					nextState = S_OPERATION;
			}
			break;
		case S_OPERATION:
		{
			if (g_InitState)
			{

				activateOperMenu();
			}
			
			heatControl(vram.TempSP);
			
			// thermocouple absent detection
			if (vram.CurrTemp < OPENLOOP_TEMP_VALUE)
				++loopDetectCntr;
			else
				loopDetectCntr = 0;
			if (loopDetectCntr > LOOP_DETECTION_TIME)
			{
				loopDetectCntr = 0;
				nvram.IsOpenLoop = 1;
			}

			if (nvram.IsOpenLoop == 1)
				nextState = S_OPEN_LOOP;
			
			if (--vram.OffCntDwn <= 0)
				nextState = S_OFF;
			break;
		}
		case S_OPEN_LOOP:
			if (g_InitState)
			{
				vram.PWM_Sp = 2;
				vram.CurrTemp = 0;
				activateManualMenu();
			}

			if (vram.PWM_Sp < 0)
				vram.PWM_Sp = 0;
			setPowerDuty(vram.PWM_Sp);

			if (nvram.IsOpenLoop == 0)
				nextState = S_OPERATION; // go back to automatic operation
			if (--vram.OffCntDwn <= 0)
				nextState = S_OFF;
			break;
		case S_OFF:
			if (g_InitState)
				setPowerOff();
			updateTemp();
			
			if  ( !(stepCounter % ms2steps(1000) ) )
				setString("OFF", 3, 0, 0);
			if  ( !(stepCounter % ms2steps(2000) ) )
			{
				char buff[10];
				sprintf(buff, "%hd*", vram.CurrTemp / 10);
				rightAlign(buff);
				setString(buff, strlen(buff), 0, 0);
			}
			
			if (vram.OffCntDwn > 0)
				nextState = nvram.IsOpenLoop ? S_OPEN_LOOP : S_OPERATION;
			break;
		case S_START_BL:
			break;	
	}
	
	if (S_BOOTUP != g_State && S_OFF != g_State
			&& !(stepCounter % ms2steps(100)) ) // we update display each 100 ms
		updateDisplay();

	if (nextState != g_State) // we changed state this time
	{
		g_InitState = 1;
		g_State = nextState;
	}
	else
		g_InitState = 0;
	
	++stepCounter;
}

int16_t PID(float setpoint, float actual_position)
{
	#define epsilon 3f
	#define dt 		0.250f; // 250 ms
	static float pre_error = 0;
	static float integral = 0;
	float error;
	float proportional;
	float derivative;
	float output;
	
	error = setpoint - actual_position;

	//CaculateP,I,D
	float kp = (float)nvram.Kp / 100.0;
	float ki = (float)nvram.Ki / 100.0;
	float kd = (float)nvram.Kd / 100.0;
	
	// Prop
	proportional = kp * error;
	
	// Integral
	integral += ki * error * dt;
	if (integral > 10) // antiwindup
		integral = 10;
	else if (integral < 0)
		integral = 0;

	// Deriv
	derivative = kd * (error - pre_error) / dt;
	
	output = proportional + integral + derivative;
	
	//Saturation Filter
	if(output > PID_MAX_OUT)
		output = PID_MAX_OUT;
	else if(output < PID_MIN_OUT)
		output = PID_MIN_OUT;

	//Update error
	pre_error = error;
	return (int16_t)(output * 10);
}


static PIT_ConfigType  sPITConfig;

#define BUS_CLOCK	24000000 //Hz

void initStateMachine()
{
	activityDetected();

	/* configure PIT module in chain mode */
	/* PIT clock source is bus clock,24MHz */
	/* PIT channel 0 load value = (600000-1) */
	uint32_t u32LoadValue0   	 = (BUS_CLOCK / 1000) * STEP_TIME_MS - 1; /* 25ms */
	sPITConfig.u32LoadValue      = u32LoadValue0;
	sPITConfig.bFreeze           = FALSE;
	sPITConfig.bModuleDis        = FALSE;    /*!< enable PIT module */
	sPITConfig.bInterruptEn      = TRUE;
	sPITConfig.bChainMode        = FALSE;
	sPITConfig.bTimerEn          = TRUE;

	PIT_Init(PIT_CHANNEL1, &sPITConfig);

	PIT_SetCallback(PIT_CHANNEL1, controlStateMachine);
}
