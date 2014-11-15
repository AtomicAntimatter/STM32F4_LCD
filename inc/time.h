/**
 * FILE:		time.h
 * DESCRIPTION:	fast time functions
 * AUTHOR:		Sharan ***REMOVED***
 * COPYRIGHT:	Copyright (c) 2014 Sharan ***REMOVED***
 * LICENSE: 	MIT License
 */

#include "stm32f4xx.h"

#define MICRO_TIME_BASE 1000000

void Time_Init(void);
void delay(uint32_t nTime);
void delayMicroseconds(uint32_t nTime);

/* defined in time.s for SysTick speed */
void Process_Time(void);
