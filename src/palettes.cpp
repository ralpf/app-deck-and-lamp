#include "palettes.h"

#define PALETE_COUNT 5

CRGBPalette16 all[PALETE_COUNT];
static bool inited;



//=============================== PALETTE DATA =================================

DEFINE_GRADIENT_PALETTE(palData_Spectrum) {
    0,   255, 0, 0,
    64,  0, 255, 0,
    128, 0, 0, 255,
    192, 255, 255, 0,
    255, 255, 0, 0
};


DEFINE_GRADIENT_PALETTE(palData_RedLava)
{
    0,   111, 0, 0,
    77,  255, 76, 0,
    191, 255, 250, 0,
    255, 11, 0, 0
};


DEFINE_GRADIENT_PALETTE(palData_MultiPink)
{
    0,   0, 0, 255,
    84,  255, 0 , 0,
    168, 150, 0, 205,
    255, 255, 164, 164
};



DEFINE_GRADIENT_PALETTE( palData_RedYellowWhite ) {
    0,   0,  0,  0,
    95, 255,  0,  0,
    191, 255,255,  0,
    255, 255,255,255
};


DEFINE_GRADIENT_PALETTE(palData_MarineBlue) {
    0,   255, 255, 255,
    8,   254, 255, 155,
    74,  38, 217, 255,
    143, 17,152, 150,
    206, 0, 40, 117,
    227, 0, 17, 97,
    255, 0, 4, 60
};


//=============================== METHODS =================================

void init()
{
    all[0] = palData_Spectrum;
    all[1] = palData_RedLava;
    all[2] = palData_MultiPink;
    all[3] = palData_RedYellowWhite;
    all[4] = palData_MarineBlue;
    inited = true;
}


CRGBPalette16 fetch_palette(ui8 idx)
{
    if (!inited) init();
    return all[ idx % PALETE_COUNT ];
}