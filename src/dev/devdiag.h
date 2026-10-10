#pragma once
#include <Types.h>


class DevDiag
{
    public:                         // methods
    void PrintSerial();
    int  PrintBuffer(char* buff, ui16 sz);
};