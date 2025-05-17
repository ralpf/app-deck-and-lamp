#include "palettes.h"
#include "palettes_data.h"


// NOTE: decided to ditch unordered map, for simplicity
// It required custom comparators and hashers because of c strings

CRGBPalette16 pals[PALETE_COUNT];
const char *   ids[PALETE_COUNT];



//=============================== METHODS =================================

void add_palette(const CRGBPalette16& pal, const char* name)
{
    static ui8 idx;
    if (idx >= PALETE_COUNT) return;
    pals[idx] = pal;
    ids[idx] = name;
    idx++;
}


void palette_init()
{
    // a bit slopy, but easy to reorder
    add_palette(palData_MultiPink,      "Multi Pink");
    add_palette(palData_RedLava,        "Red Lava");
    add_palette(palData_MarineBlue,     "Marine Blue");
    add_palette(palData_RedYellowWhite, "ReD-Yellow-White");
    add_palette(palData_Spectrum,       "Spectrum");
    add_palette(palData_Turq,           "Turq");
    add_palette(palData_Sunset_Real,    "Sunset Real");
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


const char* palette_names_json()
{
    String json = "[";
    for (ui8 i = 0; i < PALETE_COUNT; ++i)
    {
        json += "\"";
        json += palette_name_from_idx(i);
        json += "\"";
        if (i < PALETE_COUNT-1) json += ",";
    }
    json += "]";
    return strdup(json.c_str());
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