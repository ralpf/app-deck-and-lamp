#pragma once
#include "Types.h"
#include "jsonWriter.h"



// all stuff is nested inside
// only 1 100% lifetime instance exists

struct Settings
{
    //...................................................SHARED-TYPE

    struct Wave
    {
        ui8 spd  = 0;   // speed
        ui8 ampl = 0;   // amplitude
    };

    struct Flicker
    {
        bool isOn = false;
        Wave hue;
        Wave sat;
    };

    struct Sliders
    {
        ui8 hue = 200;
        ui8 sat = 255;
    };

    struct Pickers
    {
        static constexpr ui8 max = 8;
        ui32 cols32[max] = {0xFF00FF, 0};
    };

    //.....................................................MODE-TYPE

    struct Global
    {
        ui8 luma = 200;
    };

    //.....................................................MODE-TYPE

    struct Lamp
    {
        // inner enum
        enum class EMode : ui8 { Mood, Sliders, Picker, Pallete };

        // inner type
        struct Mood
        {
            ui32 col32 = 0xFFD700;  // random gold color
        };

        // top level storage
        ui8 luma = 200;     // lamp brightess
        Flicker flik;       // shared between modes

        EMode mode = EMode::Mood;     // current mode

        // per-mode storage with defaults
        Mood mood;
        Sliders sliders;
        Pickers pickers;

        // not serialized to json
        ui32 col32 = 0;                                 // actual color
    };
    
    //.....................................................MODE-TYPE
    
    struct Deck
    {
        // inner enum
        enum class EMode : ui8 { MirrorLamp, Sliders, Picker, Pallete };

        // inner type
        struct Palette
        {
            ui8 idx;
        };

        // top level storage
        ui8 luma = 128;     // lamp brightess
        Flicker flik;       // shared bt modes

        EMode mode = EMode::Pallete;   // current mode

        // per-mode storage with defaults
        Sliders sliders;
        Pickers pickers;
        Palette palette;

    };

    //........................................................STORAGE
    
    Global glob;
    Lamp   lamp;
    Deck   deck;

    //........................................................TO-JSON

    void emit_json(JsonWriter& json);
};

//.............................................................SINGLE

extern Settings app;

