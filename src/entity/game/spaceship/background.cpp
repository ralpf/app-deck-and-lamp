#include "background.h"
#include <math.h>


using Layer = SpaceshipGame::Settings::Background::Layer;


//..........................................................................................HELPERS


// scale: noise frequency, larger values make smaller features
static float sampleNoise(float x, ui16 seed, float scale, float threshold, float softness)
{
    double coordinate = fmod((double)x * scale * 256.0, 4294967296.0);
    if (coordinate < 0) coordinate += 4294967296.0;
    float noise = inoise16((ui32)coordinate, (ui32)seed << 8) / 65535.0f;
    float t = (noise - threshold) / max(softness, 0.0001f) + 0.5f;
    t = constrain(t, 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}


static float sampleLayer(float x, const Layer& layer)
{
    float mask = sampleNoise(x, layer.seed, layer.scale, layer.treshold, layer.softness);
    ui16 seedDetail = layer.seed + 10000;
    float detail = sampleNoise(x, seedDetail, layer.detailScale, 0.5f, 0.05f);
    return mask * (1.0f - detail * layer.detailMult);
}


static CRGB composeLayer(const CRGB rgb, const Layer& layer, ui16 i)
{
    if (layer.opacity8 == 0) return rgb;

    double whole = floor(layer.scroll);
    float fract = (float)(layer.scroll - whole);
    float x = (float)(whole + i);

    float left = sampleLayer(x, layer);
    float right = sampleLayer(x + 1.0f, layer);
    float mask = left + (right - left) * fract;
    CRGB color = blend(layer.color1, layer.color2, (ui8)(mask * 255.0f));

    switch (layer.blendType)
    {
        case 0:     // uniform alpha
        return blend(rgb, color, layer.opacity8);

        case 1:     // clouds opaque, gaps transparent
        return blend(rgb, color, (ui8)(mask * layer.opacity8));

        default:    // unsupported mode, leave underlying layers visible
        return rgb;
    }
}


//......................................................................................METHODS-PUB

void Background::Awake()
{
    if (pStg == nullptr) return;
    pStg->lFar.scroll = 0;
    pStg->lNear.scroll = 0;
}


void Background::Update()
{
    if (pStg == nullptr) return;
    float dt = timeSrv.dt() * 0.001f * pStg->globalSpeed;
    pStg->lFar.scroll  += dt * pStg->lFar.speed;
    pStg->lNear.scroll += dt * pStg->lNear.speed;
}


void Background::Draw(LedsStrip* pLeds)
{
    if (pLeds == nullptr) return;
    if (pStg == nullptr) return;

    for (ui16 i = 0; i < pLeds->Count(); ++i)
    {
        CRGB rgb = CRGB::Black;
        rgb = composeLayer(rgb, pStg->lFar, i);
        rgb = composeLayer(rgb, pStg->lNear, i);
        // apply color
        pLeds->SetColor(rgb, i);
    }
}
