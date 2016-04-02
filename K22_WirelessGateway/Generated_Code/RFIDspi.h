/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : RFIDspi.h
**     Project     : K22_WirelessGateway
**     Processor   : MK22FN1M0VLH12
**     Component   : GenericSWSPI
**     Version     : Component 01.024, Driver 01.15, CPU db: 3.00.000
**     Repository  : McuOnEclipse
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-04-02, 22:12, # CodeGen: 0
**     Abstract    :
**
**     Contents    :
**         RecvChar              - byte RFIDspi_RecvChar(byte *Chr);
**         SendChar              - byte RFIDspi_SendChar(byte val);
**         CharsInRxBuf          - byte RFIDspi_CharsInRxBuf(void);
**         CharsInTxBuf          - byte RFIDspi_CharsInTxBuf(void);
**         SetShiftClockPolarity - byte RFIDspi_SetShiftClockPolarity(byte Edge);
**         SetIdleClockPolarity  - byte RFIDspi_SetIdleClockPolarity(byte Level);
**         Write_ReadDummy       - void RFIDspi_Write_ReadDummy(byte val);
**         SetSlowMode           - void RFIDspi_SetSlowMode(void);
**         SetFastMode           - void RFIDspi_SetFastMode(void);
**
**     License : Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2014-2015, all rights reserved.
**     Web       : www.mcuoneclipse.com
**     This an open source software implemented with using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file RFIDspi.h
** @version 01.15
** @brief
**
*/         
/*!
**  @addtogroup RFIDspi_module RFIDspi module documentation
**  @{
*/         

#ifndef __RFIDspi
#define __RFIDspi

/* MODULE RFIDspi. */

void RFIDspi_SetFastMode(void);
/*
** ===================================================================
**     Method      :  RFIDspi_SetFastMode (component GenericSWSPI)
**     Description :
**         Sets the fast mode
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RFIDspi_SetSlowMode(void);
/*
** ===================================================================
**     Method      :  RFIDspi_SetSlowMode (component GenericSWSPI)
**     Description :
**         Changes to slow mode
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "Clock1.h"
#include "Input1.h"
#include "Output1.h"
#include "WAIT1.h"

#include "Cpu.h"



#ifndef __BWUserType_TPtrByte
#define __BWUserType_TPtrByte
  typedef byte* TPtrByte ; /* Pointer to byte */
#endif

void RFIDspi_Write_ReadDummy(byte val);
/*
** ===================================================================
**     Method      :  RFIDspi_Write_ReadDummy (component GenericSWSPI)
**     Description :
**         Writes to the bus, but does not read.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - value to put on the bus
**     Returns     : Nothing
** ===================================================================
*/

byte RFIDspi_RecvChar(byte *Chr);
/*
** ===================================================================
**     Method      :  RFIDspi_RecvChar (component GenericSWSPI)
**     Description :
**         If any data received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - A pointer to the received character.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_RXEMPTY - No data in receiver
**                           ERR_OVERRUN - Overrun error is detected
** ===================================================================
*/

byte RFIDspi_SendChar(byte val);
/*
** ===================================================================
**     Method      :  RFIDspi_SendChar (component GenericSWSPI)
**     Description :
**         Sends one character to the channel.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - Character to send.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/

byte RFIDspi_CharsInRxBuf(void);
/*
** ===================================================================
**     Method      :  RFIDspi_CharsInRxBuf (component GenericSWSPI)
**     Description :
**         Returns number of characters in the input buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Returns number of characters in the input
**                           buffer.
** ===================================================================
*/

#define RFIDspi_CharsInTxBuf()  0 /* we always directly send the characters without buffering */
/*
** ===================================================================
**     Method      :  RFIDspi_CharsInTxBuf (component GenericSWSPI)
**     Description :
**         Returns number of characters in the output buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Returns number of characters in the output
**                           buffer.
** ===================================================================
*/

byte RFIDspi_SetShiftClockPolarity(byte Edge);
/*
** ===================================================================
**     Method      :  RFIDspi_SetShiftClockPolarity (component GenericSWSPI)
**     Description :
**         Sets the shift clock polarity at runtime. Output data will
**         be shifted on the selected edge polarity.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Edge            - Edge polarity, possible values:
**                           0-falling edge,
**                           1-rising edge.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/

byte RFIDspi_SetIdleClockPolarity(byte Level);
/*
** ===================================================================
**     Method      :  RFIDspi_SetIdleClockPolarity (component GenericSWSPI)
**     Description :
**         Set the idle clock polarity at runtime. If communication
**         does not run, the clock signal will be set to the required
**         level.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Level           - Idle clock polarity:
**                           0-low
**                           1-high
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
** ===================================================================
*/

void RFIDspi_Init(void);
/*
** ===================================================================
**     Method      :  RFIDspi_Init (component GenericSWSPI)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END RFIDspi. */

#endif /* ifndef __RFIDspi */
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
