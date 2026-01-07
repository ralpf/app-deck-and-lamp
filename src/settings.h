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
    ui8 luma;
    
    // interface
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

    // interface
    void AddJsonData(JsonWriter& json) override;
};
