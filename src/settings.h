#pragma once
#include "Types.h"


class JsonWriter;

//.....................................................INTERFACE

class IJsonWritable
{
    public:
    virtual ~IJsonWritable() = default;
    virtual void AddJsonData(JsonWriter& json) = 0;
};

//..........................................................TYPE

struct GlobSettings : IJsonWritable
{
    ui8 luma;

    // interface
    void AddJsonData(JsonWriter& json) override;
};

//..........................................................TYPE




struct LampSettings : IJsonWritable
{
    // inner enum
    enum class Mode : ui8 { Mood, Sliders, Picker, Pallete };

    // inner type
    struct Flicker
    {
        ui8 ampl;   // amplitude
        ui8 spd;    // speed
    };

    // inner type
    struct Mood
    {
        ui32 color32;  
    };

    // inner type
    struct Sliders
    {
        ui8 hue;
        ui8 sat;
    };
    
    // inner type
    struct Palette
    {
        static constexpr ui8 max = 8;
        ui32 colors[max];
    };

    // top level storage
    ui8 luma = 200;     // lamp brightess

    // flicker setup, shared between modes
    bool    flikOn   = false;
    Flicker flikHue  = { .ampl = 20, .spd = 50 };
    Flicker flikLuma = { .ampl = 0,  .spd = 0  };

    Mode mode = Mode::Mood;     // current mode

    // per-mode storage with defaults
    Mood mood = { .color32 = 0xFFD700 };            // random gold color
    Sliders sliders = { .hue = 200, .sat = 255 };   // max saturation
    Palette pallete = { .colors = {0xFF00FF, 0} };  // one default color

    // not serialized to json
    ui32 color32 = 0;           // actual color

    // interface impl
    void AddJsonData(JsonWriter& json) override;
};

//..........................................................TYPE

struct DeckSettings : IJsonWritable
{
    ui8 luma;

    // interface
    void AddJsonData(JsonWriter& json) override;
};

//..........................................................TYPE

struct Settings : IJsonWritable
{
    GlobSettings glob;
    LampSettings lamp;
    DeckSettings deck;
    
    void AddJsonData(JsonWriter& json) override;
};

//........................................................SINGLE

extern Settings app;
