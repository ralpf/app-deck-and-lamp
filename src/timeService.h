#pragma once

#include <Types.h>

class TimeService
{
    private:
    // all in ms
    ui32 nowMs;
    ui32 prevMs;
    ui32 dtMs;
    ui32 frame;

    public:
    ui32 now() const { return nowMs; }
    ui32 dt()  const { return dtMs; }

    public:  // call this every loop
    void update();
};


extern TimeService timeSrv;