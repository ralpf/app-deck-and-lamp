#pragma once
#include "../../core/ientity.h"
#include <FastLED.h>

// includes all settings type definitions for Starship game


namespace SpaceshipGame
{
    struct Settings : ISetting
    {
        struct BackgroundLayer
        {
            float scale;            // size feature, smaller - bigger size
            float treshold;         // ballance of colors
            float softness;         // spreading
            float speed;
            float detailScale;
            float detailMult;
            ui8   opacity8;
            CRGB color1;
            CRGB color2;
        };

        //http://192.168.100.201/esp/deck/starship/bg/far?speed=5&trsh=0.5&soft=0.5&scale=5&opac=64&detScale=100&detMult=0.6
        BackgroundLayer bgFar = { 5.0f, 0.5f, 0.5f, 3.0f, 100.0f, 0.6f, 64, CRGB::DarkBlue, CRGB::MediumVioletRed };

        // methods
        void Reset() override { *this = Settings{}; }
    };
};