/*
 * ComputerNany.c
 *
 * Created: 2023-04-22 1:42:45 PM
 * Author : Andres Tangarife
 * Components:
 *  - ATmega328P
 *  - 128x32 OLED Display
 *  - LED strip WS2812B with 8 LEDs
 *  - 1 switch
 *  - VL53L1X Time of Flight sensor
 * Description:
 *  This project is a prototype of a computer nanny. It will be used to monitor
 *  the time spent by the user in front of the computer. It will also be used
 *  To emit a light while the user is working to help him see better and the
 *  OLED dispplay will show the time spent in front of the computer.
 *  After a certain amount of time, the light will start to blink to notify the
 *  user that he should take a break with a message on the OLED display.
 *  The user can press the switch to change the color of the light.
 */ 

#define F_CPU 16E6

/* Include libraries */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NeoPix.h"
#include "timer.h"
#include "I2C.h"
#include "SSD1306.h"
#include "VL53L1X_api.h"

//  Switch connected to PD2
#define SWITCH PD2

/* Define constants */
//  Color Struc for NeoPixel
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color_t;
//  Color constants
color_t RED = {125, 0, 0};
color_t GREEN = {0, 125, 0};
color_t BLUE = {0, 0, 125};
color_t YELLOW = {125, 125, 0};
color_t PURPLE = {125, 0, 125};
color_t CYAN = {0, 125, 125};
color_t WHITE = {125, 125, 125};
color_t PINK = {125, 0, 50};
color_t ORANGE = {125, 50, 0};
color_t LIGHT_BLUE = {0, 50, 125};
color_t LIGHT_GREEN = {0, 125, 50};

/* Global variables */
//  Time of flight sensor variables
VL53L1X_Error_t tof_status;
uint8_t state = 0;
uint16_t tof_distance;

//  Time variables
uint32_t time_to_sleep = 0;
uint32_t time_to_blink = 0;
uint32_t time_to_change_color = 0;

//  Switch variables
uint8_t switch_status = 0;

//  Other variables
uint8_t i = 0;
uint8_t j = 0;
uint8_t k = 0;
uint8_t l = 0;
uint8_t m = 0;
uint8_t n = 0;
uint8_t o = 0;
uint8_t p = 0;
uint8_t q = 0;
uint8_t r = 0;
uint8_t s = 0;
uint8_t t = 0;

//  String variables
char str[20];
char str2[20];
char str3[20];
char str4[20];


/* Function prototypes */
void tof_init(void);
void switch_init(void);




/* Main function */
int main(void)
{
    /* Initialize I2C */
    I2C_Init(F_CPU, I2CBus100);

    /* Initialize NeoPixel */
    neopixel_init();
    neopixel_turn_off_all();
    neopixel_update();
    
    /* Initialize OLED display */
    SSD1306_DispInit();
    SSD1306_DisplayOn();
    //  Clear display
    SSD1306_Clear();
    SSD1306_Render();

    /* Initialize Time of Flight sensor */
    tof_init();

    /* Initialize timer */
    Timer_Init(Timer_Prescale_64, 250); //  1 ms (250 * 64 = 16 000 000 / 16 000 = 1000)
    sei();

    /* Initialize switch */
    switch_init();

    /* Initialize variables */
    time_to_sleep = 0;
    time_to_blink = 0;
    time_to_change_color = 0;
    switch_status = 0;

    /* Main loop */
    while (1) 
    {
    }
}

/* Function definitions */
//  Initialize Time of Flight sensor
void tof_init(void)
{
    //  Check boot status
    while(state == 0)
    {
        tof_status = VL53L1X_BootState(0, &state);
        _delay_ms(2);
    }
    //  Initialize the VL53L1X sensor
    tof_status = VL53L1X_SensorInit(0);
    tof_status = VL53L1X_SetDistanceMode(0, 1);
    tof_status = VL53L1X_SetTimingBudgetInMs(0, 500);
    tof_status = VL53L1X_SetInterMeasurementInMs(0, 500);
}

//  Initialize switch
void switch_init(void)
{
    //  Set switch pin as input
    DDRD &= ~(1 << SWITCH);
    //  Enable pull-up resistor
    PORTD |= (1 << SWITCH);
}
