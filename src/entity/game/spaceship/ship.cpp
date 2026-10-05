#include "ship.h"
#include "math.h"



void Ship::Awake()
{
    pos = 50;
    pxBody[0] = CRGB(170, 160, 160);    // tail
    pxBody[1] = CRGB(160, 160, 160);
    pxBody[2] = CRGB(210, 210, 210);
    pxBody[3] = CRGB(210, 210, 210);
    pxBody[4] = CRGB(210, 210, 210);
    pxBody[5] = CRGB(170, 170, 190);
    pxBody[6] = CRGB(170, 170, 200);    // head
}



void Ship::Update()
{
    // ship
    float secondsElapsed = timeSrv.time();
    float offset = sinf(secondsElapsed * speed) * 30;
    finalPos = pos + offset;
    // flare
    if (timeSrv.now() - flareLastMs >= 100 / flareFlickSpeed)
    {
        flareLastMs = timeSrv.now();
        flareLength = random(flareMin * 100, flareMax * 100) * 0.01f;
    }
}



void Ship::Draw(LedsStrip* pLeds)
{
    this->DrawFlare(pLeds);
    this->DrawBody(pLeds);
}


void Ship::DrawBody(LedsStrip* pLeds)
{
    for (int i = 0; i < SZBODY; ++i)
    {
        CRGB rgb = pxBody[i];
        pLeds->SetColor(rgb, finalPos + i);
    }
}


void Ship::DrawFlare(LedsStrip* pLeds)
{
    // engine flare: orange at the tail, fading red toward the tip
    float invLength = 1.0f / flareLength;

    for (int i = 0; i < flareLength; ++i)
    {
        float strength = 1.0f - i * invLength;
        float coverage = flareLength - i;
        if (coverage > 1.0f) coverage = 1.0f;

        CRGB rgb(
            (ui8)(255.0f * strength * coverage),
            (ui8)(100.0f * strength * strength * coverage),
            0);

        pLeds->OverlayColor(rgb, finalPos - 1.0f - i);
    }
}