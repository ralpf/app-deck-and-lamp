#pragma once

#include "gameObject.h"


class Background : public GObject
{
    public:
    void Draw(LedsStrip* pLeds) override
    {
        // clear the leds
        pLeds->SetColor(CRGB(0,0,0));
    }
};