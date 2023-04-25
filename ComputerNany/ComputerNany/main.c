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
//  Color constants
typedef enum
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    PURPLE,
    CYAN,
    WHITE,
    PINK,
    ORANGE,
    LIGHT_BLUE,
    LIGHT_GREEN,
    MAGENTA,
    LIGHT_YELLOW,
    LIGHT_PURPLE
} color_enum_t;
// const color_t RED = {125, 0, 0};
// const color_t GREEN = {0, 125, 0};
// const color_t BLUE = {0, 0, 125};
// const color_t YELLOW = {125, 125, 0};
// const color_t PURPLE = {125, 0, 125};
// const color_t CYAN = {0, 125, 125};
// const color_t WHITE = {125, 125, 125};
// const color_t PINK = {125, 0, 50};
// const color_t ORANGE = {125, 50, 0};
// const color_t LIGHT_BLUE = {0, 0, 50};
// const color_t LIGHT_GREEN = {0, 50, 0};
// const color_t MAGENTA = {50, 0, 50};
// const color_t LIGHT_YELLOW = {50, 50, 0};
// const color_t LIGHT_PURPLE = {50, 0, 50};


/* Global variables */
//  Time of flight sensor variables
VL53L1X_ERROR tof_status;
uint8_t state = 0;
uint16_t tof_distance;

//  Time variables
uint32_t time_to_sleep = 0;
uint32_t time_to_blink = 0;
uint32_t time_to_change_color = 0;

//  Switch variables
uint8_t switch_status = 0;

//  Color variables
color_enum_t current_color = RED;

