
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "PrintHelper.h"

#include "state.h"
#include "utils.h"
#include "addresleds.h"
#include "addresledRnd.h"
#include "palettes.h"

#include "jsonWriter.h"
#include "settings.h"
#include "asyncBackend.h"
#include "endpoints.h"



#define VERSION "0.8.9"
#define LED_DBALL_PIN   14          // GPIO14 data pin
#define LED_DBALL_COUNT 12          // led ring of 6x2
#define LED_TVCON_PIN   26
#define LED_TVCON_COUNT 128         // led strip



//.........................................ALLOC LEDS
AddresLedsRnd<LED_TVCON_PIN, LED_TVCON_COUNT> ledsConsole;
AddresLeds   <LED_DBALL_PIN, LED_DBALL_COUNT> ledsBlazar;

bool recalculateLUT;                // for gamma correction
ui8  gammaLUT[256];                 // for gamma correction



//............................................................................FORWARD DECLARATION

void Loop_BlazarLamp();
void Loop_TVConsole();
void RecaluculateGammaLUT();

ui8 SampleNoise(const CmpNoise& noise, ui8 value);
CRGB FlickerColor(const CRGB col);


//.......................................................................................STATE

void InitState()
{
    oldapp.lamp.is_fliker = true;

    oldapp.lamp.noiseBrt.ampl = 80;
    oldapp.lamp.noiseBrt.timeScale = 1;

    oldapp.lamp.noiseHue.ampl = 30;
    oldapp.lamp.noiseHue.timeScale = 1;
    oldapp.lamp.noiseHue.offset = 1000;
}


void InitPalettes()
{
    palette_init();
    oldapp.console.palette.namesJson = palette_names_json();
}


//.........................................................................................ESP

void setup()
{
    // Starts Serial
    SPrint("\n\n--------------------[[ ESP32 \"Blazar\" Lamp ]]--------------------");
    SPrint(  "\n---------------------[[ + TV Console supp ]]----------------------");
    SPrint("                                                     Version %s\n", VERSION);

    SPrint("!!! testing json writer...");
    JsonWriter json;

    Settings set;
    set.emit_json(json);

    SPrint("size is %d", json.get_size());
    SPrint("data:\n%s", json.get_cstring());


    asyncBackend_init();
    endpoints_init();
    asyncBackend_start();


    //xxxxxxxxxxxxxxxxxxxxxxxxxx RET
    return;
    //InitWiFiServer(201, AnimateWiFiStartup);      // ip adress 201 ; check with platformio.ini:upload_port
    //InitHttpFrontend();
    //InitBackend();
    InitState();
    InitPalettes();
    // init OTA
    ArduinoOTA.begin();
    SPrint("OK: OTA ready\n");
    // init leds
    ledsBlazar.SetColor(CRGB::Red);
    ledsConsole.SetColor(CRGB::Blue);
    FastLED.setBrightness(oldapp.brightness);
    FastLED.show();
    SPrint("OK: Leds inited \tBlazar %i | TVConsole %i\n", LED_DBALL_COUNT, LED_TVCON_COUNT);
    // update LUT gamma correction
    RecaluculateGammaLUT();
}

void loop()
{
    delay(1);
    SPrint('.');
    //SPrint("loop ms %d", millis());

    delay(1000);
    //xxxxxxxxxxxxxxxxxxxxxxxxxx RET
    return;

    //server.handleClient();
    ArduinoOTA.handle();
    
    FastLED.setBrightness(oldapp.brightness);
    if (recalculateLUT) RecaluculateGammaLUT();

    Loop_BlazarLamp();
    Loop_TVConsole();

    FastLED.show();
}

//......................................................................LED

void Loop_BlazarLamp()
{
    ledsBlazar.OnUpdate(oldapp.lamp.bright, 0, 0);  // force no skip and no anim idx
    CRGB col;
    col.setColorCode(oldapp.lamp.color32);
    if (oldapp.lamp.is_fliker)
        col = FlickerColor(col);

    ledsBlazar.SetColor(col);
    oldapp.lamp.actualColor = rgb_2_ui32(col);
}


void Loop_TVConsole()
{
    ledsConsole.OnUpdate(oldapp.console.bright, oldapp.console.anim, oldapp.animSkip);
    ledsConsole.setRandEnabled(oldapp.console.palette.irand);
    CRGB rgb;

    switch (oldapp.console.mode)
    {
        case TVConsole::Mode::HSV:
            hsv2rgb( CHSV(oldapp.console.hsv.h, oldapp.console.hsv.s, oldapp.console.bright) , rgb);
            ledsConsole.SetColor(rgb);
            break;
        
        case TVConsole::Mode::MirrorLamp:
            rgb.setColorCode(oldapp.lamp.actualColor);
            ledsConsole.SetColor(rgb);
            break;

        case TVConsole::Mode::Palette:
            // TODO: v~~~ optimize to not call every time
            auto pal = palette_from_idx(oldapp.console.palette.idx);
            ledsConsole.SetPaletteFX(pal, oldapp.console.blend);
            break;
    }
}


//.........................................................................FX

void RecaluculateGammaLUT()
{
    recalculateLUT = false;
    updateGammaLutTable256(gammaLUT, oldapp.gamma);
    ledsConsole.SetGammaLutTable(gammaLUT);
    ledsBlazar.SetGammaLutTable(gammaLUT);
}

ui8 SampleNoise(const CmpNoise& noise, ui8 value, bool isWrap)
{
    if (noise.ampl <= 0) return value;
    float f = inoise8((millis() * noise.timeScale) + noise.offset) / 255.0;   // [ 0, 1]
    f = (f - 0.5) * 2.0;                                                      // [-1, 1]
    int result = value + noise.ampl * f;
    return isWrap ? (ui8)result : constrain(result, 0, 255);
}

CRGB FlickerColor(CRGB rgb)
{
    CHSV hsv;
    rgb2hsv(rgb, hsv);
    ui8 h = SampleNoise(oldapp.lamp.noiseHue, hsv.h, true);
    ui8 v = SampleNoise(oldapp.lamp.noiseBrt, hsv.v, false);
    hsv2rgb(CHSV(h, hsv.s, v), rgb);
    return rgb;
}














// void ActionRunningDotFade()
// {
//     static int idx = 0;
//     static ui32 stime;

//     if (millis() - stime > runningDelay)
//     {
//         stime = millis();
//         leds[idx * 2] = leds[idx * 2 + 1] = active;
//         idx = ++idx % NUM_COL;
//     }

//     fadeToBlackBy(leds, LED_DBALL_COUNT, runningFade);
//     FastLED.show();
//     CRGB bbb;
// }

// void ActionFixedColor()
// {
//     for (ui8 i = 0; i < LED_DBALL_COUNT; ++i)
//         leds[i] = active;
//     FastLED.show();
// }

// void ActionRandomColor()
// {
//     static ui32 stime;

//     if (millis() - stime > randomDelay)
//     {
//         stime = millis();
//         int idx = random(0, 6);
//         leds[idx * 2] = CHSV(random8(), 255, 255);
//         leds[idx * 2 + 1] = CHSV(random8(), 255, 255);
//     }

//     if (randomFade == 1)
//         fadeToBlackBy(leds, LED_DBALL_COUNT, runningFade);
//     FastLED.show();
// }

