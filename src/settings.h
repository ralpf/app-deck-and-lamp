#pragma once

#include <Types.h>
#include <jsonWriter.h>



// all stuff is nested inside
// only 1 100% lifetime instance exists

struct Settings
{
    //...................................................SHARED-TYPE

    struct Wave
    {
        float spd  = 0;   // speed
        ui8   ampl = 0;   // amplitude
    };

    struct Flicker
    {
        bool isOn = false;
        Wave hue;
        Wave val;
    };

    struct Sliders
    {
        ui8 hue = 200;
        ui8 sat = 255;

        ui32 to_hsv32();
    };

    struct Pickers
    {
        static constexpr ui8 max = 8;
        ui32 cols32[max] = {0xFF00FF, 0};

        ui32 to_hsv32();
    };

    struct Comet    // don't emit json for this struct
    {
        ui8  speed = 32;
        ui8  headLength = 3;
        ui8  tailLength = 24;
        ui32 rgb32ColorHead       = 0xF0FFFF;   // azure
        ui32 rgb32ColorTailStart  = 0xF5F5DC;   // beige
        ui32 rgb32ColorTailEnd    = 0xFF0000;   // red
        bool spaw_comet_now;                    // flag
    };

    //.....................................................MODE-TYPE

    struct Global
    {
        ui8 luma = 200;
        float gamma = 1;

        Comet comet;                    // NO-EMIT-JSON
        bool update_gamma = false;      // NO-EMIT-JSON
    };

    //.....................................................MODE-TYPE

    struct Lamp
    {
        // inner enum
        enum class EMode : ui8 { Mood, Sliders, Picker };

        // inner type
        struct Mood
        {
            ui32 rgb32 = 0xFFD700;  // random gold color

            ui32 to_hsv32();
        };

        // top level storage
        ui8 luma = 200;     // lamp brightess
        Flicker flik;       // shared between modes. SET by main.cpp to non-zero default

        EMode mode = EMode::Mood;     // current mode

        // per-mode storage with defaults
        Mood mood;
        Sliders sliders;
        Pickers pickers;

        ui32 hsv32_target = 0;   // NO-EMIT-JSON             // target HSV, w/o effects
        ui32 hsv32_actual = 0;   // NO-EMIT-JSON             // actual HSV, with all aplied effects
    };

    //.....................................................MODE-TYPE

    struct Deck
    {
        // inner enum
        enum class EMode : ui8 { MirrorLamp, Sliders, Picker, Palette, Fire };

        // inner type
        struct Palette
        {
            bool isRandz = false;
            ui8  speed = 4;         // index translation speed
            ui8  idx = 0;           // curr palette idx
        };

        // inner type
        struct Fire
        {
            ui16 temperature = 4800;
            ui16 speed = 100;
        };

        // top level storage
        ui8 luma = 128;     // lamp brightess
        Flicker flik;       // shared bt modes

        EMode mode = EMode::Palette;   // current mode

        // per-mode storage with defaults
        Sliders sliders;
        Pickers pickers;
        Palette palette;
        Fire    fire;

        ui32 hsv32_target = 0;   // NO-EMIT-JSON             // target HSV, w/o effects
        ui32 hsv32_actual = 0;   // NO-EMIT-JSON             // actual HSV, with all aplied effects
    };

    //........................................................STORAGE

    Global glob;
    Lamp   lamp;
    Deck   deck;

    bool flagPrintJson;          // NO-EMIT-JSON            // force print app state json

    //........................................................TO-JSON

    void emit_StateJson(JsonWriter& json);          // for sync with webpage
    void emit_PalleteJson(JsonWriter& json);        // available palletes as json array
};

//.............................................................SINGLE

extern Settings app;

