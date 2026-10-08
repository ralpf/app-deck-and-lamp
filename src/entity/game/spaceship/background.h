#pragma once

#include "gameObject.h"
#include "settings.h"




class Background : public GObject
{
    public:
    using Layer = SpaceshipGame::Settings::BackgroundLayer;

    public:         // fields
    Layer *pStgFar  = nullptr;        // pointer to settings for far background
    Layer *pStgNear = nullptr;        // pointer to settings for near background

    double scrollFar = 0;         // retain smooth movement during long runs
    double scrollNear = 0;        // same

    private:        // fields
    ui16 seed = 117;

    public:         // methods
    void Awake() override;
    void Update() override;
    void Draw(LedsStrip* pLeds) override;

    private:        // methods
};