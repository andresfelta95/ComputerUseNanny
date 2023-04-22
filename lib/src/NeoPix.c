//  NeoPixel library, ATmega328P Version
//  Andres Tangarife, NAIT
//  Revision History:
//  March 29 2023 - Initial Build
//  Description:    This library is used to control a NeoPixel LED strip 
//                  with an ATmega328P using C.
//  Notes:          The DIN pin is connected to pin 5 of port D.
//                  It has 8 LEDs in the strip.
#include "NeoPix.h"
#define F_CPU 16E6
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

//  Structure to hold the color data for each LED in the strip
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color_t;

// Data buffer for transmitting color data to the LED strip
// color_t neopixel_buffer[NEOPIXEL_NUM_LEDS];
uint8_t neopixel_buffer[NEOPIXEL_NUM_LEDS * 3];
uint64_t _Cntr = 0;

// Send a pulse to the LED strip to latch the current color data
void neopixel_send_pulse() 
{
  // Set the neopixel pin high
  NEOPIXEL_PORT |= (1 << NEOPIXEL_PIN);
  // Wait 50 microseconds
  _delay_us(1);
  // Set the neopixel pin low
  NEOPIXEL_PORT &= ~(1 << NEOPIXEL_PIN);  
}

// Initialize the neopixel strip
void neopixel_init() {
  // Set the neopixel pin as an output
  NEOPIXEL_DDR |= (1 << NEOPIXEL_PIN);
}

// Set the color of a specific LED in the strip
void neopixel_set_color(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
  // Calculate the start index of the color data for this LED
  uint16_t index = led * 3;

  // Store the color data for this LED in the data buffer
  neopixel_buffer[index + 0] = g;
  neopixel_buffer[index + 1] = r;
  neopixel_buffer[index + 2] = b;
}

// Set the color of all LEDs in the strip
void neopixel_set_color_all(uint8_t r, uint8_t g, uint8_t b) {
  // Store the color data for all LEDs in the data buffer
  for (uint8_t i = 0; i < NEOPIXEL_NUM_LEDS; i++) {
    neopixel_set_color(i, r, g, b);
  }
}

// Turn off a specific LED in the strip
void neopixel_turn_off(uint8_t led) {
  // Set the color data for this LED to 0
  neopixel_set_color(led, 0, 0, 0);
}

// Turn off all LEDs in the strip
void neopixel_turn_off_all() {
  // Set the color data for all LEDs to 0
  neopixel_set_color_all(0, 0, 0);
}

// Send the color data in the buffer to the LED strip
void neopixel_update() {
  static unsigned char cnter = 0;
  //  1 bit = 1.25us
  //  0 bit = 0.625us
  // Send the color data for each LED in the strip
  for (uint16_t i = 0; i < NEOPIXEL_NUM_LEDS * 3; i++) {
    uint8_t data = neopixel_buffer[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (data & (1 << 7)) {
        NEOPIXEL_PORT |= (1 << NEOPIXEL_PIN);
        ++_Cntr;
        NEOPIXEL_PORT &= ~(1 << NEOPIXEL_PIN);
      } else {
        NEOPIXEL_PORT |= (1 << NEOPIXEL_PIN);
        // ++cnter;
        NEOPIXEL_PORT &= ~(1 << NEOPIXEL_PIN);
      }
      data <<= 1;
    }
  }

  // Re-enable interrupts
  // sei();
}





