#pragma once

#include <Types.h>
#include <FastLED.h>

ui32 rgb_2_ui32(const CRGB& rgb);
ui32 hsv_2_ui32(const CHSV& hsv);

CRGB html_2_rgb(const char* buff);
ui32 html_2_rgb_2_ui32(const char* buff);

CRGB ui32_2_rgb(ui32 rgb32);
CHSV ui32_2_hsv(ui32 hsv32);

CHSV rgb_2_hsv_slow(const CRGB &rgb);
CRGB hsv_2_rgb_slow(const CHSV &hsv);

CHSV rgb_2_hsv_fast(const CRGB &rgb);
CRGB hsv_2_rgb_fast(const CHSV &hsv);

bool palette_compare(const CRGBPalette16& lhs, const CRGBPalette16& rhs, ui8 tolerance = 3);
void update_gammaLUT_table256(ui8* array256, float gamma);
