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

    // top level storage
    ui8 luma;       // lamp brightess

    bool    flikOn;
    Flicker flikHue;
    Flicker flikLuma;

    Mode mode;      // current mode
    ui32 color32;   // actual color

    // per-mode storage
    Mood mood;

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
