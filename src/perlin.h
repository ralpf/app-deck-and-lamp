#pragma once


#include <Types.h>
#include <FastLED.h>
#include "timeService.h"


// perlin noise wrappers based on FastLED inoise
// time is automated via time service

struct Perlin
{
    //.......................................................................................MEMBERS

    enum class EMode : ui8 { Ballance, Additive, Substractive };

    private:
    ui8   ampl = 0;         // amplitude
    float spd  = 0.0f;      // speed
    ui16  off  = 0;         // offset
    float f    = 0.0f;      // range [0,1]

    //..........................................................................................CTOR
    public:

    Perlin(float speed, ui8 amplitude, ui16 offset)
    {
        spd  = speed;
        ampl = amplitude;
        off  = offset;
    }

    Perlin(float speed, ui8 amplitude): Perlin(speed, amplitude, 0) { }

    //.......................................................................................METHODS
    public:

    i16 compute(EMode mode = EMode::Ballance)
    {
        _Update();
        if (ampl == 0 || spd == 0) return 0;
        return ampl * _Mult(mode);
    }

    //.......................................................................................METHODS

    private:
    void _Update()
    {
        ui32 distance = timeSrv.now() * spd;
        distance += off;
        f = inoise8(distance) / 255.0f;
    }


    float _Mult(EMode mode)
    {
        switch (mode)
        {
            case EMode::Ballance:       // range [-1, 1]
            return (f - 0.5f) * 2.0f;

            case EMode::Additive:       // range [0, 1]
            return f;

            case EMode::Substractive:   // range [-1, 0]
            return -f;

            default: return 0;
        }
    }

};