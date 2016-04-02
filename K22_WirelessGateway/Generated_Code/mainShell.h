/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : mainShell.h
**     Project     : K22_WirelessGateway
**     Processor   : MK22FN1M0VLH12
**     Component   : Shell
**     Version     : Component 01.079, Driver 01.00, CPU db: 3.00.000
**     Repository  : McuOnEclipse
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-04-02, 22:12, # CodeGen: 0
**     Abstract    :
**
**     Settings    :
**          Component name                                 : mainShell
**          Echo                                           : no
**          Prompt                                         : "CMD> "
**          Project Name                                   : My Project Name
**          Silent Mode Prefix                             : #
**          Blocking Send                                  : Enabled
**            Wait                                         : WAIT1
**            Timeout (ms)                                 : 20
**            Wait Time (ms)                               : 10
**            RTOS Wait                                    : yes
**          Status Colon Pos                               : 13
**          Help Semicolon Pos                             : 26
**          Multi Command                                  : Disabled
**          Utility                                        : UTIL1
**          Default Serial                                 : Enabled
**            Console Interface                            : AS1
**          Semaphore                                      : yes
**          Critical Section                               : CS1
**          History                                        : no
**          Kinetis SDK                                    : KSDK1
**     Contents    :
**         PrintPrompt                  - void mainShell_PrintPrompt(mainShell_ConstStdIOType *io);
**         SendNum8u                    - void mainShell_SendNum8u(uint8_t val, mainShell_StdIO_OutErr_FctType io);
**         SendNum8s                    - void mainShell_SendNum8s(int8_t val, mainShell_StdIO_OutErr_FctType io);
**         SendNum16u                   - void mainShell_SendNum16u(uint16_t val, mainShell_StdIO_OutErr_FctType io);
**         SendNum16s                   - void mainShell_SendNum16s(int16_t val, mainShell_StdIO_OutErr_FctType io);
**         SendNum32u                   - void mainShell_SendNum32u(uint32_t val, mainShell_StdIO_OutErr_FctType io);
**         SendNum32s                   - void mainShell_SendNum32s(int32_t val, mainShell_StdIO_OutErr_FctType io);
**         SendStr                      - void mainShell_SendStr(const uint8_t *str, mainShell_StdIO_OutErr_FctType io);
**         SendData                     - void mainShell_SendData(const uint8_t *data, uint16_t dataSize,...
**         PrintStatus                  - uint8_t mainShell_PrintStatus(mainShell_ConstStdIOType *io);
**         ParseCommand                 - uint8_t mainShell_ParseCommand(const uint8_t *cmd, bool *handled,...
**         IsHistoryCharacter           - bool mainShell_IsHistoryCharacter(uint8_t ch, uint8_t *cmdBuf, size_t...
**         ReadLine                     - bool mainShell_ReadLine(uint8_t *bufStart, uint8_t *buf, size_t bufSize,...
**         PrintCommandFailed           - void mainShell_PrintCommandFailed(const uint8_t *cmd,...
**         IterateTable                 - uint8_t mainShell_IterateTable(const uint8_t *cmd, bool *handled,...
**         SetStdio                     - uint8_t mainShell_SetStdio(mainShell_ConstStdIOTypePtr stdio);
**         GetStdio                     - mainShell_ConstStdIOTypePtr mainShell_GetStdio(void);
**         RequestSerial                - void mainShell_RequestSerial(void);
**         ReleaseSerial                - void mainShell_ReleaseSerial(void);
**         ReadAndParseWithCommandTable - uint8_t mainShell_ReadAndParseWithCommandTable(uint8_t *cmdBuf, size_t...
**         ParseWithCommandTable        - uint8_t mainShell_ParseWithCommandTable(const uint8_t *cmd,...
**         GetSemaphore                 - void* mainShell_GetSemaphore(void);
**         SendStatusStr                - void mainShell_SendStatusStr(const uint8_t *strItem, const uint8_t...
**         SendHelpStr                  - void mainShell_SendHelpStr(const uint8_t *strCmd, const uint8_t *strHelp,...
**         ReadChar                     - void mainShell_ReadChar(uint8_t *c);
**         SendChar                     - void mainShell_SendChar(uint8_t ch);
**         KeyPressed                   - bool mainShell_KeyPressed(void);
**         Init                         - void mainShell_Init(void);
**         Deinit                       - void mainShell_Deinit(void);
**
**     License   :  Open Source (LGPL)
**     Copyright : (c) Copyright Erich Styger, 2014-2016, all rights reserved.
**     http      : http://www.mcuoneclipse.com
**     This an open source software implementing a command line shell with Processor Expert.
**     This is a free software and is opened for education,  research  and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file mainShell.h
** @version 01.00
** @brief
**
*/         
/*!
**  @addtogroup mainShell_module mainShell module documentation
**  @{
*/         


#ifndef __mainShell_H
#define __mainShell_H

/* MODULE mainShell. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "Cpu.h"
#include <stddef.h> /* for size_t */


#ifndef __BWUserType_mainShell_StdIO_OutErr_FctType
#define __BWUserType_mainShell_StdIO_OutErr_FctType
  typedef void (*mainShell_StdIO_OutErr_FctType)(uint8_t); /* Callback for an output or error I/O function */
#endif
#ifndef __BWUserType_mainShell_StdIO_In_FctType
#define __BWUserType_mainShell_StdIO_In_FctType
  typedef void (*mainShell_StdIO_In_FctType)(uint8_t *); /* Callback for an I/O input function. */
#endif
#ifndef __BWUserType_mainShell_StdIO_KeyPressed_FctType
#define __BWUserType_mainShell_StdIO_KeyPressed_FctType
  typedef bool (*mainShell_StdIO_KeyPressed_FctType)(void); /* Callback which returns true if a key has been pressed */
#endif
#ifndef __BWUserType_mainShell_StdIOType
#define __BWUserType_mainShell_StdIOType
  typedef struct {                     /* Record containing input, output and error callback (stdin, stdout, stderr). */
    mainShell_StdIO_In_FctType stdIn;  /* standard input */
    mainShell_StdIO_OutErr_FctType stdOut; /* standard output */
    mainShell_StdIO_OutErr_FctType stdErr; /* standard error */
    mainShell_StdIO_KeyPressed_FctType keyPressed; /* key pressed callback */
  } mainShell_StdIOType;
#endif
#ifndef __BWUserType_mainShell_ConstStdIOType
#define __BWUserType_mainShell_ConstStdIOType
  typedef const mainShell_StdIOType mainShell_ConstStdIOType; /* constant StdIOType */
#endif
#ifndef __BWUserType_mainShell_ParseCommandCallback
#define __BWUserType_mainShell_ParseCommandCallback
  typedef uint8_t (*mainShell_ParseCommandCallback)(const uint8_t *cmd, bool *handled, const mainShell_StdIOType *io); /* Callback for parsing a shell command */
#endif
#ifndef __BWUserType_mainShell_ConstStdIOTypePtr
#define __BWUserType_mainShell_ConstStdIOTypePtr
  typedef const mainShell_ConstStdIOType *mainShell_ConstStdIOTypePtr; /* Pointer to constant standard I/O descriptor */
#endif
#ifndef __BWUserType_mainShell_ConstParseCommandCallback
#define __BWUserType_mainShell_ConstParseCommandCallback
  typedef const mainShell_ParseCommandCallback mainShell_ConstParseCommandCallback; /* Callback for parsing a shell command */
#endif


/* settings for command line history */
#define mainShell_HISTORY_ENABLED  0   /* 1: enabled, 0: disabled */
#define mainShell_NOF_HISTORY      0   /* number of items in history */
#define mainShell_HIST_LEN         0   /* history buffer size */

/* settings for silent prefix char */
#define mainShell_SILENT_PREFIX_CHAR    '#' /* with this char as first character in the cmd, printing is silent. Use a space to disable it */
#define mainShell_NO_SILENT_PREFIX_CHAR ' ' /* used for no silent prefix char */
#define mainShell_SILENT_PREFIX_CHAR_ENABLED (mainShell_SILENT_PREFIX_CHAR != mainShell_NO_SILENT_PREFIX_CHAR)

/* multi command support */
#define mainShell_MULTI_CMD_ENABLED   0 /* 1: enabled, 0: disabled */
#define mainShell_MULTI_CMD_SIZE      0 /* max size of each command */
#define mainShell_MULTI_CMD_CHAR      ';' /* separation character */

/* settings for local echo */
#define mainShell_ECHO_ENABLED  0      /* 1: enabled, 0: disabled */

#define mainShell_DEFAULT_SERIAL  1 /* If set to 1, then the shell implements its own StdIO which is returned by mainShell_GetStdio(); */

#define mainShell_DASH_LINE "--------------------------------------------------------------"
/* predefined commands */
#define mainShell_CMD_HELP   "help"
#define mainShell_CMD_STATUS "status"
void mainShell_SendStr(const uint8_t *str, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendStr (component Shell)
**     Description :
**         Prints a string using an I/O function
**     Parameters  :
**         NAME            - DESCRIPTION
**       * str             - Pointer to string (zero terminated) to be
**                           printed.
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

uint8_t mainShell_ParseCommand(const uint8_t *cmd, bool *handled, mainShell_ConstStdIOType *io);
/*
** ===================================================================
**     Method      :  mainShell_ParseCommand (component Shell)
**     Description :
**         Parses a shell command. Use 'help' to get a list of
**         supported commands.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**       * handled         - Pointer to variable to indicate if
**                           the command has been handled. The caller
**                           passes this variable to the command scanner
**                           to find out if the passed command has been
**                           handled. The variable is initialized by the
**                           caller.
**       * io              - Pointer to I/O callbacks
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void mainShell_SendNum32s(int32_t val, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendNum32s (component Shell)
**     Description :
**         Sends a 32bit signed number to the given I/O
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - number to print
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendNum16s(int16_t val, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendNum16s (component Shell)
**     Description :
**         Sends a 16bit signed number to the given I/O
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - number to print
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_PrintPrompt(mainShell_ConstStdIOType *io);
/*
** ===================================================================
**     Method      :  mainShell_PrintPrompt (component Shell)
**     Description :
**         Prints the prompt to the stdOut channel
**     Parameters  :
**         NAME            - DESCRIPTION
**       * io              - Pointer to IO to be used
**     Returns     : Nothing
** ===================================================================
*/

bool mainShell_ReadLine(uint8_t *bufStart, uint8_t *buf, size_t bufSize, mainShell_ConstStdIOType *io);
/*
** ===================================================================
**     Method      :  mainShell_ReadLine (component Shell)
**     Description :
**         Reads a line from stdIn and returns TRUE if we have a line,
**         FALSE otherwise.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * bufStart        - Pointer to start of buffer
**       * buf             - Pointer to buffer where to read in the
**                           information
**         bufSize         - size of buffer
**       * io              - Pointer to I/O callbacks
**     Returns     :
**         ---             - TRUE if something has been read, FALSE
**                           otherwise
** ===================================================================
*/

uint8_t mainShell_PrintStatus(mainShell_ConstStdIOType *io);
/*
** ===================================================================
**     Method      :  mainShell_PrintStatus (component Shell)
**     Description :
**         Prints various available system status information
**     Parameters  :
**         NAME            - DESCRIPTION
**       * io              - Pointer to I/O callbacks
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void mainShell_PrintCommandFailed(const uint8_t *cmd, mainShell_ConstStdIOType *io);
/*
** ===================================================================
**     Method      :  mainShell_PrintCommandFailed (component Shell)
**     Description :
**         Prints a standard message for failed or unknown commands
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command which was failing
**       * io              - Pointer to I/O callbacks
**     Returns     : Nothing
** ===================================================================
*/

uint8_t mainShell_ParseWithCommandTable(const uint8_t *cmd, mainShell_ConstStdIOType *io, mainShell_ConstParseCommandCallback *parseCallback);
/*
** ===================================================================
**     Method      :  mainShell_ParseWithCommandTable (component Shell)
**     Description :
**         Parses a shell command. It handles first the internal
**         commands and will call the provided callback.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**       * io              - Pointer to I/O callbacks
**       * parseCallback   - Pointer to callback
**                           which will be called to parse commands in
**                           the user application, or NULL if not used.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

mainShell_ConstStdIOTypePtr mainShell_GetStdio(void);
/*
** ===================================================================
**     Method      :  mainShell_GetStdio (component Shell)
**     Description :
**         Returns the default stdio channel. This method is only
**         available if a shell is enabled in the component properties.
**     Parameters  : None
**     Returns     :
**         ---             - Pointer to the stdio descriptor
** ===================================================================
*/

void mainShell_SendNum32u(uint32_t val, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendNum32u (component Shell)
**     Description :
**         Sends a 32bit unsigned number to the given I/O
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - number to print
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendNum16u(uint16_t val, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendNum16u (component Shell)
**     Description :
**         Sends a 16bit unsigned number to the given I/O
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - number to print
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendNum8u(uint8_t val, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendNum8u (component Shell)
**     Description :
**         Sends an 8bit unsigned number to the given I/O
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - number to print
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendNum8s(int8_t val, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendNum8s (component Shell)
**     Description :
**         Sends an 8bit signed number to the given I/O
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - number to print
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_Init(void);
/*
** ===================================================================
**     Method      :  mainShell_Init (component Shell)
**     Description :
**         Initializes the module, especially creates the mutex
**         semaphore if an RTOS is used.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_RequestSerial(void);
/*
** ===================================================================
**     Method      :  mainShell_RequestSerial (component Shell)
**     Description :
**         Used to get mutual access to the shell console. Only has an
**         effect if using an RTOS with semaphore for the console
**         access.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_ReleaseSerial(void);
/*
** ===================================================================
**     Method      :  mainShell_ReleaseSerial (component Shell)
**     Description :
**         Used to release mutual access to the shell console. Only has
**         an effect if using an RTOS with semaphore for the console
**         access.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendHelpStr(const uint8_t *strCmd, const uint8_t *strHelp, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendHelpStr (component Shell)
**     Description :
**         Prints a string using an I/O function, formated for the
**         'help' command
**     Parameters  :
**         NAME            - DESCRIPTION
**       * strCmd          - Pointer to string of the command
**       * strHelp         - Pointer to help text string
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendStatusStr(const uint8_t *strItem, const uint8_t *strStatus, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendStatusStr (component Shell)
**     Description :
**         Prints a status string using an I/O function, formated for
**         the 'status' command
**     Parameters  :
**         NAME            - DESCRIPTION
**       * strItem         - Pointer to string of the command
**       * strStatus       - Pointer to help text string
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_ReadChar(uint8_t *c);
/*
** ===================================================================
**     Method      :  mainShell_ReadChar (component Shell)
**     Description :
**         Reads a character (blocking)
**     Parameters  :
**         NAME            - DESCRIPTION
**       * c               - Pointer to character to be used to store the
**                           result
**     Returns     : Nothing
** ===================================================================
*/

void mainShell_SendChar(uint8_t ch);
/*
** ===================================================================
**     Method      :  mainShell_SendChar (component Shell)
**     Description :
**         Sends a character (blocking)
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - character to be sent
**     Returns     : Nothing
** ===================================================================
*/

bool mainShell_KeyPressed(void);
/*
** ===================================================================
**     Method      :  mainShell_KeyPressed (component Shell)
**     Description :
**         Checks if a key has been pressed (a character is present in
**         the input buffer)
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void mainShell_Deinit(void);
/*
** ===================================================================
**     Method      :  mainShell_Deinit (component Shell)
**     Description :
**         De-Initializes the module, especially frees the mutex
**         semaphore if an RTOS is used.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void* mainShell_GetSemaphore(void);
/*
** ===================================================================
**     Method      :  mainShell_GetSemaphore (component Shell)
**     Description :
**         Return the semaphore of the shell.
**     Parameters  : None
**     Returns     :
**         ---             - semaphore, or NULL if not used or not
**                           allocated.
** ===================================================================
*/

uint8_t mainShell_ReadAndParseWithCommandTable(uint8_t *cmdBuf, size_t cmdBufSize, mainShell_ConstStdIOType *io, mainShell_ConstParseCommandCallback *parseCallback);
/*
** ===================================================================
**     Method      :  mainShell_ReadAndParseWithCommandTable (component Shell)
**     Description :
**         Reads characters from the default input channel and appends
**         it to the buffer. Once a new line has been detected, the
**         line will be parsed using the handlers in the table.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmdBuf          - Pointer to buffer provided by the
**                           caller where to store the command to read
**                           in. Characters will be appended, so make
**                           sure string buffer is initialized with a
**                           zero byte at the beginning.
**         cmdBufSize      - Size of buffer
**       * io              - Pointer to I/O channels to be used
**       * parseCallback   - Pointer to callback
**                           table provided by the user application to
**                           parse commands. The table has a NULL
**                           sentinel.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t mainShell_IterateTable(const uint8_t *cmd, bool *handled, mainShell_ConstStdIOType *io, mainShell_ConstParseCommandCallback *parserTable);
/*
** ===================================================================
**     Method      :  mainShell_IterateTable (component Shell)
**     Description :
**         Parses a shell command. It handles first the internal
**         commands and will call the provided callback.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * cmd             - Pointer to command string
**       * handled         - Pointer to boolean which is set to
**                           TRUE if a command parser has handled the
**                           command.
**       * io              - Pointer to I/O callbacks
**       * parserTable     - Pointer to callback which
**                           will be called to parse commands in the
**                           user application, or NULL if not used.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t mainShell_SetStdio(mainShell_ConstStdIOTypePtr stdio);
/*
** ===================================================================
**     Method      :  mainShell_SetStdio (component Shell)
**     Description :
**         Sets an StdIO structure which is returned by GetStdio()
**     Parameters  :
**         NAME            - DESCRIPTION
**         stdio           - New stdio structure to be used.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void mainShell_SendData(const uint8_t *data, uint16_t dataSize, mainShell_StdIO_OutErr_FctType io);
/*
** ===================================================================
**     Method      :  mainShell_SendData (component Shell)
**     Description :
**         Sends data using an I/O function. Unlike SendStr(), with
**         this method it is possible to send binary data, including
**         zero bytes.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Pointer to data to be sent
**         dataSize        - Number of bytes to be sent.
**         io              - I/O callbacks to be used for printing.
**     Returns     : Nothing
** ===================================================================
*/

bool mainShell_IsHistoryCharacter(uint8_t ch, uint8_t *cmdBuf, size_t cmdBufIdx, bool *isPrev);
/*
** ===================================================================
**     Method      :  mainShell_IsHistoryCharacter (component Shell)
**     Description :
**         Returns TRUE if character is a history character
**     Parameters  :
**         NAME            - DESCRIPTION
**         ch              - current command character
**       * cmdBuf          - Pointer to command line buffer read
**                           so far
**         cmdBufIdx       - Index of character into cmdBuf
**       * isPrev          - Pointer to return value, if it is
**                           'previous' history or not
**     Returns     :
**         ---             - TRUE if it is an accepted history character
** ===================================================================
*/

/* END mainShell. */

#endif
/* ifndef __mainShell_H */
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
