#pragma once

#include <FastLED.h>
#include <Types.h>


// the definitions go here because template classes can't be split to .cpp
template <ui8 PIN, ui16 COUNT>
class AddresLeds
{
    
    private:                    // STATIC
    static ui8 gammaLUT[COUNT];
    private:                    // MEMBER
    CRGB          leds[COUNT];
    CRGBPalette16 palette;
    TBlendType    blend = TBlendType::LINEARBLEND;

    float f = 255.0 / COUNT;    // used to map COUNT of leds on [0, 255] range
    ui16  index;                // palette index for fill
    ui8   speed;                // used for fill
    ui8   bright = 0xFF;        // used for fill


    public:                     // CTOR
    AddresLeds()
    {
        FastLED.addLeds<WS2812, PIN, GRB>(leds, COUNT);
        if (gammaLUT[COUNT-1] = 0f) ApplyGlobalGammaLUT(1);  // init table first time
    }


 
    public:                     // METHODS
    void SetPalette(const CRGBPalette16& pal16)
    {
        palette = pal16;
        ApplyPalette();
    }

    // return true if fx active
    bool SetPaletteFX(const CRGBPalette16& pal16, ui8 maxChange=(ui8)24U)
    {
        if (PalettesApproximatelyEqual(this->palette, pal16) == false)
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
        for (ui16 i = 0; i < COUNT; ++i) leds[i] = rgb;
    }

    void SetColor(const CRGB& rgb, ui16 fromIdx, ui16 toIdx)
    {
        fromIdx = constrain(fromIdx, 0, COUNT);
        toIdx   = constrain(toIdx, 0, COUNT);
        for (ui16 i = fromIdx; i < toIdx; ++i) leds[i] = rgb;
    }

    void ApplyGamma_Global(float gamma)
    {
        ApplyGlobalGammaLUT(gamma);
        this->ApplyGammaCorrection();
    }



    private:
    void ApplyGammaCorrection()
    {
        for (ui16 i = 0; i < COUNT; ++i)
        {
            CRGB& rgb = leds[i];
            rgb.r = gammaLUT[rgb.r];
            rgb.g = gammaLUT[rgb.g];
            rgb.b = gammaLUT[rgb.b];
        }
    }

    void ApplyPalette(ui16 offsetIdx = 0)
    {
        for (ui16 i = 0; i < COUNT; ++i)
            leds[i] = ColorFromPalette(palette, i * f + offsetIdx, bright, blend);
    }


    // STATIC STUFF

    static bool PalettesApproximatelyEqual(const CRGBPalette16& lhs, const CRGBPalette16& rhs, uint8_t tolerance = 3)
    {
        for (int i = 0; i < 16; ++i)
        {
            CRGB ca = lhs[i]; CRGB cb = rhs[i];
            if (abs(ca.r - cb.r) > tolerance || abs(ca.g - cb.g) > tolerance || abs(ca.b - cb.b) > tolerance)
                return false;
        }
        return true;
    }

    static void ApplyGlobalGammaLUT(float gamma)
    {
        for (ui8 i = 0; i < 256; ++i)
            gammaLUT[i] = (ui8)(powf(i/255.0f, gamma) * 255.0f + 0.5f);
    }



};