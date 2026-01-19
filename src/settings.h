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

    //.....................................................MODE-TYPE

    struct Global
    {
        ui8 luma = 200;
        float gamma = 1;

        bool update_gamma = false;     // NO-EMIT-JSON
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
        Flicker flik;       // shared between modes

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
        enum class EMode : ui8 { MirrorLamp, Sliders, Picker, Palette };

        // inner type
        struct Palette
        {
            bool isStillBlending;  // NO-EMIT-JSON     // this is a runtime var
            bool isRandz = false;
            ui8  idx = 0;
        };

        // top level storage
        ui8 luma = 128;     // lamp brightess
        Flicker flik;       // shared bt modes

        EMode mode = EMode::Palette;   // current mode

        // per-mode storage with defaults
        Sliders sliders;
        Pickers pickers;
        Palette palette;

        ui32 hsv32_target = 0;   // NO-EMIT-JSON             // target HSV, w/o effects
        ui32 hsv32_actual = 0;   // NO-EMIT-JSON             // actual HSV, with all aplied effects
    };

    //........................................................STORAGE

    Global glob;
    Lamp   lamp;
    Deck   deck;

    bool flagPrintJson;          // NO-EMIT-JSON            // force print app state json

    //........................................................TO-JSON

    void emit_json(JsonWriter& json);
};

//.............................................................SINGLE

extern Settings app;

