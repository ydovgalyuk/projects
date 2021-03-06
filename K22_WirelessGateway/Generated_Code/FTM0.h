/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : FTM0.h
**     Project     : K22_WirelessGateway
**     Processor   : MK22FN1M0VLH12
**     Component   : Init_FTM
**     Version     : Component 01.010, Driver 01.10, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-04-02, 22:12, # CodeGen: 0
**     Abstract    :
**          This file implements the FTM (FTM0) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : FTM0
**          Device                                         : FTM0
**          Settings                                       : 
**            Clock gate                                   : Enabled
**            Clock settings                               : 
**              Clock source                               : System clock
**              Prescaler                                  : divide by 4
**              Counter init                               : 1
**              Modulo counter                             : 150
**              Period                                     : 10.000 us
**              Deadtime                                   : 
**                Deadtime prescaler                       : divide by 1
**                Deadtime value                           : 0
**                Deadtime delay                           : DISABLED
**              Num of timer overflows                     : 0
**            FTM mode                                     : Disabled
**            BDM mode                                     : FTM counter stopped; channels outputs functional
**            Global time base                             : Disabled
**            Global time base output                      : Disabled
**            Write protection                             : Disabled
**            Combined channels                            : 
**              Channels 0 and 1                           : 
**                Complementary mode                       : Disabled
**                Fault control                            : Disabled
**                Synchronization                          : Disabled
**                Deadtime                                 : Disabled
**                Inverting                                : Disabled
**              Channels 2 and 3                           : 
**                Complementary mode                       : Disabled
**                Fault control                            : Disabled
**                Synchronization                          : Disabled
**                Deadtime                                 : Disabled
**                Inverting                                : Disabled
**              Channels 4 and 5                           : 
**                Complementary mode                       : Disabled
**                Fault control                            : Disabled
**                Synchronization                          : Disabled
**                Deadtime                                 : Disabled
**                Inverting                                : Disabled
**              Channels 6 and 7                           : 
**                Complementary mode                       : Disabled
**                Fault control                            : Disabled
**                Synchronization                          : Disabled
**                Deadtime                                 : Disabled
**                Inverting                                : Disabled
**            Synchronization                              : 
**              Synchronization mode                       : Disabled
**                Counter reinitialization                 : Disabled
**              HW trigger mode                            : Enabled
**              CNTIN synchronization                      : Disabled
**              INVCTRL synchronization                    : Disabled
**              SWOCTRL synchronization                    : Disabled
**              Software Trigger                           : Disabled
**              OUTMASK synchronization                    : Disabled
**              Minimum loading point                      : Disabled
**              Maximum loading point                      : Disabled
**            External triggers                            : 
**              Counter init trigger                       : Disabled
**              Channel 0 trigger                          : Disabled
**              Channel 1 trigger                          : Disabled
**              Channel 2 trigger                          : Disabled
**              Channel 3 trigger                          : Disabled
**              Channel 4 trigger                          : Disabled
**              Channel 5 trigger                          : Disabled
**            Fault control settings                       : 
**              Fault control mode                         : Disabled
**              Fault input filter value                   : Disabled
**              Fault input 0 filter                       : Disabled
**              Fault input 0 polarity                     : High
**              Fault input 1 filter                       : Disabled
**              Fault input 1 polarity                     : High
**              Fault input 2 filter                       : Disabled
**              Fault input 2 polarity                     : High
**              Fault input 3 filter                       : Disabled
**              Fault input 3 polarity                     : High
**          Channels                                       : 
**            Channel 0                                    : Disabled
**            Channel 1                                    : Disabled
**            Channel 2                                    : Disabled
**            Channel 3                                    : Disabled
**            Channel 4                                    : Disabled
**            Channel 5                                    : Disabled
**            Channel 6                                    : Disabled
**            Channel 7                                    : Disabled
**          Pins                                           : 
**            Fault pins                                   : 
**              Fault pin 0                                : Disabled
**              Fault pin 1                                : Disabled
**              Fault pin 2                                : Disabled
**              Fault pin 3                                : Disabled
**            External clock pin                           : Disabled
**            HW Synchronization triggers                  : 
**              Hardware Trigger 0                         : Disabled
**              Hardware Trigger 1                         : Disabled
**              Hardware Trigger 2                         : Disabled
**          Interrupts                                     : 
**            Channels / Timer overflow / Fault            : 
**              Interrupt                                  : INT_FTM0
**              Interrupt request                          : Enabled
**              Interrupt priority                         : 8
**              ISR Name                                   : uS_Timer_ISR
**              Channels interrupt/DMA                     : See the respective Channel settings
**              Timer overflow interrupt                   : Enabled
**              Fault interrupt                            : Disabled
**          Initialization                                 : 
**            Initialize the Channels Output               : no
**            Load enable                                  : no
**            Channel 0 load select                        : no
**            Channel 1 load select                        : no
**            Channel 2 load select                        : no
**            Channel 3 load select                        : no
**            Channel 4 load select                        : no
**            Channel 5 load select                        : no
**            Channel 6 load select                        : no
**            Channel 7 load select                        : no
**            Call Init method                             : yes
**     Contents    :
**         Init - void FTM0_Init(void);
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
** @file FTM0.h
** @version 01.10
** @brief
**          This file implements the FTM (FTM0) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup FTM0_module FTM0 module documentation
**  @{
*/         

#ifndef FTM0_H_
#define FTM0_H_

/* MODULE FTM0. */

/* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"

/* Peripheral base address parameter */
#define FTM0_DEVICE FTM0_BASE_PTR


/*
** ===================================================================
**     Method      :  FTM0_Init (component Init_FTM)
**     Description :
**         This method initializes registers of the FTM module
**         according to the Peripheral Initialization settings.
**         Call this method in user code to initialize the module. By
**         default, the method is called by PE automatically; see "Call
**         Init method" property of the component for more details.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void FTM0_Init(void);
/*
** ===================================================================
** The interrupt service routine must be implemented by user in one
** of the user modules (see FTM0.c file for more information).
** ===================================================================
*/
PE_ISR(uS_Timer_ISR);


/* END FTM0. */
#endif /* #ifndef __FTM0_H_ */
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
