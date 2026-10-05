#pragma once

#include "gameObject.h"


struct BackgroundLayer
{
    float intensity;            // 0..1, apply strength
    float scale;                // noise cell width / average star spacing, in LEDs
    float contrast;             // 0 = flat, 1 = original, above 1 = stronger
    CRGB color1;
    CRGB color2;
};


class Background : public GObject
{
    public:         // fields
    BackgroundLayer diffuse = { 0.05f, 48.0f, 0.9f, CRGB(90, 8, 160), CRGB(4, 40, 140) };
    BackgroundLayer nebula  = { 0.30f, 14.0f, 2.4f, CRGB(180, 12, 0), CRGB(0, 100, 35) };
    BackgroundLayer stars   = { 0.50f, 54.0f, 0.6f, CRGB(100, 100, 120), CRGB(255, 255, 255) };
    float speed = 16.0f;         // LEDs per second, right to left

    private:        // fields
    double scroll = 0;         // retain smooth movement during long runs

    public:         // methods
    void Awake() override;
    void Update() override;
    void Draw(LedsStrip* pLeds) override;

    private:        // methods
    CRGB StarColor(i32 worldPixel) const;
};