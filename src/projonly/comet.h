#pragma once

#include "../ledsStrip.h"


class Comet
{
    private:
    LedsStrip& leds;

    public:
    Comet(LedsStrip& target) : leds(target) { }

    public:
    void Update();
};
