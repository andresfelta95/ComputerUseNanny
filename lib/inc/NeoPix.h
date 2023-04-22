//  NeoPixel library, ATmega328P Version
//  Andres Tangarife, NAIT
//  Revision History:
//  March 29 2023 - Initial Build
//  Description:    This is the header for the library
//                  used to control a NeoPixel LED strip
//                  with an ATmega328P using C.
//  Notes:          The DIN pin is connected to pin 5 of port D.
//                  It has 8 LEDs in the strip.

// #ifndef NEOPIXEL_H
// #define NEOPIXEL_H // Include AVR library for I/O port access

// #define NUM_LEDS 8    // Number of LEDs in the strip

// // Define colors using 8-bit RGB values
// typedef struct {
//   uint8_t r = 0b00000000;
//   uint8_t g = 0b00000000;
//   uint8_t b = 0b00000000;
// } color_t;

// // Initialize the NeoPixel strip
// void neopixel_init(void);

// // Set the color of a specific LED in the strip
// void neopixel_set_pixel(uint8_t index, color_t color);

// // Set the color of all LEDs in the strip
// void neopixel_set_all(color_t color);

// //  Clear the color of a specific LED in the strip
// void neopixel_clear_pixel(uint8_t index);

// // Clear the color of all LEDs in the strip
// void neopixel_clear(void);

// // Send the color data to the strip
// void neopixel_update(void);

// #endif // NEOPIXEL_H

#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdint.h>

#define NEOPIXEL_PORT PORTD
#define NEOPIXEL_DDR DDRD
#define NEOPIXEL_PIN PD5

#define NEOPIXEL_NUM_LEDS 12

void neopixel_init();
void neopixel_set_color(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
void neopixel_set_color_all(uint8_t r, uint8_t g, uint8_t b);
void neopixel_turn_off(uint8_t led);
void neopixel_turn_off_all();
void neopixel_send_pulse();
void neopixel_update();

#endif /* NEOPIXEL_H */






