#pragma once
#include "../../../ledsStrip.h"





class GObject   // any game object
{
    protected:
    float pos = 0;              // coresponds to led

    public:
    virtual void Awake() { }
    virtual void Update() { }
    virtual void Draw(LedsStrip* pLeds) = 0;
};