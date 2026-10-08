#pragma once

#include <Types.h>
#include <jsonWriter.h>

// see readme.md for more insight


// override to make a settings struct. Do not define ctor or assigment operator, we rely on build-in ones
struct ISetting
{
    ui16 version = 0;               // change in this struct shoudl bump the version
    virtual void Reset() = 0;    // implement to restore defaults
};


// override to make and entity class
class IEntity
{
    public:  // methods
    virtual ISetting* GetSettings() = 0;                // return the base pointer
    virtual void AppendToJson(JsonWriter& jw) = 0;      // this will serialize current settings into an existing json
    virtual void Update() = 0;                          // implement to add action for an entity
};