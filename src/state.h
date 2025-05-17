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
    bool update;
    ui8  bright = 0xF0;
    ui8  blend  = 0x18;

    enum class Mode  { MirrorLamp, Palette, HSV } mode;
    struct { ui8 h = 0xF0; ui8 s = 0xFF; } hsv;
    struct { ui32 color32; } mirrorLamp;
    struct { ui8 idx; bool irand; const char* namesJson; } palette;
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