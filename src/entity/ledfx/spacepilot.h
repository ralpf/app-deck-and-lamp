#pragma once
#include "../core/ientity.h"
#include "../../ledsStrip.h"
#include <FastLED.h>





// this is an example class, can be used as template
class SpacePilot : public IEntity
{
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++USER+SETTINGS

    struct Settings : public ISetting
    {
        //                          fields
        float speed = 1.0f;             // ship speed
        ui8   centerWidth = 3;          // center zone of the view, in leds
        //                          method
        void Reset() override { *this = Settings{}; }
    };

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    //..................................................................ENUM


    //................................................................STRUCT

    struct Data
    {
        LedsStrip *pL       = 0;        // pointer to leds
        Settings  *pS       = 0;        // pointer to settings
        double distance     = 0;        // traveled
        float  perspective  = 0;        // camera's
        float  turn         = 0;        // when steering, shifts the middle point left or right
        bool   debugFlag    = false;    // if true, will display a b/w mask for clouds
    };

    //................................................................STRUCT

    struct Octave                       // noise octave
    {
        ui32  seed = 0;
        float scale = 1.0f;             // noise frequency; larger values make smaller features
        float weight = 1.0f;            // contribution to the combined noise
        float detailsStart = 0.0f;      // normalized distance from center
        // cached stuff
        float scale256;
        ui32  travel;
        // method
        void  SetDistance(double dist);
        float Sample(float x) const;
    };

    //................................................................STRUCT

    struct CloudSide                    // left or right side
    {
        bool   isLeft;
        Octave octaveLarge;
        Octave octaveMedium;
        Octave octaveSmall;
        // method
        void Update(Data& data);
    };

    //................................................................STRUCT

    struct CloudLayer
    {
        // simple
        ui8  opacity    = 255;
        CRGB col1       = CRGB::Black;
        CRGB col2       = CRGB::White;
        // composite
        CloudSide left  { true  };
        CloudSide right { false };
        // method
        void Update(Data& data);
    };

    //................................................................STRUCT

    struct Background
    {
        CloudLayer far;
        // method
        void Update(Data& data);
    };

    //................................................................STRUCT

    struct Environment
    {
        Background bg;
        // method
        void Update(Data& data);
    };

    //................................................................STRUCT

    struct Cockpit
    {
        ui8 leftPanelN;     // number of leds for left panel
        ui8 rightPanleN;    // number of leds for right panel
        // method
        void Update(Data& data);
    };

    //................................................................STRUCT

    struct Camera
    {
        // entity
        Environment env;
        Cockpit     ckp;
        // method
        void Update(Data& data);
    };


    //.................................................................FIELD


    public:
    Settings S;

    public:         // CTOR
    SpacePilot(LedsStrip& target) : leds(target) { }

    public:
    void Update() override;
    virtual void AppendToJson(JsonWriter& jw) { /* do nothing */}
    Settings* GetSettings() override { return &S; }

    private:       // FIELDS
    LedsStrip& leds;
    Camera cam;
    Data   data;   // ship data
};