/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */

/* Begin of <includes> initialization, DO NOT MODIFY LINES BELOW */

#include "rtTasks.h"
#include "FRTOS1.h"
#include "frtos_tasks.h"

/* End <includes> initialization, DO NOT MODIFY LINES ABOVE */

static portTASK_FUNCTION(_433MHzTask, pvParameters) {

	/* Write your task initialization code here ... */

	for (;;) {
		/* Write your task code here ... */
		_433MHz_sniffer();
		/* You can use a task delay like
		 vTaskDelay(1000/portTICK_RATE_MS);
		 */
	}
	/* Destroy the task */
	vTaskDelete(_433MHzTask);
}

static portTASK_FUNCTION(_2_4GHzTask, pvParameters) {

	/* Write your task initialization code here ... */

	for (;;) {
		/* Write your task code here ... */
		_2_4GHz_Entry();
		/* You can use a task delay like
		 vTaskDelay(1000/portTICK_RATE_MS);
		 */
	}
	/* Destroy the task */
	vTaskDelete(_2_4GHzTask);
}

static portTASK_FUNCTION(Gateway, pvParameters) {

	/* Write your task initialization code here ... */

	for (;;) {
		/* Write your task code here ... */
		GatewayEntry();
		/* You can use a task delay like
		 vTaskDelay(1000/portTICK_RATE_MS);
		 */
	}
	/* Destroy the task */
	vTaskDelete(Gateway);
}

static portTASK_FUNCTION(InfoLED, pvParameters) {

	/* Write your task initialization code here ... */

	for (;;) {
		/* Write your task code here ... */

		/* You can use a task delay like
		 vTaskDelay(1000/portTICK_RATE_MS);
		 */
	}
	/* Destroy the task */
	vTaskDelete(InfoLED);
}

static portTASK_FUNCTION(RFIDTask, pvParameters) {

	/* Write your task initialization code here ... */

	PCD_Init();
	vTaskDelay(500/portTICK_RATE_MS);
	for (;;) {
		/* Write your task code here ... */
		mfrc522_loop();
		/* You can use a task delay like
		 vTaskDelay(1000/portTICK_RATE_MS);
		 */
	}
	/* Destroy the task */
	vTaskDelete(RFIDTask);
}

void CreateTasks(void) {
	if (FRTOS1_xTaskCreate(
			_433MHzTask, /* pointer to the task */
			"433MHz", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			_2_4GHzTask, /* pointer to the task */
			"2.4GHz", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			Gateway, /* pointer to the task */
			"Gateway", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			InfoLED, /* pointer to the task */
			"InfoLED", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			RFIDTask, /* pointer to the task */
			"RFID", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			(xTaskHandle*)NULL /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
}

