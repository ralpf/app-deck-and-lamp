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
        ui8  firePointCount   = 2;      // can be 1, 2, 3, 6. Sets the number of individual fires across blazar's led columns
        ui8  hueShift         = 0;
        ui16 minPhaseLengthMs = 150;
        ui16 maxPhaseLengthMs = 900;
        ui16 minTemperature   = 1400;
        ui16 maxTemperature   = 4000;
        float filterKFactor   = 0.05f;
        float speed           = 3.0f;
        // impl. for abstract method
        void Reset() override { *this = Settings{}; }
    };

    private:
    struct FirePoint
    {
        float elapsedTime = 0;
        float currTemp = 1000.0f;
        ui16  phaseDuration = 0;   // pick a random phase on first update
        ui16  targetTemp = 2000;
    };


    public:         // fields
    LedsStrip& leds;
    Settings s;     // the settings of effect. Expose it so backend callbacks can change it

    private:        // fields
    ui32  prevTime = 0;
    FirePoint firePoints[6];

    public:         // CTOR
    FireSimple(LedsStrip& target) : leds(target) { }

    public:         // methods
    Settings* GetSettings() override { return &s; }     // uses covariance for returned type
    void AppendToJson(JsonWriter& jw) override;
    void Update() override;

    private:        // methods
    void _UpdateFirePoint(FirePoint& p, float dt);
    CRGB _GetFirePointColor(const FirePoint& p) const;
};