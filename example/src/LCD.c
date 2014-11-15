/**
* FILE:         LCD.c
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

#include "LCD.h"

static lcd_conf_t Lcd_Conf;

static void send(uint8_t value, uint8_t mode);
static void pulseEnable(void);
static void writeBits(uint8_t value, uint8_t nbits);
static void set_output(uint8_t output);

void LCD_Init(
    uint8_t fourBitMode, GPIO_TypeDef* gpio,
    uint16_t rs, uint16_t rw, uint16_t enable,
    uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3,
    uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
    uint32_t periph;

    assert_param(IS_GPIO_ALL_PERIPH(gpio));
    assert_param(IS_GPIO_PIN(rs));
    assert_param(IS_GPIO_PIN(rw));
    assert_param(IS_GPIO_PIN(enable));
    assert_param(IS_GPIO_PIN(d0));
    assert_param(IS_GPIO_PIN(d1));
    assert_param(IS_GPIO_PIN(d2));
    assert_param(IS_GPIO_PIN(d3));

    if(!fourBitMode) {
        assert_param(IS_GPIO_PIN(d4));
        assert_param(IS_GPIO_PIN(d5));
        assert_param(IS_GPIO_PIN(d6));
        assert_param(IS_GPIO_PIN(d7));
    }

    Lcd_Conf.gpio = gpio;
    Lcd_Conf.rs = rs;
    Lcd_Conf.rw = rw;
    Lcd_Conf.enable = enable;
    Lcd_Conf.data[0] = d0;
    Lcd_Conf.data[1] = d1;
    Lcd_Conf.data[2] = d2;
    Lcd_Conf.data[3] = d3;
    Lcd_Conf.data[4] = d4;
    Lcd_Conf.data[5] = d5;
    Lcd_Conf.data[6] = d6;
    Lcd_Conf.data[7] = d7;

    if (gpio == GPIOA)
        periph = RCC_AHB1Periph_GPIOA;
    else if (gpio == GPIOB)
        periph = RCC_AHB1Periph_GPIOB;
    else if (gpio == GPIOC)
        periph = RCC_AHB1Periph_GPIOC;
    else if (gpio == GPIOD)
        periph = RCC_AHB1Periph_GPIOD;
    else if (gpio == GPIOE)
        periph = RCC_AHB1Periph_GPIOE;
    else
        return;

    RCC_AHB1PeriphClockCmd(periph, ENABLE);

    set_output(1);

    if(fourBitMode)
        Lcd_Conf.dispFunc = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        Lcd_Conf.dispFunc = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

void LCD_4BitInit(
    GPIO_TypeDef* gpio, uint16_t rs, uint16_t rw, uint16_t enable,
    uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3)
{
    LCD_Init(1, gpio, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

void LCD_Begin(uint8_t rows, uint8_t cols, uint8_t dotSize) {
    if(rows > 1)
        Lcd_Conf.dispFunc |= LCD_2LINE;

    if((dotSize != 0) && (rows == 1))
        Lcd_Conf.dispFunc |= LCD_5x10DOTS;

    delayMicroseconds(50000);

    GPIO_ResetBits(Lcd_Conf.gpio, Lcd_Conf.rs);
    GPIO_ResetBits(Lcd_Conf.gpio, Lcd_Conf.enable);

    if(Lcd_Conf.rw != 255)
        GPIO_ResetBits(Lcd_Conf.gpio, Lcd_Conf.rw);

    if(!(Lcd_Conf.dispFunc & LCD_8BITMODE)) {
        writeBits(0x03, 4);
        delayMicroseconds(4500);

        writeBits(0x03, 4);
        delayMicroseconds(4500);

        writeBits(0x03, 4);
        delayMicroseconds(150);

        writeBits(0x02, 4);
    } else {
        send(LCD_FUNCTIONSET | Lcd_Conf.dispFunc, LOW);
        delayMicroseconds(4500);

        send(LCD_FUNCTIONSET | Lcd_Conf.dispFunc, LOW);
        delayMicroseconds(150);

        send(LCD_FUNCTIONSET | Lcd_Conf.dispFunc, LOW);
    }

    send(LCD_FUNCTIONSET | Lcd_Conf.dispFunc, LOW);

    Lcd_Conf.dispCtrl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_display();
    LCD_clear();

    Lcd_Conf.dispMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    send(LCD_ENTRYMODESET | Lcd_Conf.dispMode, LOW);
}

void LCD_clear(void) {
    send(LCD_CLEARDISPLAY, LOW);
    delayMicroseconds(2000);
}

void LCD_home(void) {
    send(LCD_RETURNHOME, LOW);
    delayMicroseconds(2000);
}

void LCD_setCursor(uint8_t row, uint8_t col) {
    int row_offsets[] = {0x00,0x40,0x14,0x54};
    send(LCD_SETDDRAMADDR | (col + row_offsets[row]), LOW);
}

void LCD_noDisplay(void) {
    Lcd_Conf.dispCtrl &= ~LCD_DISPLAYON;
    send(LCD_DISPLAYCONTROL | Lcd_Conf.dispCtrl, LOW);
}

void LCD_display(void) {
    Lcd_Conf.dispCtrl |= LCD_DISPLAYON;
    send(LCD_DISPLAYCONTROL | Lcd_Conf.dispCtrl, LOW);
}

void LCD_noCursor(void) {
    Lcd_Conf.dispCtrl &= ~LCD_CURSORON;
    send(LCD_DISPLAYCONTROL | Lcd_Conf.dispCtrl, LOW);
}
void LCD_cursor(void) {
    Lcd_Conf.dispCtrl |= LCD_CURSORON;
    send(LCD_DISPLAYCONTROL | Lcd_Conf.dispCtrl, LOW);
}

void LCD_noBlink(void) {
    Lcd_Conf.dispCtrl &= ~LCD_BLINKON;
    send(LCD_DISPLAYCONTROL | Lcd_Conf.dispCtrl, LOW);
}
void LCD_blink(void) {
    Lcd_Conf.dispCtrl |= LCD_BLINKON;
    send(LCD_DISPLAYCONTROL | Lcd_Conf.dispCtrl, LOW);
}

void LCD_scrollDisplayLeft(void) {
    send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, LOW);
}

void LCD_scrollDisplayRight(void) {
    send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, LOW);
}

void LCD_leftToRight(void) {
  Lcd_Conf.dispMode |= LCD_ENTRYLEFT;
  send(LCD_ENTRYMODESET | Lcd_Conf.dispMode, LOW);
}

void LCD_rightToLeft(void) {
  Lcd_Conf.dispMode &= ~LCD_ENTRYLEFT;
  send(LCD_ENTRYMODESET | Lcd_Conf.dispMode, LOW);
}

void LCD_autoScroll(void) {
  Lcd_Conf.dispMode |= LCD_ENTRYSHIFTINCREMENT;
  send(LCD_ENTRYMODESET | Lcd_Conf.dispMode, LOW);
}

void LCD_noAutoScroll(void) {
  Lcd_Conf.dispMode &= ~LCD_ENTRYSHIFTINCREMENT;
  send(LCD_ENTRYMODESET | Lcd_Conf.dispMode, LOW);
}

void LCD_createChar(uint8_t location, uint8_t udg[]) {
    location &= 0x7;
    send(LCD_SETCGRAMADDR | (location << 3), LOW);
    int i;
    for(i = 0; i < 8; i++) {
        send(udg[i], HIGH);
    }
}

void LCD_put(const char c) {
    send(c, HIGH);
}

void LCD_printf(const char *fmt, ...) {
    char buffer[32];
    va_list args;
    va_start(args, fmt);
    uint16_t size = vsprintf(buffer, fmt, args);

    uint16_t i;
    uint8_t c;
    for(i = 0; i < size; i++) {
        c = buffer[i];

        if(c == 10)
            break;
        else
            send(c, HIGH);
    }
}

/* LOW LEVEL FUNCTIONS */

