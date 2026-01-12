#pragma once

#include <Types.h>
#include <FastLED.h>

ui32 html_2_ui32(const char* buff);
ui32 rgb_2_ui32(CRGB rgb);

void rgb2hsv(const CRGB &rgb, CHSV &hsv);
void hsv2rgb(const CHSV &hsv, CRGB &rgb);

bool palettesApproxEqual(const CRGBPalette16& lhs, const CRGBPalette16& rhs, ui8 tolerance = 3);
void updateGammaLutTable256(ui8* array256, float gamma);