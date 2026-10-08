#include "background.h"
#include <math.h>



//..........................................................................................HELPERS


// x: position X for noise
// scale: the size of the features
// threshold: the distribution for colors
// softness: the smoothness of edges
float computeNoiseMask(float x, ui16 seed, float scale, float threshold, float softness)
{
    ui32 coordinate = (ui32)(x * scale * 256.0f);
    float noise = inoise16(coordinate, (ui32)seed << 8) / 65535.0f;

    float t = (noise - threshold) / max(softness, 0.0001f) + 0.5f;
    t = constrain(t, 0.0f, 1.0f);

    return t * t * (3.0f - 2.0f * t);
}


// static float background_contrast(float mask, float contrast)
// {
//     return constrain((mask - 0.5f) * max(contrast, 0.0f) + 0.5f, 0.0f, 1.0f);
// }


// static float background_mask(float coordinate, ui16 seed, float contrast)
// {
//     // each FastLED noise cell spans 256 coordinate units
//     ui32 wholeCoordinate = (ui32)coordinate;
//     ui16 x = (ui16)wholeCoordinate;
//     float fraction = coordinate - wholeCoordinate;
//     float left = inoise8(x, seed);
//     float right = inoise8((ui16)(x + 1), seed);
//     float mask = (left + (right - left) * fraction) / 255.0f;
//     return background_contrast(mask, contrast);
// }


// static CRGB background_color(const BackgroundLayer& layer, float mask, float brightness)
// {
//     CRGB rgb = blend(layer.color1, layer.color2, (ui8)(mask * 255.0f));
//     float strength = constrain(layer.intensity, 0.0f, 1.0f) * brightness;
//     rgb.nscale8((ui8)(strength * 255.0f));
//     return rgb;
// }


// static ui32 background_hash(ui32 value)
// {
//     // stable random values for each star cell, without per-frame randomization
//     value ^= value >> 16;
//     value *= 0x7FEB352D;
//     value ^= value >> 15;
//     value *= 0x846CA68B;
//     value ^= value >> 16;
//     return value;
// }


using Layer = SpaceshipGame::Settings::BackgroundLayer;

//......................................................................................METHODS-PUB

void Background::Awake()
{
    // reset layers
    scrollBgFar = 0;
}


void Background::Update()
{
    scrollBgFar += timeSrv.dt() * pBgFar->speed * 0.001;
}


void Background::Draw(LedsStrip* pLeds)
{
    if (pLeds == nullptr) return;
    if (pBgFar == nullptr) return;

    ui16 countLeds = pLeds->Count();
    ui16 seedFar = seed + 100;
    ui16 seedDetail = seedFar + 10000;
    Layer& layerFar = *pBgFar;

    double whole = floor(scrollBgFar);
    float fraction = (float)(scrollBgFar - whole);

    for (ui16 i = 0; i < countLeds; ++i)
    {
        // far background
        float x = (float)(whole + i);
        float left = computeNoiseMask(x, seedFar, layerFar.scale, layerFar.treshold, layerFar.softness);
        float right = computeNoiseMask(x + 1.0f, seedFar, layerFar.scale, layerFar.treshold, layerFar.softness);
        // carve details into color2
        float detailLeft = computeNoiseMask(x, seedDetail, layerFar.detailScale, 0.5f, 0.05f);
        float detailRight = computeNoiseMask(x + 1.0f, seedDetail, layerFar.detailScale, 0.5f, 0.05f);

        left  *= 1.0f - detailLeft  * layerFar.detailMult;
        right *= 1.0f - detailRight * layerFar.detailMult;
        // retain smooth movement
        float f = left + (right - left) * fraction;

        CRGB rgb = blend(layerFar.color1, layerFar.color2, (ui8)(f * 255.0f));
        // apply opacity
        rgb.nscale8(layerFar.opacity8);
        // apply resulting color
        pLeds->SetColor(rgb, i);
    }
}


//......................................................................................METHODS-PRV


