/*
 * main.c
 *
 *  Created on: 11/10/2015
 *      Author: user
 */


#include <stdlib.h>
#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "gpio.h"
#include "sysinit.h"
#include "start.h"
#include "wait.h"
#include "pit.h"
#include "nvram.h"
#include "Thermocouple_MAX31855.h"
#include "StateMachine.h"


void initPowerPWM(void);
void initDisplay(void);
void initHMI(void );
void controlStateMachine(void);

#define UART_MAX_BUFF	50
uint8_t uartBuff[UART_MAX_BUFF];

int main()
{
	/* Perform processor initialization */

	sysinit();	
	
	printf("\nRunning the SolderTermo_v2 project.\r\n");
	
	initTime();

	initNVRAM();
	
	readNVRAM();	
	
	printf("Version %hd\tCRC %hd\r\n", nvram.Version, nvram.nvramCrc);

	initDisplay();

	//initMAX31855();
	initThermocoupleADC();

	initPowerPWM();

	initHMI();

	initButtons();

	initStateMachine();

	int fastLog = 0;
	while(1)
	{
		if (fastLog)
			printf("%hd\t%hd\r\n", vram.ADCVal, vram.CurrTemp);
		else
			printf("%hd\t%hd\t%hd\t%hd\t%hd\t%hd\r\n", vram.CurrTemp / 10, nvram.Tsp, vram.PWM_Sp,
					nvram.Kp, nvram.Ki, nvram.Kd);


		if ( UART_CharPresent(UART2) )
		{
			uint8_t c;
			uint8_t charNo = 0;
			memset(uartBuff, 0, UART_MAX_BUFF);
			printf("Finish your input by ENTER\r\n");
			printf("\r\n");
			do
			{
				c = uartBuff[charNo++] = UART_GetChar(UART2); // wait for char to come
				UART_PutChar(UART2, c);
				WAIT_Waitms(50);
			}
			while ( c != '\r' && charNo < UART_MAX_BUFF );

			switch(uartBuff[0])
			{
				case 'f': fastLog = 1; break;  // fast logging
				case 's': fastLog = 0; break;  // slow logging
				case 'p': nvram.Kp = atoi(uartBuff + 1); break;  // fast logging
				case 'i': nvram.Ki = atoi(uartBuff + 1); break;  // slow logging
				case 'd': nvram.Kd = atoi(uartBuff + 1); break;  // slow logging
			}
		}

		WAIT_Waitms(fastLog ? 20 : 200);
	}

	return 0; // should never come here
}
