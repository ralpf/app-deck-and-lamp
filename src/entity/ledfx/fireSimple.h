#pragma once

#include "../core/ientity.h"
#include "../../ledsStrip.h"
#include <FastLED.h>






class FireSimple : public IEntity
{
    // NESTED TYPE
    public:
    struct Settings : public ISetting
    {
        ui8  hueShift         = 0;
        ui16 minPhaseLengthMs = 150;
        ui16 maxPhaseLengthMs = 900;
        ui16 minTemperature   = 1000;
        ui16 maxTemperature   = 3000;
        float filterKFactor = 0.12f;
        float speed = 1.0f;
        // impl. for abstract method
        void Reset() override { *this = Settings{}; }
    };


    public:         // fields
    LedsStrip& leds;
    Settings s;     // the settings of effect. Expose it so backend callbacks can change it

    private:        // fields
    ui32  prevTime = 0;
    float elapsedTime = 0;
    float currTemp = 1000.0f;
    ui16  phaseDuration = 1000;     // non-zero start value removes some checks in update
    ui16  targetTemp = 2000;        // same reson for non-zero value

    public:         // CTOR
    FireSimple(LedsStrip& target) : leds(target) { }

    public:         // methods
    Settings* GetSettings() override { return &s; }     // uses covariance for returned type
    void AppendToJson(JsonWriter& jw) override;
    void Update() override;
};