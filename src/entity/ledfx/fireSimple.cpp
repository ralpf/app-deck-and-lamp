#include "fireSimple.h"
#include <colorUtils.h>
#include <PrintHelper.h>



void FireSimple::Update()
{
    // timer stuff
    ui32 currTime = millis();
    float dt = (currTime - prevTime) * s.speed;
    prevTime = currTime;

    const ui8 firePointCount = s.firePointCount;
    const ui16 ledsPerPoint = leds.Count() / firePointCount;

    for (ui8 i = 0; i < firePointCount; ++i)
    {
        FirePoint& p = firePoints[i];
        _UpdateFirePoint(p, dt);
        CRGB currColor = _GetFirePointColor(p);

        ui16 fromIdx = i * ledsPerPoint;
        leds.SetColor(currColor, fromIdx, fromIdx + ledsPerPoint);
    }
}


void FireSimple::AppendToJson(JsonWriter& jw)
{
}


void FireSimple::_UpdateFirePoint(FirePoint& p, float dt)
{
    // phase timing
    if (dt < p.phaseDuration - p.elapsedTime)
    {
        p.elapsedTime += dt;
    }
    else    // phase ended
    {
        p.elapsedTime = 0;
        // pick a random phase duration in ms
        p.phaseDuration = random16(s.maxPhaseLengthMs - s.minPhaseLengthMs) + s.minPhaseLengthMs;
        // pick a target temperature
        p.targetTemp = random16(s.maxTemperature - s.minTemperature) + s.minTemperature;
    }
    // update temperature
    float K = s.filterKFactor;
    if (p.targetTemp < p.currTemp) K *= 0.35f;
    float k = 1.0f - powf(1.0f - K, dt / 20.0f);
    p.currTemp += (p.targetTemp - p.currTemp) * k;
}


CRGB FireSimple::_GetFirePointColor(const FirePoint& p) const
{
    // convert to color
    float heat = p.currTemp * (255.0f / 6000.0f);
    CRGB currColor = HeatColor((ui8)constrain(heat, 0.0f, 255.0f));
    // warm the colored part while preserving white
    ui8 white = min(currColor.r, min(currColor.g, currColor.b));
    currColor.g = white + scale8(currColor.g - white, 180);
    // recolorize
    if (s.hueShift != 0)
    {
        CHSV hsv = rgb_2_hsv_slow(currColor);
        hsv.h = (ui8)(hsv.h + s.hueShift);
        currColor = hsv_2_rgb_slow(hsv);
    }
    // affect brightness
    ui8 brightness = (ui8)constrain(heat * 2.0f, 0.0f, 255.0f);
    currColor.nscale8_video(brightness);
    return currColor;
}