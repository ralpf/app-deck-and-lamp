#pragma once
#include "../../core/ientity.h"
#include "../../../ledsStrip.h"
#include "gameObject.h"
#include "settings.h"
// a simple scroller about asteroids and enemies
// use 50 fps for simplicty (fixed step movements)







namespace SpaceshipGame
{
    class Engine : public IEntity
    {

        //.....................................................................CTOR

        public:
        Engine(LedsStrip* pled) : pLeds(pled) { }

        //...................................................................FIELDS

        private:
        static constexpr ui8 MaxGObjs = 64;

        bool inited = false;

        LedsStrip* pLeds;                       // used led strip
        GObject*   gObjects[ MaxGObjs ];        // main objs array
        ui8        countObjs = 0;

        public:
        Settings s;        // user vars

        //.................................................................METHODS

        public:                      // mimic unity name for order convetion
        void Update() override;      // call per frame
        // for interface
        Settings* GetSettings() override { return &s; }
        void AppendToJson(JsonWriter& jw) override { /* do nothing yet */ }


        private:
        void Awake();                // call once
        void Draw();
    };
}






/*==================================================================================================
A simple scrolling spaceship game on a ~125 LED strip.The ship travels in one height layer, 
avoiding asteroids and possibly shooting enemies. We see a window of 5 layers around the ship: 
objects at our height have their normal size and color; objects above look reddish, objects 
below look bluish, and both become more transparent with distance. Starting 2 layers away, 
objects also shrink by one LED on each side for every additional layer.
==================================================================================================*/