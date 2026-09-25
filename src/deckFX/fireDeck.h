#pragma once
#include "../ledsStrip.h"
#include "../settings.h"



class FireDeck
{
    //....................................................................FIELDS
    private:
    LedsStrip& leds;

    ui32 lastUpdateMs = 0;
    ui32 animationMs = 0;
    ui8  timeRemainder = 0;

    CRGBPalette16 palette;
    ui16 paletteTemperature = 0;

    public:
    ui16 temperature = 4800;           // approximate hot-tip tint, 1900..7000 K
    ui16 speed = 100;                  // percent of normal speed; zero freezes animation

    //......................................................................CTOR
    public:
    FireDeck(LedsStrip& target) : leds(target)  { }

    //...............................................................METHODS-PUB
    public:
    void Setup(Settings::Deck::Fire settings);
    void Update();                      // called from main loop to update animation

    //...............................................................METHODS-PRV
    private:
    void _UpdatePalette();
    ui8  _BaseHeat(ui16 idx);

    //...............................................................NESTED-TYPE

    class Flare
    {
        //................................................................FIELDS
        private:
        ui16 waitMs = 2000;
        ui16 ageMs = 0;
        ui16 riseMs = 0;
        ui16 fadeMs = 0;
        ui32 position8 = 0;           // pixel position with 8 fractional bits
        ui16 radius8 = 0;
        ui8  strength = 0;
        ui8  level = 0;

        //...........................................................METHODS-PUB
        public:
        void Update(ui32 elapsedMs, FireDeck& fire);
        ui8  HeatAt(ui16 idx) const;

        //...........................................................METHODS-PRV
        private:
        void _Advance(ui32 elapsedMs, FireDeck& fire);
    };

    Flare flare;                      // requires the full nested type definition
};
