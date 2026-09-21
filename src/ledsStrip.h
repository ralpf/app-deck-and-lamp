#pragma once

#include <math.h>       // for fmodf()
#include <memory>
#include <FastLED.h>
#include <Types.h>
#include <colorUtils.h>
#include <timeService.h>


// 
class LedsStrip
{
    const ui16 ledCount;
    const float LED2PAL;                                 // used to map ledCount of leds on [0, 255] range of palette


    //....................................................................FIELDS
    private:

    CRGB*          leds;                // don't write directly, use _SetRGB
    CRGBPalette16  pal;                 // active palette
    ui8*           gammaLUT = nullptr;  // 256 element LUT array. Can be null

    public:
    ui8       bright = 200;

    //......................................................................CTOR
    protected:

    LedsStrip(CRGB* buffer, ui16 count)
        : ledCount(count), LED2PAL(255.0f / count), leds(buffer), rand(count)
    {
    }

    ~LedsStrip() = default;

    LedsStrip(const LedsStrip&) = delete;
    LedsStrip& operator=(const LedsStrip&) = delete;

    //...............................................................METHODS-PUB
    public:

    ui16 Count() const
    {
        return ledCount;
    }


    void SetColor(const CRGB& rgb)
    {
        for (ui16 i = 0; i < ledCount; ++i) _SetRGB(i, rgb);
    }


    void SetColor(const CRGB& rgb, ui16 fromIdx, ui16 toIdx)
    {
        fromIdx = constrain(fromIdx, 0, ledCount);
        toIdx   = constrain(toIdx, 0, ledCount);
        for (ui16 i = fromIdx; i < toIdx; ++i) _SetRGB(i, rgb);
    }


    void SetColor(const CRGB& rgb, ui16 idx)
    {
        _SetRGB(idx, rgb);
    }


    void OverlayColor(const CRGB& rgb, float idx)
    {
        if (!(idx > -1.0f && idx < ledCount)) return;

        i32 leftIdx = (i32)floorf(idx);
        ui16 weight = (ui16)((idx - leftIdx) * 256.0f + 0.5f);
        CRGB color = _CorrectRGB(rgb);

        // split the corrected color without losing light to rounding
        CRGB right((color.r * weight + 128) / 256,
                   (color.g * weight + 128) / 256,
                   (color.b * weight + 128) / 256);
        CRGB left(color.r - right.r, color.g - right.g, color.b - right.b);

        // add with saturation; off-strip contributions are discarded
        if (leftIdx >= 0) leds[leftIdx] += left;
        if (leftIdx + 1 < ledCount) leds[leftIdx + 1] += right;
    }


    void ScaleDown(ui8 scale)
    {
        for (ui16 i = 0; i < ledCount; ++i) leds[i].nscale8(scale);
    }


    void SetPalette(const CRGBPalette16& pal16)
    {
        pal = pal16;
        _ApplyPalette();
    }


    void SetPaletteFX(const CRGBPalette16& pal16, ui8 maxChange=(ui8)24U)
    {
        nblendPaletteTowardPalette(pal, const_cast<CRGBPalette16&>(pal16), maxChange);
        _ApplyPalette();
    }


    void SetGammaLutTable(ui8* array256)    // can pass null to remove gamma correction
    {
        gammaLUT = array256;
    }

    //...............................................................METHODS-PRV
    private:

    void _ApplyPalette()
    {
        for (ui16 i = 0; i < ledCount; ++i)
        {
            ui16 pos = rand.isOn ? rand.Remap(i) : i;
            ui8 palIdx = (int)(pos * LED2PAL + anim.offset);    // use int for correct wrap
            _SetRGB(i, ColorFromPalette(pal, palIdx));
        }
    }


    void _SetRGB(ui16 idx, CRGB rgb)
    {
        idx = constrain(idx, 0, ledCount-1);
        leds[idx] = _CorrectRGB(rgb);
    }


    CRGB _CorrectRGB(CRGB rgb)
    {
        ui8 luma = bright;

        //              can add more filters or corrections
        if (gammaLUT != nullptr)        // gamma correction
        {
            rgb = CRGB(gammaLUT[rgb.r], gammaLUT[rgb.g], gammaLUT[rgb.b]);
            luma = gammaLUT[luma]; // lut the brightness too
        }
        // apply leds brightness
        return rgb.nscale8(luma);
    }

    //...............................................................NESTED-TYPE

    struct Animation
    {
        float speed  = 1.0;                      // for animation
        float offset = 0.0;                      // used for animations

        void update()
        {
            offset += timeSrv.dt() * speed * 0.01f;
            // fix floating point err accumulation
            offset = fmodf(offset, 256.0f);
        }
    };

    //...............................................................NESTED-TYPE

    struct Randomz
    {
        public:
        bool isOn = false;

        private:
        std::unique_ptr<ui16[]> randIdxs;
        const ui16 ledCount;

        public:
        Randomz(ui16 count) : randIdxs(new ui16[count]), ledCount(count)
        {
            for (ui16 i = 0; i < ledCount; ++i) randIdxs[i] = i;
            DoFisherYatesShuffle();
        }

        private:
        void DoFisherYatesShuffle()
        {
            for (ui16 i = ledCount - 1; i > 0; --i)
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

    public:                 // this can't be moved bc require full type definition
    Animation anim;
    Randomz   rand;
};
