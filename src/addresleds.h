#pragma once

#include <FastLED.h>
#include <Types.h>
#include "utils.h"


// the definitions go here because template classes can't be split to .cpp
template <ui8 PIN, ui16 COUNT>
class AddresLeds
{
    
    private:                    // MEMBER
    CRGB          leds[COUNT];  // don't write directly, use SetRGB
    CRGBPalette16 palette;
    TBlendType    blend = TBlendType::LINEARBLEND;

    float f = 255.0 / COUNT;    // used to map COUNT of leds on [0, 255] range
    ui8   bright = 0xFF;        // used for fill
    ui8   animIdx;              // used for fill

    ui8*  gammaLUT = nullptr;   // 256 element LUT array. Can be null


    public:                     // CTOR
    AddresLeds()
    {
        FastLED.addLeds<WS2812, PIN, GRB>(leds, COUNT);
    }


 
    public:                     // METHODS
    void OnUpdate(const ui8 brightness, const ui8 animSpeed, const ui8 animSkip)
    {
        // NOTE: must call every update.
        static ui16 k;
        if (animSkip && ++k % animSkip != 0) return;  // skip every
        this->bright = brightness;
        if (animSpeed > 0) animIdx += animSpeed;
        else               animIdx  = 0;
    }

    void SetPalette(const CRGBPalette16& pal16)
    {
        palette = pal16;
        ApplyPalette();
    }

    // return true if fx active
    bool SetPaletteFX(const CRGBPalette16& pal16, ui8 maxChange=(ui8)24U)
    {
        if (maxChange > 0 && palettesApproxEqual(this->palette, pal16) == false)
        {
            nblendPaletteTowardPalette(this->palette, const_cast<CRGBPalette16&>(pal16), maxChange);
            ApplyPalette();
            return true;                // with fx
        }

        SetPalette(pal16);
        return false;                   // without fx
    }

    void SetColor(const CRGB& rgb)
    {
        for (ui16 i = 0; i < COUNT; ++i) _SetRGB(i, rgb);
    }

    void SetColor(const CRGB& rgb, ui16 fromIdx, ui16 toIdx)
    {
        fromIdx = constrain(fromIdx, 0, COUNT);
        toIdx   = constrain(toIdx, 0, COUNT);
        for (ui16 i = fromIdx; i < toIdx; ++i) _SetRGB(i, rgb);
    }

    void SetColor(const CRGB& rgb, ui16 idx)
    {
        _SetRGB(idx, rgb);
    }

    // TODO: debug this method
    void SetColor(const CRGB& rgb, std::initializer_list<ui16> iter)
    {
        for (ui16 x : iter) _SetRGB(x, rgb);
    }

    // can pass null to remove gamma correction
    void SetGammaLutTable(ui8* array256)
    {
        gammaLUT = array256;
    }


    private:
    void ApplyPalette()
    {
        const ui8 MAX = 0xFF;
        for (ui16 i = 0; i < COUNT; ++i)
        {
            ui8 idx = i * f + animIdx;
            _SetRGB( i, ColorFromPalette(palette, idx, MAX, blend) );
        }
    }

    
    protected:
    virtual void _SetRGB(ui16 idx, CRGB rgb)
    {
        idx = constrain(idx, 0, COUNT-1);
        if (gammaLUT != nullptr)        // gamma correction
            rgb = CRGB(gammaLUT[rgb.r], gammaLUT[rgb.g], gammaLUT[rgb.b]);
        // can add more filters or corrections
        // apply bright
        leds[idx] = rgb.nscale8(bright);
    }
};