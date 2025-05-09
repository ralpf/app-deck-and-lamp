#pragma once

#include <Types.h>
#include <Arduino.h>
#include <FastLED.h>

ui32 html_2_UI32(String htmlColor);
ui32 rgb_2_UI32(CRGB rgb);

void rgb2hsv(const CRGB &rgb, CHSV &hsv);
void hsv2rgb(const CHSV &hsv, CRGB &rgb);