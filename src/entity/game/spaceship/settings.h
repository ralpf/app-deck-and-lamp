#pragma once
#include "../../core/ientity.h"
#include <FastLED.h>

// includes all settings type definitions for Starship game


namespace SpaceshipGame
{
    struct Settings : ISetting
    {
        struct Background
        {
            struct Layer
            {
                // !! on reorder, pls fix the initializers
                float scale;            // size feature, smaller - bigger size
                float treshold;         // ballance of colors
                float softness;         // spreading
                float speed;
                float detailScale;
                float detailMult;
                ui8   opacity8;
                ui8   blendType;
                CRGB  color1;
                CRGB  color2;
                // runtime, do not serialize
                ui16   seed;
                double scroll;
            };

            Layer  lNear;
            Layer  lFar;

        };


        Background bg;

        //http://192.168.100.201/esp/deck/starship/bg/far?speed=5&trsh=0.5&soft=0.5&scale=5&opac=64&detScale=100&detMult=0.6
        //BackgroundLayer bgFar  = { 5.0f, 0.5f, 0.5f, 3.0f, 100.0f, 0.6f, 64, 0, CRGB::DarkBlue, CRGB::MediumVioletRed };
        //BackgroundLayer bgNear = { 5.0f, 0.5f, 0.5f, 3.0f, 100.0f, 0.6f, 64, 1, CRGB::DarkBlue, CRGB::MediumVioletRed };

        // methods
        void Reset() override
        {
            // far layer
            bg.lFar.scale = 5.0f;
            bg.lFar.treshold = 0.5f;
            bg.lFar.softness = 0.5f;
            bg.lFar.speed = 3.0f;
            bg.lFar.detailScale = 100.0f;
            bg.lFar.detailMult = 0.6f;
            bg.lFar.opacity8 = 64;
            bg.lFar.blendType = 0;
            bg.lFar.color1 = CRGB::DarkBlue;
            bg.lFar.color2 = CRGB::MediumVioletRed;
            bg.lFar.seed = 450;
            bg.lFar.scroll = 0;
            // near layer
            bg.lNear.scale = 10.0f;
            bg.lNear.treshold = 0.6f;
            bg.lNear.softness = 0.6f;
            bg.lNear.speed = 6.0f;
            bg.lNear.detailScale = 100.0f;
            bg.lNear.detailMult = 0.1f;
            bg.lNear.opacity8 = 64;
            bg.lNear.blendType = 1;
            bg.lNear.color1 = CRGB::Black;
            bg.lNear.color2 = CRGB::Green;
            bg.lNear.seed = 18466;
            bg.lNear.scroll = 0;
        }
    };
};