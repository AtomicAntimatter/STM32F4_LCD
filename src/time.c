/**
 * FILE:		time.c
 * DESCRIPTION:	fast time functions
 * AUTHOR:		Sharan ***REMOVED***
 * COPYRIGHT:	Copyright (c) 2014 Sharan ***REMOVED***
 * LICENSE: 	MIT License
 */

#include "time.h"

volatile uint32_t msTicks;
volatile uint32_t usTicks;
volatile uint32_t usDelay;

void Time_Init(void) {
	SystemCoreClockUpdate();
  	SysTick_Config(SystemCoreClock / MICRO_TIME_BASE);
}

void delay(uint32_t nTime) {
	usDelay = nTime*1000;
	while(usDelay);
}

void delayMicroseconds(uint32_t nTime) {
    usDelay = nTime;
    while(usDelay);
}
