#pragma once
#include "Types.h"
#include "jsonWriter.h"



// all stuff is nested inside
// only 1 100% lifetime instance exists

struct Settings
{
    //...................................................SHARED-TYPE

    struct Flicker
    {
        ui8 ampl;   // amplitude
        ui8 spd;    // speed
    };

    struct Sliders
    {
        ui8 hue;
        ui8 sat;
    };

    struct Pickers
    {
        static constexpr ui8 max = 8;
        ui32 cols32[max];
    };

    //.....................................................MODE-TYPE

    struct Global
    {
        ui8 luma;
    };

    //.....................................................MODE-TYPE

    struct Lamp
    {
        // inner enum
        enum class EMode : ui8 { Mood, Sliders, Picker, Pallete };

        // inner type
        struct Mood
        {
            ui32 col32;
        };

        // top level storage
        ui8 luma = 200;     // lamp brightess

        // flicker setup, shared between modes
        bool    flikOn   = false;
        Flicker flikHue  = { .ampl = 20, .spd = 50 };
        Flicker flikLuma = { .ampl = 0,  .spd = 0  };

        EMode mode = EMode::Mood;     // current mode

        // per-mode storage with defaults
        Mood mood       = { .col32 = 0xFFD700 };        // random gold color
        Sliders sliders = { .hue = 200, .sat = 255 };   // max saturation
        Pickers pickers = { .cols32 = {0xFF00FF, 0} };  // one default color

        // not serialized to json
        ui32 col32 = 0;                                 // actual color
    };

    //.....................................................MODE-TYPE

    struct Deck
    {
        ui8 luma;
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

