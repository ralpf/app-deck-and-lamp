#pragma once

#include "../ledsStrip.h"


class Comet
{
    private:
    LedsStrip& leds;
    bool isRunning = false;
    ui32 lastUpdateMs = 0;

    public:
    float position = 0.0f;              // the led at which the comet head is. Can be fractional (for smoot animation)
    ui8   backgroundScaleDown8 = 128;   // the dimm factor for the background (128 = 0.5)
    ui8   speed = 16;                   // the speed of the comet, in leds per second
    ui8   headLenth = 3;                // the size of the comet head
    ui8   tailLength = 16;              // tail length, in number of affected leds
    CRGB  headColor;
    CRGB  tailStartColor;
    CRGB  tailEndColor;
    bool  directionLeftToRight = true;  // direction of propagation

    public:
    Comet(LedsStrip& target) : leds(target)  { }

    public:
    void Update();                      // called from main loop to update animation
    void StartOne();                    // call once for one run
};
