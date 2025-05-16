#pragma once

#include "Types.h"


struct CmpNoise
{
    float timeScale = 1;
    ui8   ampl      = 80;
    ui16  offset    = 0;
};


struct BlazarLamp
{
    ui32 color32 = 0x00FF00FF;
    ui32 actualColor;               // the color applied to lamp
    bool is_fliker;
    CmpNoise noiseBrt;
    CmpNoise noiseHue;
};


struct ModeRandom
{
    ui16 skip = 300;                // in ms before next led lit
    ui8  vari  = 1;                 // each cycle fade to black
};


struct TVConsole
{
    enum Mode { HSV, MirrorLamp, Palette } mode;
    struct HSV
    {
        ui8 h = 0xF0;
        ui8 s = 0xFF;
        ui8 v = 0xF0;
    } hsv;

    bool update;

    ui8  bright  = 0x00;
    ui32 color32 = 0x00FF00FF;

    bool paletteRand  = 0x0;
    ui8  paletteIdx   = 0x0;
    ui8  paletteBlend = 24;
};


struct StateData
{
    // global
    bool globalChanged;
    ui8 curr_mode  = 0;
    ui8 brightness = 200;       // overlays other brightness, global mult
    float gamma    = 2.2;

    // containers
    BlazarLamp lamp;
    ModeRandom rand;
    TVConsole console;
};

extern StateData app;