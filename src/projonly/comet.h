#pragma once

#include "../ledsStrip.h"


class Comet
{
    private:
    LedsStrip& leds;

    public:
    float position;                     // the led at which the comet head is. Can be fractional (for smoot animation)
    ui8   backgroundScaleDown8 = 128;   // the dimm factor for the background (128 = 0.5)
    ui8   speed = 16;                   // the speed of the comet
    ui8   headLenth = 3;                // the size of the comet head
    ui8   tailLength = 16;              // tail length, in number of affected leds
    CHSV  headColor;
    CHSV  tailStartColor;
    CHSV  tailEndColor;
    bool  directionLeftToRight;    // direction of propagation

    public:
    Comet(LedsStrip& target) : leds(target)
    {
        // default colors
        this->headColor      = rgb_2_hsv_slow(CRGB::Azure);
        this->tailStartColor = rgb_2_hsv_slow(CRGB::Beige);
        this->tailEndColor   = rgb_2_hsv_slow(CRGB::Red);
        this->directionLeftToRight = true;
    }

    public:
    void Update();                      // called from main loop to update animation
    void StartOne();                    // call once for one run
};