//  Other variables
uint8_t user_is_here = 0;
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t leave_seconds = 0;
volatile uint8_t here_ticks = 0;
volatile uint8_t leave_ticks = 0;
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
void start_Animation(void);
void tof_check_distance(void);
void go_to_sleep(void);
void change_color(void);
void set_color(color_enum_t color);




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
    
    /* Start animation */
    start_Animation();

    /* Initialize timer */
    Timer_Init(Timer_Prescale_64, 25000);   //  16E6 / 64 / 2500 = 10 Hz
    
    /* Enable sleep mode for power saving */
    sleep_enable();


    /* Initialize switch */
    switch_init();

    /* Initialize variables */
    time_to_sleep = 0;
    time_to_blink = 0;
    time_to_change_color = 0;
    switch_status = 0;

    /* Enable global interrupts */
    sleep_enable();
    sei();

    /* Main loop */
    while (1) 
    {
        //  Call the interrupt
        sleep_mode();
        /* Check distance with Time of Flight sensor */
        tof_check_distance();

        SSD1306_Clear();

        /* Check switch status */
        if((PIND & (1 << SWITCH)))
        {
            switch_status = 1;
        }
        else
        {
            switch_status = 0;
        }

        /* Change color */
        if(switch_status == 1)
        {
            change_color();
            switch_status = 0;
        }

        /* Check if there is someone in front of the computer */
        if(tof_distance < 500)
        {
            user_is_here = 1; 
            /* Turn on NeoPixel */
            set_color(current_color);
            neopixel_update();
            /* Display time on OLED display */
            sprintf(str, "Time: %u:%u", minutes, seconds);
            SSD1306_StringXY(0, 0, str);
            SSD1306_Render();
        }
        else
        {
            user_is_here = 0;
            /* Turn off NeoPixel */
            neopixel_turn_off_all();
            neopixel_update();
        }

        /* Display distance on OLED display */
        sprintf(str, "Distance: %u", tof_distance);
        SSD1306_StringXY(0, 3, str);
        SSD1306_Render();

        /* Sleep for 500ms*/
        _delay_ms(500);

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

//  Start animation
void start_Animation(void)
{
    //  Animation for the OLED display
    // SSD1306_StringXY(0, 0, "Hello!");
    // Move a circle across the screen
    for (uint8_t i = 0; i < 128; i += 4)
    {
        SSD1306_Clear();
        SSD1306_Circle(i, 16, 10);
        SSD1306_Render();
    }
    SSD1306_Clear();
    SSD1306_Render();

    //  Animation for the NeoPixel
    set_color(CYAN);
    neopixel_update();
    _delay_ms(1000);
    set_color(MAGENTA);
    neopixel_update();
    _delay_ms(1000);
    set_color(YELLOW);
    neopixel_update();
    _delay_ms(1000);
    set_color(WHITE);
    neopixel_update();
    _delay_ms(1000);
    set_color(RED);
    neopixel_update();
    _delay_ms(1000);
    set_color(GREEN);
    neopixel_update();
    _delay_ms(1000);
    neopixel_turn_off_all();
    neopixel_update();
}

//  Check distance with Time of Flight sensor
void tof_check_distance(void)
{
    /*Variables*/
    uint8_t _DataReady = 0;
    uint8_t _RangeStatus = 0;
    //  Start continuous ranging measurements
    tof_status = VL53L1X_StartRanging(0);
    //  Wait for new measurement
    while (_DataReady == 0)
    {
        tof_status = VL53L1X_CheckForDataReady(0, &_DataReady);
    }
    tof_status = VL53L1X_GetRangeStatus(0, &_RangeStatus);
    tof_status = VL53L1X_GetDistance(0, &tof_distance);
    tof_status = VL53L1X_ClearInterrupt(0);

    //  Stop continuous ranging measurements
    tof_status = VL53L1X_StopRanging(0);
    
}

//  Go to sleep
void go_to_sleep(void)
{
    //  Turn off display
    SSD1306_DisplayOff();
    //  Turn off NeoPixel
    neopixel_turn_off_all();
    neopixel_update();
    //  Go to sleep
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
    //  Turn on display
    SSD1306_DisplayOn();
}

//  Change color
void change_color(void)
{
    //  Check switch status
    
    //  Change color
    if (current_color == LIGHT_PURPLE)
    {
        current_color = RED;
    }
    else
    {
        current_color++;
    }
    set_color(current_color);
    neopixel_update();
    
}

//  Set color to NeoPixel
void set_color(color_enum_t color)
{
    switch(color)
    {
        case RED:
            neopixel_set_color_all(125, 0, 0);
            break;
        case GREEN:
            neopixel_set_color_all(0, 125, 0);
            break;
        case BLUE:
            neopixel_set_color_all(0, 0, 125);
            break;
        case YELLOW:
            neopixel_set_color_all(125, 125, 0);
            break;
        case CYAN:
            neopixel_set_color_all(0, 125, 125);
            break;
        case PURPLE:
            neopixel_set_color_all(125, 0, 125);
            break;
        case WHITE:
            neopixel_set_color_all(125, 125, 125);
            break;
        case PINK:
            neopixel_set_color_all(125, 0, 50);
            break;
        case ORANGE:
            neopixel_set_color_all(125, 50, 0);
            break;
        case LIGHT_BLUE:
            neopixel_set_color_all(0, 50, 125);
            break;
        case LIGHT_GREEN:
            neopixel_set_color_all(0, 125, 50);
            break;
        case MAGENTA:
            neopixel_set_color_all(125, 0, 100);
            break;
        case LIGHT_YELLOW:
            neopixel_set_color_all(125, 100, 0);
            break;
        case LIGHT_PURPLE:
            neopixel_set_color_all(125, 0, 100);
            break;
        default:
            neopixel_set_color_all(0, 0, 0);
            break;
    }
}

/* Interrupt service routines */

//  Timer interrupt
ISR(TIMER1_COMPA_vect)
{
    // rearm the output compare operation   
    OCR1A += 25000; // 100ms intervals 

    if (user_is_here)
    {
        //  Reset leave_seconds
        leave_seconds = 0;
        //  Increment the here_Ticks
        if (++here_ticks >= 100)
        {
            here_ticks = 0;
        }
        if (here_ticks % 10 == 0)
        {
            //  Increment seconds
            if (++seconds >= 60)
            {
                seconds = 0;
                //  Increment minutes
                if (++minutes >= 60)
                {
                    minutes = 0;
                }
            }
        }
    }
    // If user left but it may come back
    else if (!user_is_here && here_ticks > 0 && leave_seconds < 10)
    {
        //  Increment leave_ticks
        if (++leave_seconds % 10 == 0)
        {
            //  Increment leave_seconds
            ++leave_seconds;
        }
    }
    //  If user left and it won't come back
    else if (!user_is_here && leave_seconds >= 10)
    {
        //  Reset here_ticks
        here_ticks = 0;
        //  Reset leave_seconds
        leave_seconds = 0;
        //  Reset seconds
        seconds = 0;
        //  Reset minutes
        minutes = 0;
    }
}
