#include "utils.h"


ui32 html_2_ui32(const char* buff)  // pass copy by value
{
    // lambda
    auto hex = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };

    ui32 err_col = 0xFF8080;
    if (!buff) return err_col;

    ui16 len = 0;
    while (buff[len]) ++len;

    if (len != 6 && len != 7) return err_col;
    if (len == 7)
    {
        if (buff[0] != '#') return err_col;
        buff++;
    }

    int h0 = hex(buff[0]), h1 = hex(buff[1]);
    int h2 = hex(buff[2]), h3 = hex(buff[3]);
    int h4 = hex(buff[4]), h5 = hex(buff[5]);
    if (h0 < 0 || h1 < 0 || h2 < 0 || h3 < 0 || h4 < 0 || h5 < 0) return err_col;

    ui8 r = (ui8)((h0 << 4) | h1);
    ui8 g = (ui8)((h2 << 4) | h3);
    ui8 b = (ui8)((h4 << 4) | h5);

    return ((ui32)r << 16) | ((ui32)g << 8) | b;
}


ui32 rgb_2_ui32(CRGB rgb)
{
    return ((ui32)rgb.r << 16) | ((ui32)rgb.g << 8) | rgb.b;
}


void rgb2hsv(const CRGB &rgb, CHSV &hsv)
{
    float r = rgb.r / 255.0;
    float g = rgb.g / 255.0;
    float b = rgb.b / 255.0;

    float max = fmax(r, fmax(g, b));
    float min = fmin(r, fmin(g, b));
    float delta = max - min;

    hsv.v = max * 255;

    if (delta == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return;
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

    hue *= 60.0;
    if (hue < 0) hue += 360.0;

    hsv.h = (hue / 360.0) * 255;
}


void hsv2rgb(const CHSV &hsv, CRGB &rgb)
{
    float h = (hsv.h / 255.0) * 360.0;
    float s = hsv.s / 255.0;
    float v = hsv.v / 255.0;

    int i = floor(h / 60.0);
    float f = (h / 60.0) - i;
    float p = v * (1.0 - s);
    float q = v * (1.0 - s * f);
    float t = v * (1.0 - s * (1.0 - f));

    switch (i % 6) {
        case 0: rgb = CRGB(v * 255, t * 255, p * 255); break;
        case 1: rgb = CRGB(q * 255, v * 255, p * 255); break;
        case 2: rgb = CRGB(p * 255, v * 255, t * 255); break;
        case 3: rgb = CRGB(p * 255, q * 255, v * 255); break;
        case 4: rgb = CRGB(t * 255, p * 255, v * 255); break;
        case 5: rgb = CRGB(v * 255, p * 255, q * 255); break;
    }
}


bool palettesApproxEqual(const CRGBPalette16& lhs, const CRGBPalette16& rhs, ui8 tolerance)
{
    for (int i = 0; i < 16; ++i)
    {
        CRGB ca = lhs[i]; CRGB cb = rhs[i];
        if (abs(ca.r - cb.r) > tolerance || abs(ca.g - cb.g) > tolerance || abs(ca.b - cb.b) > tolerance)
            return false;
    }
    return true;
}


void updateGammaLutTable256(ui8* array256, float gamma)
{
    gamma = gamma > 0.001 ? gamma : 0.001;
    // can't use ui8 as it will  overflow 255->0 resulting in infinite loop
    for (ui16 i = 0; i < 256; ++i)
        array256[i] = (ui8)(powf(i/255.0f, gamma) * 255.0f + 0.5f);
}