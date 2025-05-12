#pragma once

#include <FastLED.h>
#include <Types.h>


class PaletteLeds
{
    private:
    CRGB* leds;
    ui16  count;
    ui16  index;            // palette index for fill
    ui8   speed;            // used for fill
    ui8   bright;           // used for fill

    CRGBPalette16 original;
    CRGBPalette16 current;

    private:
    void Apply();


    public:
    PaletteLeds() { /* default object will be not in use */ }
    PaletteLeds(CRGB *leds, ui16 count);

    //void Set(CRGBPalette256 pal256);
    void SetPalette(const CRGBPalette16 rhs);
    void SetColor(const CRGB rhs);
};