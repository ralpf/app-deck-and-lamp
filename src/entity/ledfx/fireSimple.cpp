#include "fireSimple.h"
#include <colorUtils.h>
#include <PrintHelper.h>



void FireSimple::Update()
{
    // timer stuff
    ui32 currTime = millis();
    float dt = (currTime - prevTime) * s.speed;
    prevTime = currTime;

    // phase timing
    if (dt < phaseDuration - elapsedTime)
    {
        elapsedTime += dt;
    }
    else    // phase ended
    {
        elapsedTime = 0;
        // pick a random phase duration in ms
        phaseDuration = random16(s.maxPhaseLengthMs - s.minPhaseLengthMs) + s.minPhaseLengthMs;
        // pick a target temperature
        targetTemp = random16(s.maxTemperature - s.minTemperature) + s.minTemperature;
    }
    // update temperature
    float K = s.filterKFactor;
    if (targetTemp < currTemp) K *= 0.35f;
    float k = 1.0f - powf(1.0f - K, dt / 20.0f);
    currTemp += (targetTemp - currTemp) * k;
    // convert to color
    float heat = currTemp * (255.0f / 6000.0f);
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
    // set leds color
    leds.SetColor(currColor);
}


void FireSimple::AppendToJson(JsonWriter& jw)
{
}