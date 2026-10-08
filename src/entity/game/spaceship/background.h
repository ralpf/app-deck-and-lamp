#pragma once

#include "gameObject.h"
#include "settings.h"




class Background : public GObject
{
    public:
    using Layer = SpaceshipGame::Settings::BackgroundLayer;

    public:         // fields
    Layer *pBgFar = nullptr;        // pointer to settings for far background

    double scrollBgFar = 0;         // retain smooth movement during long runs
    //Layer diffuse = { 0.05f, 48.0f, 0.9f, CRGB(90, 8, 160), CRGB(4, 40, 140) };
    //Layer nebula  = { 0.30f, 14.0f, 2.4f, CRGB(180, 12, 0), CRGB(0, 100, 35) };
    //Layer stars   = { 0.50f, 54.0f, 0.6f, CRGB(100, 100, 120), CRGB(255, 255, 255) };
    //float speed = 16.0f;         // LEDs per second, right to left

    private:        // fields
    ui16 seed = 117;

    public:         // methods
    void Awake() override;
    void Update() override;
    void Draw(LedsStrip* pLeds) override;

    private:        // methods
};