#pragma once

#include "ientity.h"


// this is an example class, can be used as template
class ExampleTemplate : public IEntity
{
    struct Settings : public ISetting
    {
        void Reset() override { *this = Settings{}; }
    };

    public:
    Settings s;

    public:
    void Update() override;
    virtual void AppendToJson(JsonWriter& jw) { /* do nothing */}
    Settings* GetSettings() override { return &s; }
};