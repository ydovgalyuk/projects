/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : BitIoLdd12.c
**     Project     : K22_433MHz
**     Processor   : MK22FN1M0VLH12
**     Component   : BitIO_LDD
**     Version     : Component 01.033, Driver 01.03, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-03-31, 13:26, # CodeGen: 34
**     Abstract    :
**         The HAL BitIO component provides a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL BitIO API are simpler and more
**         portable to various microprocessors.
**     Settings    :
**          Component name                                 : BitIoLdd12
**          Pin for I/O                                    : CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB
**          Pin signal                                     : 
**          Direction                                      : Output
**          Initialization                                 : 
**            Init. direction                              : Output
**            Init. value                                  : 0
**            Auto initialization                          : yes
**          Safe mode                                      : yes
**     Contents    :
**         Init   - LDD_TDeviceData* BitIoLdd12_Init(LDD_TUserData *UserDataPtr);
**         GetVal - bool BitIoLdd12_GetVal(LDD_TDeviceData *DeviceDataPtr);
**         ClrVal - void BitIoLdd12_ClrVal(LDD_TDeviceData *DeviceDataPtr);
**         SetVal - void BitIoLdd12_SetVal(LDD_TDeviceData *DeviceDataPtr);
**         NegVal - void BitIoLdd12_NegVal(LDD_TDeviceData *DeviceDataPtr);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file BitIoLdd12.c
** @version 01.03
** @brief
**         The HAL BitIO component provides a low level API for unified
**         access to general purpose digital input/output pins across
**         various device designs.
**
**         RTOS drivers using HAL BitIO API are simpler and more
**         portable to various microprocessors.
*/         
/*!
**  @addtogroup BitIoLdd12_module BitIoLdd12 module documentation
**  @{
*/         

/* MODULE BitIoLdd12. */

#include "FreeRTOS.h" /* FreeRTOS interface */
#include "BitIoLdd12.h"

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct {
  LDD_TUserData *UserDataPtr;          /* Pointer to user data */
} BitIoLdd12_TDeviceData;              /* Device data structure type */

typedef BitIoLdd12_TDeviceData *BitIoLdd12_TDeviceDataPtr ; /* Pointer to the device data structure. */

/* {FreeRTOS RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static BitIoLdd12_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/*
** ===================================================================
**     Method      :  BitIoLdd12_Init (component BitIO_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the "Enable
**         in init. code" is set to "yes" value then the device is also
**         enabled(see the description of the Enable() method). In this
**         case the Enable() method is not necessary and needn't to be
**         generated. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Pointer to the dynamically allocated private
**                           structure or NULL if there was an error.
*/
/* ===================================================================*/
LDD_TDeviceData* BitIoLdd12_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate device structure */
  BitIoLdd12_TDeviceDataPtr DeviceDataPrv;

  /* {FreeRTOS RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->UserDataPtr = UserDataPtr; /* Store the RTOS device structure */
  /* Configure pin as output */
  /* GPIOA_PDDR: PDD|=0x2000 */
  GPIOA_PDDR |= GPIO_PDDR_PDD(0x2000);
  /* Set initialization value */
  /* GPIOA_PDOR: PDO&=~0x2000 */
  GPIOA_PDOR &= (uint32_t)~(uint32_t)(GPIO_PDOR_PDO(0x2000));
  /* Initialization of Port Control register */
  /* PORTA_PCR13: ISF=0,MUX=1 */
  PORTA_PCR13 = (uint32_t)((PORTA_PCR13 & (uint32_t)~(uint32_t)(
                 PORT_PCR_ISF_MASK |
                 PORT_PCR_MUX(0x06)
                )) | (uint32_t)(
                 PORT_PCR_MUX(0x01)
                ));
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_BitIoLdd12_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv);
}
/*
** ===================================================================
**     Method      :  BitIoLdd12_GetVal (component BitIO_LDD)
*/
/*!
**     @brief
**         Returns the input/output value. If the direction is [input]
**         then the input value of the pin is read and returned. If the
**         direction is [output] then the last written value is read
**         and returned (see <Safe mode> property for limitations).
**         This method cannot be disabled if direction is [input].
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @return
**                         - Input or output value. Possible values:
**                           <false> - logical "0" (Low level)
**                           <true> - logical "1" (High level)
*/
/* ===================================================================*/
bool BitIoLdd12_GetVal(LDD_TDeviceData *DeviceDataPtr)
{
  uint32_t PortData;                   /* Port data masked according to the bit used */

  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  PortData = GPIO_PDD_GetPortDataOutput(BitIoLdd12_MODULE_BASE_ADDRESS) & BitIoLdd12_PORT_MASK;
  return (PortData != 0U) ? (bool)TRUE : (bool)FALSE;
}

/*
** ===================================================================
**     Method      :  BitIoLdd12_ClrVal (component BitIO_LDD)
*/
/*!
**     @brief
**         Clears (set to zero) the output value. It is equivalent to
**         the [PutVal(FALSE)]. This method is available only if the
**         direction = _[output]_ or _[input/output]_.
**     @param
**         DeviceDataPtr   - Pointer to device data
**                           structure returned by <Init> method.
*/
/* ===================================================================*/
void BitIoLdd12_ClrVal(LDD_TDeviceData *DeviceDataPtr)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_ClearPortDataOutputMask(BitIoLdd12_MODULE_BASE_ADDRESS, BitIoLdd12_PORT_MASK);
}

/*
** ===================================================================
**     Method      :  BitIoLdd12_SetVal (component BitIO_LDD)
*/
/*!
**     @brief
**         Sets (to one) the output value. It is equivalent to the
**         [PutVal(TRUE)]. This method is available only if the
**         direction = _[output]_ or _[input/output]_.
**     @param
**         DeviceDataPtr   - Pointer to device data
**                           structure returned by <Init> method.
*/
/* ===================================================================*/
void BitIoLdd12_SetVal(LDD_TDeviceData *DeviceDataPtr)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_SetPortDataOutputMask(BitIoLdd12_MODULE_BASE_ADDRESS, BitIoLdd12_PORT_MASK);
}

/*
** ===================================================================
**     Method      :  BitIoLdd12_NegVal (component BitIO_LDD)
*/
/*!
**     @brief
**         Negates (inverts) the output value. It is equivalent to the
**         [PutVal(!GetVal())]. This method is available only if the
**         direction = _[output]_ or _[input/output]_.
**     @param
**         DeviceDataPtr   - Pointer to device data
**                           structure returned by <Init> method.
*/
/* ===================================================================*/
void BitIoLdd12_NegVal(LDD_TDeviceData *DeviceDataPtr)
{
  (void)DeviceDataPtr;                 /* Parameter is not used, suppress unused argument warning */
  GPIO_PDD_TogglePortDataOutputMask(BitIoLdd12_MODULE_BASE_ADDRESS, BitIoLdd12_PORT_MASK);
}

/* END BitIoLdd12. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
