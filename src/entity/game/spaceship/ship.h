#pragma once
#include "gameObject.h"


class Ship : public GObject
{
    private:
    static constexpr ui8 SZBODY = 7;

    CRGB pxBody[SZBODY];
    float speed = 0.2f;
    float finalPos = 0;
    // flare
    float flareLength = 10.0f;
    float flareFlickSpeed = 1.8f;
    ui32  flareLastMs = 0;
    ui8   flareMin = 6;
    ui8   flareMax = 12;



    public:
    void Awake() override;
    void Update() override;
    void Draw(LedsStrip* pLeds) override;

    private:
    void DrawFlare(LedsStrip* pLeds);
    void DrawBody(LedsStrip* pLeds);
};