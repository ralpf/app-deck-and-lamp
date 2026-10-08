#pragma once

#include "gameObject.h"
#include "settings.h"




class Background : public GObject
{
    public:
    using Settings = SpaceshipGame::Settings::Background;

    public:         // fields
    Settings *pStg;

    public:         // methods
    void Awake() override;
    void Update() override;
    void Draw(LedsStrip* pLeds) override;

    private:        // methods
};