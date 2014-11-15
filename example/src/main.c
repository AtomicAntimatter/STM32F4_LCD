#include "main.h"

extern volatile uint32_t msTicks;

int main(void) {
    Time_Init();    // initialize the time library (important!)

    // make the user defined graphic
    uint8_t udg[8] = {0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c};

    // initialize all pins
    LCD_4BitInit(GPIOE, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
        GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13);

    LCD_createChar(0, udg);             // create user defined graphic
    LCD_Begin(2, 16, LCD_5x8DOTS);      // 2 rows 16 cols 5x8 font

    LCD_printf("Hello World! %d", 9);   // printf hello world
    LCD_put(0);                         // print the UDG
    delay(5000);                        // delay in milliseconds
    LCD_clear();                        // clear the lcd
    delay(2000);
    LCD_setCursor(0,5);                 // set cursor to row 0 col 5
    LCD_blink();                        // blink the spot
    delay(2000);
    LCD_noBlink();                      // stop the blinking
    LCD_cursor();                       // show the cursor
    delay(2000);
    LCD_put('$');                       // output a $
    LCD_scrollDisplayLeft();            // scroll the display 1 spot to left
    delay(2000);
    LCD_rightToLeft();                  // enable writing backwards
    LCD_printf("abcd");                 // should output dcba

    while(1) {
        delay(1000);
        LCD_scrollDisplayRight();       // scroll from left to right
    }

	return(0);
}
