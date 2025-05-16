#pragma once


#include <FastLED.h>
#include <Types.h>


void          palette_init();   // mandatory call first!
ui8           palette_count();
i16           palette_idx(const char* name);
const CRGBPalette16& palette_from_idx(ui8 idx);
const CRGBPalette16& palette_from_name(const char* name);



