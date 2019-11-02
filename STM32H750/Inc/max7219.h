// Library for MAX7219 using 8x8 dot matrix LEDs
// Carlos Santos - 2019

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stm32h7xx_hal.h>
#include <ctype.h>

// Define port and pin for CLK - change accordingly for your circuit
#define MAX7219_CLK_Port     GPIOB
#define MAX7219_CLK_Pin      GPIO_PIN_13

// Define port and pin for CS - change accordingly for your circuit
#define MAX7219_CS_Port     GPIOB
#define MAX7219_CS_Pin      GPIO_PIN_14

// Define port and pin for DIN - change accordingly for your circuit
#define MAX7219_DIN_Port     GPIOB
#define MAX7219_DIN_Pin      GPIO_PIN_15

//MAX7219 settings
//brightness intensity
static const char intensity = 0x02;
//decoding (0x00 = no decoding)
static const char decoding = 0x00;
//scan limit (0x07 = 8 LEDS)
static const char scanLimit = 0x07;
//test display(0x00 = no test)
static const char testDisplay = 0x00;

//number of 8x8 led matrix modules
static const int totalLedMaxtrixElements = 8;


// Exported functions prototypes for MAX7219
void max7219_init(void);
void max7219_clear(void);
void max7219_8x8led_display_message(char* message);
void max7219_8x8led_shift_message(int shiftbits);
void max7219_8x8led_showBuffer();
void max7219_8x8led_freeBuffer();
void max7219_8x8led_display_message_continuous(char* message, int shiftDelay);
