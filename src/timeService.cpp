#include "timeService.h"

#include <Arduino.h>


TimeService timeSrv;


void TimeService::update()
{
    prevMs = nowMs;
    nowMs = millis();
    dtMs = nowMs - prevMs;
}