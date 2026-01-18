#pragma once

#include <math.h>       // for fmodf()
#include <FastLED.h>
#include <Types.h>
#include <colorUtils.h>
#include <timeService.h>



// the definitions go here because template classes can't be split to .cpp

template <ui8 PIN, ui16 COUNT>
class Leds
{
    static constexpr float LED2PAL = 255.0f / COUNT;    // used to map COUNT of leds on [0, 255] range of palette
    struct Animation;                                   // fwd declaration, nested class
    struct Randomz;                                     // same


    //....................................................................FIELDS
    private:

    CRGB           leds[COUNT];         // don't write directly, use _SetRGB
    CRGBPalette16  pal;                 // active palette
    ui8*           gammaLUT = nullptr;  // 256 element LUT array. Can be null

    public:
    ui8       bright = 200;
    Animation anim;
    Randomz   rand;

    //......................................................................CTOR
    public:

    Leds()
    {
        // there is only templetized api
        // no pure code api to instantiate the led strip
        FastLED.addLeds<WS2812, PIN, GRB>(leds, COUNT);
    }

    //...............................................................METHODS-PUB
    public:

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


    void SetPalette(const CRGBPalette16& pal16)
    {
        pal = pal16;
        _ApplyPalette();
    }


    bool SetPaletteFX(const CRGBPalette16& pal16, ui8 maxChange=(ui8)24U)   // returns true if fx in process
    {
        if (maxChange > 0 && palette_compare(pal, pal16) == false)
        {
            nblendPaletteTowardPalette(pal, const_cast<CRGBPalette16&>(pal16), maxChange);
            _ApplyPalette();
            return true;      // with fx
        }

        SetPalette(pal16);
        return false;         //  w/o fx
    }


    void SetGammaLutTable(ui8* array256)    // can pass null to remove gamma correction
    {
        gammaLUT = array256;
    }

    //...............................................................METHODS-PRV
    private:

    void _ApplyPalette()
    {
        for (ui16 i = 0; i < COUNT; ++i)
        {
            ui8 palIdx = i * LED2PAL + anim.offset;
            _SetRGB( i, ColorFromPalette(pal, palIdx) );
        }
    }


    void _SetRGB(ui16 idx, CRGB rgb)
    {
        ui8 luma = bright;
        idx = constrain(idx, 0, COUNT-1);
        if (rand.isOn) idx = rand.Remap(idx);

        //              can add more filters or corrections
        if (gammaLUT != nullptr)        // gamma correction
        {
            rgb = CRGB(gammaLUT[rgb.r], gammaLUT[rgb.g], gammaLUT[rgb.b]);
            luma = gammaLUT[luma]; // lut the brightness too
        }
        // apply leds brightness
        leds[idx] = rgb.nscale8(luma);
    }

    //...............................................................NESTED-TYPE

    struct Animation
    {
        float speed  = 1.0;                      // for animation
        float offset = 0.0;                      // used for animations

        void update()
        {
            offset += timeSrv.dt() * speed;
            // fix floating point err accumulation
            offset = fmodf(offset, 256.0f);
        }
    };

    //...............................................................NESTED-TYPE

    struct Randomz
    {
        public:
        bool isOn;

        private:
        ui16 randIdxs[COUNT];

        public:
        Randomz()       // ctor
        {
            for (ui16 i = 0; i < COUNT; ++i) randIdxs[i] = i;
            DoFisherYatesShuffle();
        }

        private:
        void DoFisherYatesShuffle()
        {
            for (ui16 i = COUNT - 1; i > 0; --i)
            {
                ui16 j = random(i + 1);
                ui16 tmp    = randIdxs[i];
                randIdxs[i] = randIdxs[j];
                randIdxs[j] = tmp;
            }
        }

        public:
        ui16 Remap(ui16 idx)
        {
            return randIdxs[idx];
        }
    };
};