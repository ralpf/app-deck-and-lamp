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
    ui32 actualColor;               // the color applied to lamp. It can flicker, and will not match BlazarLamp.color32
    bool is_fliker;
    ui8  bright = 250;
    ui8  blend  = 24;
    CmpNoise noiseBrt;
    CmpNoise noiseHue;
};


struct TVConsole
{
    ui8  bright = 109;
    ui8  blend  = 24;
    ui8  anim   = 0;

    enum class Mode  { MirrorLamp, Palette, HSV } mode;
    struct { ui8 h = 0xF0; ui8 s = 0xFF; } hsv;
    struct { ui32 color32; } mirrorLamp;
    struct { ui8 idx; bool irand; const char* namesJson; } palette;
};


struct Metrics
{
    ui16 freeStack;               // bytes
    ui16 jsonSize;                // the size of sync data
};


struct StateData
{
    enum class DeviceMode { Lamp, Console, Globals /*keep last*/ };
    // global
    ui8   brightness = 200;       // overlays other led brightness, global mult
    ui8   animSkip = 8;           // this will scale down the addressed leds animations
    float gamma    = 2;           // a int 22 shoulb be send for 2.2

    // containers
    BlazarLamp lamp;
    TVConsole console;
};


extern StateData oldapp;
const char* app_fetchStateJson();