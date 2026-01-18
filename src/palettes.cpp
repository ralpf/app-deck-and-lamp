#include "palettes.h"
#include "palettes_data.h"
#include <jsonWriter.h>


// NOTE: decided to ditch unordered map, for simplicity
// It required custom comparators and hashers because of c strings

//................................................................TYPE

struct PaletteEntry
{
    const TProgmemRGBGradientPalette_byte* pal;
    const char* name;
};

//..............................................................STATIC

static const PaletteEntry kAll[] = {
    { palData_MultiPink,      "Multi Pink" },
    { palData_RedLava,        "Red Lava" },
    { palData_MarineBlue,     "Marine Blue" },
    { palData_RedYellowWhite, "ReD-Yellow-White" },
    { palData_Spectrum,       "Spectrum" },
    { palData_Turq,           "Turq" },
    { palData_Sunset_Real,    "Sunset Real" },
};

static constexpr ui8 kCount = sizeof(kAll) / sizeof(kAll[0]);

//..............................................................HEADER

ui8 palette_count()
{
    return kCount;
}


i16 palette_idx(const char* name)
{
    for (ui8 i = 0; i < kCount; ++i)
        if (strcmp(name, kAll[i].name) == 0)
            return i;
    return -1;
}


const char* palette_name_from_idx(ui8 idx)
{
    return kAll[ idx % kCount ].name;
}


const char* palette_emit_names_json(JsonWriter& json)
{
    json.root(); {
        json.field_arr("pals");
        for (ui8 i = 0; i < kCount; ++i)
            json.field_s("", kAll[i].name);
        json.end();
    json.end(); }
    return json.get_cstring();
}


const CRGBPalette16 palette_from_idx(ui8 idx)
{
    return kAll[ idx % kCount ].pal;
}


const CRGBPalette16 palette_from_name(const char* name)
{
    static const CRGBPalette16 errorPal = palData_special_Eror;
    auto i = palette_idx(name);
    return i >= 0 ? kAll[i].pal : errorPal;
}