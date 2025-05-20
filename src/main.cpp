
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "PrintHelper.h"

#include "httpserver.h"
#include "backend.h"
#include "state.h"
#include "utils.h"
#include "addresleds.h"
#include "addresledRnd.h"
#include "palettes.h"



#define VERSION "0.8.5"
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
    app.lamp.is_fliker = true;

    app.lamp.noiseBrt.ampl = 80;
    app.lamp.noiseBrt.timeScale = 1;

    app.lamp.noiseHue.ampl = 30;
    app.lamp.noiseHue.timeScale = 1;
    app.lamp.noiseHue.offset = 1000;
}


void InitPalettes()
{
    palette_init();
    app.console.palette.namesJson = palette_names_json();
}


void AnimateWiFiStartup(float seconds)
{
    static ui8 i = 1;
    ui8 k = ++i * 5;
    ledsConsole.SetColor(CRGB::Green);

    if (seconds > 0)
        ledsConsole.SetColor(CRGB::Gold, k, k + 5);
    else        // connected
        ledsConsole.SetColor(CRGB::Yellow, 0, k + 5);

    FastLED.show();
}

    
//.........................................................................................ESP
    
void setup()
{
    // Starts Serial
    SPrint("\n\n--------------------[[ ESP32 \"Blazar\" Lamp ]]--------------------");
    SPrint("\n\n----------------------[[ + TV Console supp ]]----------------------");
    SPrint("                                                   Version %s\n", VERSION);
    InitWiFiServer(201, AnimateWiFiStartup);      // ip adress 201 ; check with platformio.ini:upload_port
    InitHttpFrontend();
    InitBackend();
    InitState();
    InitPalettes();
    // init OTA
    ArduinoOTA.begin();
    SPrint("OK: OTA ready\n");
    // init leds
    ledsBlazar.SetColor(CRGB::Red);
    ledsConsole.SetColor(CRGB::Blue);
    FastLED.setBrightness(app.brightness);
    FastLED.show();
    SPrint("OK: Leds inited \tBlazar %i | TVConsole %i\n", LED_DBALL_COUNT, LED_TVCON_COUNT);
    // update LUT gamma correction
    RecaluculateGammaLUT();
}

void loop()
{
    delay(1);

    server.handleClient();
    ArduinoOTA.handle();
    
    FastLED.setBrightness(app.brightness);
    if (recalculateLUT) RecaluculateGammaLUT();

    Loop_BlazarLamp();
    Loop_TVConsole();

    FastLED.show();
}

//......................................................................LED

void Loop_BlazarLamp()
{
    ledsBlazar.OnUpdate(app.lamp.bright, 0, 0);  // force no skip and no anim idx
    CRGB col;
    col.setColorCode(app.lamp.color32);
    if (app.lamp.is_fliker)
        col = FlickerColor(col);

    ledsBlazar.SetColor(col);
    app.lamp.actualColor = rgb_2_UI32(col);
}


void Loop_TVConsole()
{
    ledsConsole.OnUpdate(app.console.bright, app.console.anim, app.animSkip);
    ledsConsole.setRandEnabled(app.console.palette.irand);
    CRGB rgb;

    switch (app.console.mode)
    {
        case TVConsole::Mode::HSV:
            hsv2rgb( CHSV(app.console.hsv.h, app.console.hsv.s, app.console.bright) , rgb);
            ledsConsole.SetColor(rgb);
            break;
        
        case TVConsole::Mode::MirrorLamp:
            rgb.setColorCode(app.lamp.actualColor);
            ledsConsole.SetColor(rgb);
            break;

        case TVConsole::Mode::Palette:
            // TODO: v~~~ optimize to not call every time
            auto pal = palette_from_idx(app.console.palette.idx);
            ledsConsole.SetPaletteFX(pal, app.console.blend);
            break;
    }
}


//.........................................................................FX

void RecaluculateGammaLUT()
{
    recalculateLUT = false;
    updateGammaLutTable256(gammaLUT, app.gamma);
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
    ui8 h = SampleNoise(app.lamp.noiseHue, hsv.h, true);
    ui8 v = SampleNoise(app.lamp.noiseBrt, hsv.v, false);
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

