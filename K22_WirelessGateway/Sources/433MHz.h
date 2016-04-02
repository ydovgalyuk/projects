/*
 * Arduino.h
 *
 *  Created on: 20 бер. 2016
 *      Author: Yuriy
 */

#ifndef SOURCES_433MHZ_H_
#define SOURCES_433MHZ_H_

#include "PE_Types.h"
//#define boolean	bool
    #define PROGMEM
    #define memcpy_P(dest, src, num) memcpy((dest), (src), (num))
    // Include libraries for RPi:
    #include <string.h> /* memcpy */
    #include <stdlib.h> /* abs */
    #include <stddef.h> /* NULL */
    //#include <wiringPi.h>
    #include <stdint.h>
    #define CHANGE 1
    // The following typedefs are needed to be able to compile RCSwitch.cpp
    // with the RPi C++ compiler (g++)
    #ifdef __cplusplus
        extern "C"{
    #endif
        typedef uint8_t boolean;
        typedef uint8_t byte;
    #ifdef __cplusplus
    }
    #endif

#define LOW		0
#define HIGH	1
#define OUTPUT	1

void attachInterrupt(int nReceiverInterrupt, void (*handleInterrupt)() , int edge);
void detachInterrupt(int nReceiverInterrupt);
void delayMicroseconds( int uSec);
void digitalWrite(int nPin, int state);
long micros();
void pinMode(int nPin, int type);


#endif /* SOURCES_433MHZ_H_ */
