#include "palettes.h"
#include "palettes_data.h"


// NOTE: decided to ditch unordered map, for simplicity
// It required custom comparators and hashers because of c strings

CRGBPalette16 pals[PALETE_COUNT];
const char *   ids[PALETE_COUNT];



//=============================== METHODS =================================

void palette_init()
{
    pals[0] = palData_MultiPink;            ids[0] = "Multi Pink";
    pals[1] = palData_RedLava;              ids[1] = "Red Lava";
    pals[2] = palData_MarineBlue;           ids[2] = "Marine Blue";
    pals[3] = palData_RedYellowWhite;       ids[3] = "ReD-Yellow-White";
    pals[4] = palData_Spectrum;             ids[4] = "Spectrum";
}


ui8 palette_count()
{
    return PALETE_COUNT;
}


i16 palette_idx(const char* name)
{
    for (ui8 i = 0; i < PALETE_COUNT; ++i)
        if (strcmp(name, ids[i]) == 0)
            return i;
    return -1;
}


const char* palette_name_from_idx(ui8 idx)
{
    return ids[ idx % PALETE_COUNT ];
}


const CRGBPalette16& palette_from_idx(ui8 idx)
{
    return pals[ idx % PALETE_COUNT ];
}


const CRGBPalette16& palette_from_name(const char* name)
{
    static const CRGBPalette16 errorPal = palData_special_Eror;
    auto i = palette_idx(name);
    return i >= 0 ? pals[i] : errorPal;
}