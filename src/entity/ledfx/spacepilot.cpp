#include "spacepilot.h"
#include <timeService.h>



void SpacePilot::Update()
{
    // fix pointers
    data.pS = &S;
    data.pL = &leds;
    // update runtime data
    data.distance += timeSrv.dt() * S.speed / 1000.0f;
    // update entities
    cam.Update(data);
}


void SpacePilot::Octave::SetDistance(double dist)
{
    scale256 = scale * 256.0f;
    travel = (ui32)(dist * scale256);
}


inline float SpacePilot::Octave::Sample(float x) const
{
    // call SetDistance() first !
    ui32 coord32 = (ui32)(x * scale256) - travel;
    return inoise16(coord32, seed << 8) / 65535.0f * weight;
}


void SpacePilot::CloudSide::Update(Data& data)
{
    // a bit more readable
    ui16 totalN  = data.pL->Count();
    ui16 centerN = data.pS->centerWidth;
    if (centerN > totalN / 10) centerN = totalN / 10;    // no more than 10% of leds

    float turn = constrain(data.turn, -1.0f, 1.0f);
    float sideTurn = isLeft ? turn : -turn;

    float countF = (totalN - centerN) * 0.5f + (totalN + centerN) * 0.5f * sideTurn;
    ui16  count  = (ui16)(constrain(countF, 0.0f, (float)totalN) + 0.5f);
    if (count == 0) return;

    ui16 i0 = isLeft ? count -1 : totalN - count;   // index 0, is in the middle for both sides

    octaveLarge.SetDistance(data.distance);
    octaveMedium.SetDistance(data.distance);
    octaveSmall.SetDistance(data.distance);

    float totalWeight = octaveLarge.weight + octaveMedium.weight + octaveSmall.weight;
    if (totalWeight <= 0) return;

    for (ui16 i = 0; i < count; ++i)
    {
        // curr led index. We move left or rith from center
        ui16 iL = isLeft ? i0 - i : i0 + i;
        // obtain mask
        float mask = (octaveLarge.Sample(i) + octaveMedium.Sample(i) + octaveSmall.Sample(i));
        mask /= totalWeight;    // normalized mask
        // draw mask
        ui8 Y = (ui8)(mask * 255.0f);
        data.pL->SetColor(CRGB(Y,Y,Y), iL);
    }
}


void SpacePilot::CloudLayer::Update(Data& data)
{
    left.Update(data);
    right.Update(data);
}


void SpacePilot::Background::Update(Data& data)
{
    far.Update(data);
}


void SpacePilot::Environment::Update(Data& data)
{
    bg.Update(data);
}


void SpacePilot::Cockpit::Update(Data& data)
{
}


void SpacePilot::Camera::Update(Data& data)
{
    env.Update(data);
    ckp.Update(data);
}