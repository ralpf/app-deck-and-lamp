#pragma once
#include "Types.h"


struct CmpNoise
{
    float timeScale = 1;
    ui8   ampl      = 80;
    ui16  offset    = 0;
};


struct ModeMood
{
    ui32 color32 = 0x00FF00FF;
    bool is_fliker;
    CmpNoise noiseBrt;
    CmpNoise noiseHue;
};


struct ModeRandom
{
    ui16 delay = 300;    // in ms before next led lit
    ui8  fade  = 1;      // each cycle fade to black
};


struct StateData
{
    // global
    ui8 curr_mode  = 0;
    ui8 brightness = 200;

    // modes
    ModeMood   mode_Mood;
    ModeRandom mode_Rand;
};

extern StateData app;