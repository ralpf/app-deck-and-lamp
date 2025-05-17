#pragma once

#include "addresleds.h"


template <ui8 PIN, ui16 COUNT>
class AddresLedsRnd : public AddresLeds<PIN, COUNT>
{

    public:  // inherited methods because of template class
    using AddresLeds<PIN, COUNT>::SetBrightness;
    using AddresLeds<PIN, COUNT>::SetPalette;
    using AddresLeds<PIN, COUNT>::SetPaletteFX;
    using AddresLeds<PIN, COUNT>::SetColor;
    using AddresLeds<PIN, COUNT>::SetGammaLutTable;



    private:
    ui16 randIdxs[COUNT];
    bool randEnabled;

    public:
    AddresLedsRnd() : AddresLeds<PIN, COUNT>()
    {
        // Fisher-Yates shuffle algorithm
        for (ui16 i = COUNT - 1; i > 0; --i)
        {
            ui16 j = random(i + 1);
            ui16 tmp    = randIdxs[i];
            randIdxs[i] = randIdxs[j];
            randIdxs[j] = tmp;
        }
    }


    public:         // METHODS
    void setRandEnabled(bool enabled) { randEnabled = enabled; }


    protected:
    void _SetRGB(ui16 idx, CRGB rgb) override
    {
        auto i = randEnabled ? randIdxs[idx] : idx;
        AddresLeds<PIN, COUNT>::_SetRGB(i, rgb);
    }
};