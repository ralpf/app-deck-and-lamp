#include "klass.h"


PaletteLeds::PaletteLeds(CRGB *leds, ui16 count)
{
    this->leds  = leds;
    this->count = count;
}


void PaletteLeds::Apply()
{
    float f = 255.0 / count;
    for (int i = 0; i < count; ++i)
        leds[i] = ColorFromPalette(current, i * f, 200);

    FastLED.show();
}


void PaletteLeds::SetPalette(const CRGBPalette16 rhs)
{
    original  = rhs;
    current   = rhs;
    speed     = 0xFF;
    bright    = 0xFF;
    Apply();
}


void PaletteLeds::SetColor(const CRGB rhs)
{
    for (ui8 i = 0; i < count; ++i)
        leds[i] = rhs;
}