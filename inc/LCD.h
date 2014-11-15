/**
* FILE:         LCD.h
* DESCRIPTION:  LCD Interface for STM32F4
* AUTHOR:       Sharan ***REMOVED***
* COPYRIGHT:    Copyright (c) 2014 Sharan ***REMOVED***
* VERSION:      1.0.0
* LICENSE:      Creative Commons Attribution-ShareAlike
* NOTE:         Code has been ported from the Arduino LCD Library
*               to work with the STM32F4. This library will
*               therefore share the same license.
* CHANGES:
*               - additional functions/calls to interface with GPIO
*               - removal of unused variables
*               - simplification of low-level functions
*               - changed parameter to row x col instead of col x row
*               - uses Time library to implement delayMicroseconds
*/

#ifndef __LCD_H
#define __LCD_H

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "time.h"

#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

#define HIGH    Bit_SET
#define LOW     Bit_RESET

typedef struct
{
    GPIO_TypeDef* gpio;
    uint16_t rs, rw, enable, data[8], rows;
    uint8_t dispFunc, dispCtrl, dispMode;
} lcd_conf_t;

void LCD_Init(
    uint8_t fourBitMode, GPIO_TypeDef* gpio,
    uint16_t _rs, uint16_t _rw, uint16_t _enable,
    uint16_t _d0, uint16_t _d1, uint16_t _d2, uint16_t _d3,
    uint16_t _d4, uint16_t _d5, uint16_t _d6, uint16_t _d7);

void LCD_4BitInit(
    GPIO_TypeDef* gpio, uint16_t _rs, uint16_t _rw, uint16_t _enable,
    uint16_t _d0, uint16_t _d1, uint16_t _d2, uint16_t _d3);

void LCD_Begin(uint8_t rows, uint8_t cols, uint8_t dotSize);
void LCD_clear(void);
void LCD_home(void);
void LCD_setCursor(uint8_t row, uint8_t col);
void LCD_noDisplay(void);
void LCD_display(void);
void LCD_noCursor(void);
void LCD_cursor(void);
void LCD_noBlink(void);
void LCD_blink(void);
void LCD_scrollDisplayLeft(void);
void LCD_scrollDisplayRight(void);
void LCD_leftToRight(void);
void LCD_rightToLeft(void);
void LCD_autoScroll(void);
void LCD_noAutoScroll(void);
void LCD_createChar(uint8_t location, uint8_t udg[]);
void LCD_put(const char c);
void LCD_printf(const char *fmt, ...);

#endif
