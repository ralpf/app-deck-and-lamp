#include "fireDeck.h"


//......................................................................................METHODS-PUB

void FireDeck::Update()
{
    ui32 nowMs = millis();
    ui32 elapsedMs = nowMs - lastUpdateMs;
    lastUpdateMs = nowMs;

    // retain fractional time at low speeds; widen only this once-per-frame product
    ui64 scaledTime = (ui64)elapsedMs * speed + timeRemainder;
    elapsedMs = scaledTime / 100;
    timeRemainder = scaledTime % 100;
    animationMs += elapsedMs;

    _UpdatePalette();
    flare.Update(elapsedMs, *this);

    for (ui16 i = 0; i < leds.Count(); ++i)
    {
        ui8 heat = qadd8(_BaseHeat(i), flare.HeatAt(i));

        // keep interpolation away from the palette's wrap back to dark red
        ui8 paletteIdx = (ui16)heat * 240 / 255;
        leds.SetColor(ColorFromPalette(palette, paletteIdx, 255, LINEARBLEND), i);
    }
}

void FireDeck::Setup(Settings::Deck::Fire settings)
{
    this->temperature = settings.temperature;
    this->speed       = settings.speed;
}

//......................................................................................METHODS-PRV

void FireDeck::_UpdatePalette()
{
    ui16 kelvin = constrain(temperature, 1900U, 7000U);
    if (kelvin == paletteTemperature) return;
    paletteTemperature = kelvin;

    // FastLED temperature anchors approximate the hottest color, not flame physics
    CRGB hot;
    if (kelvin <= 5200)
    {
        ui8 mix = (ui32)(kelvin - 1900) * 255 / (5200 - 1900);
        hot = blend(CRGB(Candle), CRGB(CarbonArc), mix);
    }
    else
    {
        ui8 mix = (ui32)(kelvin - 5200) * 255 / (7000 - 5200);
        hot = blend(CRGB(CarbonArc), CRGB(OvercastSky), mix);
    }

    palette = CRGBPalette16(CRGB(2, 0, 0), CRGB(110, 3, 0), CRGB(255, 65, 0), hot);
}


ui8 FireDeck::_BaseHeat(ui16 idx)
{
    // fixed spatial coordinates let patches form and fade instead of scrolling
    ui8 broadGlow = inoise8((ui16)(idx * 16 + 1703), (ui16)(animationMs / 16));
    ui8 fineFlicker = inoise8((ui16)(idx * 64 + 9107), (ui16)animationMs);
    i16 heat = 20 + ((i16)broadGlow - 64) * 3 / 2 + ((i16)fineFlicker - 128) / 3;
    return constrain(heat, 8, 220);
}

//......................................................................................FLARE-PUB

void FireDeck::Flare::Update(ui32 elapsedMs, FireDeck& fire)
{
    _Advance(elapsedMs, fire);

    ui16 durationMs = riseMs + fadeMs;
    ui8 envelope = 0;
    if (ageMs < riseMs)
        envelope = (ui32)ageMs * 255 / riseMs;
    else if (ageMs < durationMs)
        envelope = (ui32)(durationMs - ageMs) * 255 / fadeMs;

    // a curved decay keeps the brightest part brief
    level = scale8(strength, scale8(envelope, envelope));
}


ui8 FireDeck::Flare::HeatAt(ui16 idx) const
{
    if (level == 0) return 0;

    i32 distance8 = (i32)((ui32)idx << 8) - (i32)position8;
    if (distance8 < 0) distance8 = -distance8;
    if (distance8 >= radius8) return 0;

    ui8 coverage = (ui32)(radius8 - distance8) * 255 / radius8;
    ui8 softCoverage = ease8InOutQuad(coverage);
    return scale8(level, softCoverage);
}

//......................................................................................FLARE-PRV

void FireDeck::Flare::_Advance(ui32 elapsedMs, FireDeck& fire)
{
    ui16 durationMs = riseMs + fadeMs;
    if (ageMs < durationMs)
    {
        ui16 remainingMs = durationMs - ageMs;
        if (elapsedMs < remainingMs)
        {
            ageMs += elapsedMs;
            return;
        }
        elapsedMs -= remainingMs;
        ageMs = durationMs;
        waitMs = random16(1500, 5001);
    }

    if (elapsedMs < waitMs)
    {
        waitMs -= elapsedMs;
        return;
    }

    // prefer a warm region, without always choosing the hottest pixel
    ui16 position = random16(fire.leds.Count());
    for (ui8 i = 0; i < 3; ++i)
    {
        ui16 candidate = random16(fire.leds.Count());
        if (fire._BaseHeat(candidate) > fire._BaseHeat(position)) position = candidate;
    }

    position8 = ((ui32)position << 8) + random8();
    ui32 lastPixel8 = (ui32)(fire.leds.Count() - 1) << 8;
    if (position8 > lastPixel8) position8 = lastPixel8;
    radius8 = random16(384, 1025);       // full width about 3..8 pixels
    strength = random8(120, 201);
    riseMs = random16(20, 61);
    fadeMs = random16(150, 401);
    ageMs = 0;
    waitMs = 0;
}
