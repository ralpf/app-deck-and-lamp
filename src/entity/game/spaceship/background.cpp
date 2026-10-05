#include "background.h"
#include <math.h>



//..........................................................................................HELPERS

static float background_contrast(float mask, float contrast)
{
    return constrain((mask - 0.5f) * max(contrast, 0.0f) + 0.5f, 0.0f, 1.0f);
}


static float background_mask(float coordinate, ui16 seed, float contrast)
{
    // each FastLED noise cell spans 256 coordinate units
    ui32 wholeCoordinate = (ui32)coordinate;
    ui16 x = (ui16)wholeCoordinate;
    float fraction = coordinate - wholeCoordinate;
    float left = inoise8(x, seed);
    float right = inoise8((ui16)(x + 1), seed);
    float mask = (left + (right - left) * fraction) / 255.0f;
    return background_contrast(mask, contrast);
}


static CRGB background_color(const BackgroundLayer& layer, float mask, float brightness)
{
    CRGB rgb = blend(layer.color1, layer.color2, (ui8)(mask * 255.0f));
    float strength = constrain(layer.intensity, 0.0f, 1.0f) * brightness;
    rgb.nscale8((ui8)(strength * 255.0f));
    return rgb;
}


static ui32 background_hash(ui32 value)
{
    // stable random values for each star cell, without per-frame randomization
    value ^= value >> 16;
    value *= 0x7FEB352D;
    value ^= value >> 15;
    value *= 0x846CA68B;
    value ^= value >> 16;
    return value;
}


//......................................................................................METHODS-PUB

void Background::Awake()
{
    scroll = 0;
}


void Background::Update()
{
    scroll += timeSrv.dt() * 0.001 * max(speed, 0.0f);
}


void Background::Draw(LedsStrip* pLeds)
{
    if (pLeds == nullptr) return;

    float diffuseStep = 256.0f / max(diffuse.scale, 1.0f);
    float nebulaStep = 256.0f / max(nebula.scale, 1.0f);
    float diffusePhase = (float)fmod(scroll * diffuseStep, 65536.0);
    float nebulaPhase = (float)fmod(scroll * nebulaStep, 65536.0);

    for (ui16 i = 0; i < pLeds->Count(); ++i)
    {
        CRGB rgb(0, 0, 0);

        if (diffuse.intensity > 0)
        {
            float mask = background_mask(diffusePhase + i * diffuseStep, 13731, diffuse.contrast);
            // use the same mask for tint and brightness, with a persistent base glow
            rgb = background_color(diffuse, mask, 0.25f + mask * 0.75f);
        }

        if (nebula.intensity > 0)
        {
            float mask = background_mask(nebulaPhase + i * nebulaStep, 43127, nebula.contrast);
            // leave dark gaps between the denser clouds
            float cloud = constrain((mask - 0.55f) / 0.45f, 0.0f, 1.0f);
            rgb += background_color(nebula, mask, cloud);
        }

        // compose first, then apply strip brightness and gamma once
        pLeds->SetColor(rgb, i);
    }

    if (stars.intensity > 0)
    {
        double wholeScroll = floor(scroll);
        i32 firstWorldPixel = (i32)wholeScroll;
        float fraction = (float)(scroll - wholeScroll);

        // include the incoming star just beyond the right edge
        for (i32 i = 0; i <= pLeds->Count(); ++i)
        {
            CRGB rgb = StarColor(firstWorldPixel + i);
            if (rgb.r == 0 && rgb.g == 0 && rgb.b == 0) continue;

            // retain fractional movement and add light over the clouds
            pLeds->OverlayColor(rgb, (float)i - fraction);
        }
    }
}


//......................................................................................METHODS-PRV

CRGB Background::StarColor(i32 worldPixel) const
{
    i32 spacing = (i32)constrain(stars.scale, 1.0f, 65535.0f);
    ui32 cell = (ui32)(worldPixel / spacing);
    ui32 hash = background_hash(cell + 27183);
    i32 starPixel = hash % spacing;
    if (worldPixel % spacing != starPixel) return CRGB(0, 0, 0);

    float tint = ((hash >> 8) & 255) / 255.0f;
    float mask = ((hash >> 16) & 255) / 255.0f;
    float brightness = background_contrast(mask, stars.contrast);
    return background_color(stars, tint, brightness);
}
