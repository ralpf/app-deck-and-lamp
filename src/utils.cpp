#include "utils.h"


ui32 html_2_UI32(String htmlColor)  // pass copy by value
{
    ui32 err_col = 0xFF8080;

    auto len = htmlColor.length();
    if (len != 6 && len != 7) return err_col;
    if (len == 7 && htmlColor[0] != '#') return err_col;

    if (len == 7) htmlColor = htmlColor.substring(1, 7);

    ui8 r = strtol(htmlColor.substring(0, 2).c_str(), NULL, 16);
    ui8 g = strtol(htmlColor.substring(2, 4).c_str(), NULL, 16);
    ui8 b = strtol(htmlColor.substring(4, 6).c_str(), NULL, 16);

    return (r << 16) | (g << 8) | b;
}


ui32 rgb_2_UI32(CRGB rgb)
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