#include "utils.h"



//............................................................................................HEADER

ui32 rgb_2_ui32(const CRGB& rgb)
{
    return ((ui32)rgb.r << 16) | ((ui32)rgb.g << 8) | rgb.b;
}


ui32 hsv_2_ui32(const CHSV& hsv)
{
    return ((ui32)hsv.h << 16) | ((ui32)hsv.s << 8) | hsv.v;
}


CRGB html_2_rgb(const char* html)
{
    auto hex = [](char c) -> int {          // lambda
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };

    ui32 err_col = 0xFF8080;
    if (!html) return err_col;

    ui16 len = 0;
    while (html[len]) ++len;

    if (len != 6 && len != 7) return err_col;
    if (len == 7)
    {
        if (html[0] != '#') return err_col;
        html++;
    }

    int h0 = hex(html[0]), h1 = hex(html[1]);
    int h2 = hex(html[2]), h3 = hex(html[3]);
    int h4 = hex(html[4]), h5 = hex(html[5]);
    if (h0 < 0 || h1 < 0 || h2 < 0 || h3 < 0 || h4 < 0 || h5 < 0) return err_col;

    ui8 r = (ui8)((h0 << 4) | h1);
    ui8 g = (ui8)((h2 << 4) | h3);
    ui8 b = (ui8)((h4 << 4) | h5);

    return CRGB(r,g,b);
}


ui32 html_2_rgb_2_ui32(const char* buff)
{
    CRGB rgb = html_2_rgb(buff);
    return rgb_2_ui32(rgb);
}


CRGB ui32_2_rgb(ui32 rgb32)
{
    return CRGB(rgb32);
}


CHSV ui32_2_hsv(ui32 hsv32)
{
    return CHSV {
        (ui8)((hsv32 >> 16) & 0xFF),
        (ui8)((hsv32 >>  8) & 0xFF),
        (ui8)( hsv32        & 0xFF)
    };
}


CHSV rgb_2_hsv_slow(const CRGB &rgb)
{
    float r = rgb.r / 255.0f;
    float g = rgb.g / 255.0f;
    float b = rgb.b / 255.0f;

    float max = fmax(r, fmax(g, b));
    float min = fmin(r, fmin(g, b));
    float delta = max - min;

    CHSV hsv;
    hsv.v = max * 255;

    if (delta == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = (delta / max) * 255;
    float hue;

    if (max == r) {
        hue = (g - b) / delta;
    } else if (max == g) {
        hue = 2.0 + (b - r) / delta;
    } else {
        hue = 4.0 + (r - g) / delta;
    }

    hue *= 60;
    if (hue < 0) hue += 360;

    hsv.h = (hue / 360) * 255;
    return hsv;
}


CRGB hsv_2_rgb_slow(const CHSV &hsv)
{
    float h = (hsv.h / 255.0) * 360.0;
    float s = hsv.s / 255.0;
    float v = hsv.v / 255.0;

    int i = floor(h / 60.0);
    float f = (h / 60.0) - i;
    float p = v * (1.0 - s);
    float q = v * (1.0 - s * f);
    float t = v * (1.0 - s * (1.0 - f));

    CRGB rgb;
    switch (i % 6) {
        case 0: rgb = CRGB(v * 255, t * 255, p * 255); break;
        case 1: rgb = CRGB(q * 255, v * 255, p * 255); break;
        case 2: rgb = CRGB(p * 255, v * 255, t * 255); break;
        case 3: rgb = CRGB(p * 255, q * 255, v * 255); break;
        case 4: rgb = CRGB(t * 255, p * 255, v * 255); break;
        case 5: rgb = CRGB(v * 255, p * 255, q * 255); break;
    }
    return rgb;
}


CHSV rgb_2_hsv_fast(const CRGB &rgb)
{
    return rgb2hsv_approximate(rgb);    // fastled
}


CRGB hsv_2_rgb_fast(const CHSV &hsv)
{
    return hsv;     // fastled implicit cast
}


bool palette_compare(const CRGBPalette16& lhs, const CRGBPalette16& rhs, ui8 tolerance)
{
    for (int i = 0; i < 16; ++i)
    {
        CRGB ca = lhs[i];
        CRGB cb = rhs[i];

        if (abs(ca.r - cb.r) > tolerance || abs(ca.g - cb.g) > tolerance || abs(ca.b - cb.b) > tolerance)
            return false;
    }
    return true;
}


void update_gammaLUT_table256(ui8* array256, float gamma)
{
    if (!array256) return;
    gamma = gamma > 0.001 ? gamma : 0.001;
    // can't use ui8 as it will  overflow 255->0 resulting in infinite loop
    for (ui16 i = 0; i < 256; ++i)
        array256[i] = (ui8)(powf(i/255.0f, gamma) * 255.0f + 0.5f);
}