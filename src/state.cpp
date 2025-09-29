#include <ArduinoJson.h>
#include "state.h"

StateData app;



ui32 getRemainedStackSize()
{
    UBaseType_t remaining = uxTaskGetStackHighWaterMark2(NULL);         // null-> stack for current task
    return remaining * sizeof(StackType_t);
}


const char* app_fetchStateJson()
{
    StaticJsonDocument<4096> doc;

    //doc[]
    
}