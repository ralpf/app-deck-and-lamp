#pragma once

#include <FastLED.h>
#include <Types.h>


// the definitions go here because template classes can't be split to .cpp
template <ui8 PIN, ui16 COUNT>
class AddresLeds
{
    
    private:
    CRGB          leds[COUNT];
    CRGBPalette16 palette;
    TBlendType    blend = TBlendType::LINEARBLEND;

    float f = 255.0 / COUNT;    // used to map COUNT of leds on [0, 255] range
    ui16  index;                // palette index for fill
    ui8   speed;                // used for fill
    ui8   bright = 0xFF;        // used for fill


    public:                     // CTOR
    AddresLeds()
    {
        FastLED.addLeds<WS2812, PIN, GRB>(leds, COUNT);
    }


    private:                // METHODS
    void ApplyPalette(ui16 offsetIdx = 0)
    {
        for (ui16 i = 0; i < COUNT; ++i)
            leds[i] = ColorFromPalette(palette, i * f + offsetIdx, bright, blend);
    }


    public:
    void SetPalette(const CRGBPalette16& pal16)
    {
        palette = pal16;
        ApplyPalette();
    }

    void SetColor(const CRGB& rgb)
    {
        for (ui16 i = 0; i < COUNT; ++i) leds[i] = rgb;
    }

    void SetColor(const CRGB& rgb, ui16 fromIdx, ui16 toIdx)
    {
        fromIdx = constrain(fromIdx, 0, COUNT);
        toIdx   = constrain(toIdx, 0, COUNT);
        for (ui16 i = fromIdx; i < toIdx; ++i) leds[i] = rgb;
    }

};