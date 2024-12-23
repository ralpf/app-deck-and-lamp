#include <FastLED.h>
#include "PrintHelper.h"

#define VERSION "0.1.1"
#define LED_PIN 14 // GPIO14 as your data pin
#define NUM_LEDS 6

CRGB leds[NUM_LEDS];

void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    SPrint("\nStarting Levitating Disco Ball Leds\nVersion %s\n", VERSION);
}

void loop()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Green; // Set all LEDs to red
        FastLED.show();
        delay(500);            // Wait half a second
        leds[i] = CRGB::Black; // Turn off
    }
}
