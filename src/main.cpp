
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "PrintHelper.h"

#include "httpserver.h"
#include "backend.h"
#include "state.h"
#include "utils.h"
#include "addrleds.h"
#include "palettes.h"



#define VERSION "0.8.4"
#define LED_DBALL_PIN   14          // GPIO14 data pin
#define LED_DBALL_COUNT 12          // led ring of 6x2
#define LED_TVCON_PIN   26
#define LED_TVCON_COUNT 128         // led strip



//.........................................ALLOC LEDS
AddresLeds<LED_TVCON_PIN, LED_TVCON_COUNT> ledsConsole;
AddresLeds<LED_DBALL_PIN, LED_DBALL_COUNT> ledsBlazar;

bool updateLeds;


//............................................................................FORWARD DECLARATION

void Action_BlazarLamp();
void Action_TVConsole();

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
    SPrint("                                                   Version %s\n", VERSION);
    InitWiFiServer(201, AnimateWiFiStartup);      // ip adress 201 ; check with platformio.ini:upload_port
    InitHttpFrontend();
    InitBackend();
    InitState();
    // init OTA
    ArduinoOTA.begin();
    SPrint("OK: OTA ready\n");
    // init leds
    ledsBlazar.SetColor(CRGB::Red);
    ledsConsole.SetColor(CRGB::Blue);
    FastLED.setBrightness(app.brightness);
    FastLED.show();
    SPrint("OK: Leds inited \tBlazar %i | TVConsole %i\n", LED_DBALL_COUNT, LED_TVCON_COUNT);
}

void loop()
{
    delay(10);

    server.handleClient();
    ArduinoOTA.handle();
    FastLED.setBrightness(app.brightness);

    Action_BlazarLamp();
    Action_TVConsole();

    if (updateLeds) FastLED.show();
    updateLeds = false;
}

//......................................................................LED

void Action_BlazarLamp()
{
    switch (app.curr_mode)
    {
        case 0:
            CRGB col;
            col.setColorCode(app.lamp.color32);
            if (app.lamp.is_fliker)
                col = FlickerColor(col);

            ledsBlazar.SetColor(col);
            app.lamp.actualColor = rgb_2_UI32(col);
            updateLeds = true;
            break;

        case 1:
            break;

        default:
            break;
    }
}


void Action_TVConsole()
{
    CRGB rgb;
    auto appHSV = app.console.hsv;
    
    switch (app.console.mode)
    {
        case TVConsole::Mode::HSV:
            if (!app.console.update) break;
            hsv2rgb( CHSV(appHSV.h, appHSV.s, appHSV.v) , rgb);
            ledsConsole.SetColor(rgb);
            break;
        
        case TVConsole::Mode::MirrorLamp:
            rgb.setColorCode(app.lamp.actualColor);
            rgb.nscale8_video(app.console.hsv.v);
            ledsConsole.SetColor(rgb);
            break;

        case TVConsole::Mode::Palette:
            if (app.console.update) ledsConsole.SetPalette( fetch_palette(app.console.paletteIdx) );
            break;
    }

    updateLeds = true;
    app.console.update = false;
}


//.........................................................................FX

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