static void send(uint8_t value, uint8_t mode) {
	GPIO_WriteBit(Lcd_Conf.gpio, Lcd_Conf.rs, mode);

    if(Lcd_Conf.rw != 255)
        GPIO_ResetBits(Lcd_Conf.gpio, Lcd_Conf.rw);

    if(Lcd_Conf.dispFunc & LCD_8BITMODE) {
        writeBits(value, 8);
    } else {
        writeBits(value >> 4, 4);
        writeBits(value, 4);
    }
}

static void pulseEnable(void) {
	GPIO_ToggleBits(Lcd_Conf.gpio, Lcd_Conf.enable);
    delayMicroseconds(1);
	GPIO_ToggleBits(Lcd_Conf.gpio, Lcd_Conf.enable);
    delayMicroseconds(100);
}

static void writeBits(uint8_t value, uint8_t nbits) {
    set_output(1);

    int i;
    for(i = 0; i < nbits; i++) {
		GPIO_WriteBit(Lcd_Conf.gpio, Lcd_Conf.data[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

static void set_output(uint8_t output) {
    GPIO_InitTypeDef GPIO_InitStruct;
    uint32_t pins;
    uint8_t dir;

    pins = Lcd_Conf.data[0] | Lcd_Conf.data[1]
            | Lcd_Conf.data[2] | Lcd_Conf.data[3];

    if(Lcd_Conf.dispFunc & LCD_8BITMODE) {
        pins |= Lcd_Conf.data[4] | Lcd_Conf.data[5]
            | Lcd_Conf.data[6] | Lcd_Conf.data[7];
    }

    dir = GPIO_Mode_IN;

    if(output) {
        pins |= Lcd_Conf.rs | Lcd_Conf.enable;
        pins |= Lcd_Conf.rw == 255 ? 0 : Lcd_Conf.rw;
        dir = GPIO_Mode_OUT;
    }

    GPIO_InitStruct.GPIO_Pin = pins;
    GPIO_InitStruct.GPIO_Mode = dir;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;

    GPIO_Init(Lcd_Conf.gpio, &GPIO_InitStruct);
}
