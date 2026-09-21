#pragma once

#include "ledsStrip.h"


// a helper class to create fastled strips. Manages the hardware part
// inherits from LedsStrip that manages all change states in the strip
template <ui8 PIN, ui16 COUNT>
class LedsHardware : public LedsStrip
{
    static_assert(COUNT > 0, "LED count must be greater than zero");

    //....................................................................FIELDS
    private:

    CRGB leds[COUNT];

    //......................................................................CTOR
    public:

    LedsHardware() : LedsStrip(leds, COUNT)
    {
        // FastLED requires the data pin at compile time
        FastLED.addLeds<WS2812, PIN, GRB>(leds, COUNT);
    }
};
