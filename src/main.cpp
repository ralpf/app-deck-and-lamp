
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "PrintHelper.h"

#include "httpserver.h"
#include "backend.h"
#include "state.h"
#include "utils.h"

#define VERSION "0.8.1"
#define LED_PIN 14 // GPIO14 as your data pin
#define LED_TVCON_PIN 26
#define NUM_LEDS_DBALL 12
#define NUM_LEDS_TVCON 128
#define NUM_COL 6

CRGB leds[NUM_LEDS_DBALL];
CRGB leds_tvcon[NUM_LEDS_TVCON];


//............................................................................FORWARD DECLARATION

void Action_Error();
void Action_ConnectingToWiFi();
void Action_Lamp_Mood();
void Action_Lamp_Random();
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

//.........................................................................................ESP

void setup()
{
    // Starts Serial
    SPrint("\n\n--------------------[[ ESP32 \"Blazar\" Lamp ]]--------------------");
    SPrint("                                                   Version %s\n", VERSION);
    // Init Fast LED
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS_DBALL);
    FastLED.addLeds<WS2812, LED_TVCON_PIN, GRB>(leds_tvcon, NUM_LEDS_TVCON);
    fill_solid(leds, NUM_LEDS_DBALL, CRGB::Green);
    fill_solid(leds_tvcon, NUM_LEDS_TVCON, CRGB::Green);
    FastLED.setBrightness(app.brightness);
    FastLED.show();
    SPrint("OK: FastLED %i leds on pin %i\n", NUM_LEDS_DBALL, LED_PIN);
    // Init HTTP
    InitWiFiServer(201);      // ip adress 201
    InitHttpFrontend();
    InitBackend();
    InitState();
    // init OTA
    ArduinoOTA.begin();
    SPrint("OK: OTA ready\n");
}

void loop()
{
    delay(10);

    server.handleClient();
    ArduinoOTA.handle();
    FastLED.setBrightness(app.brightness);

    switch (app.curr_mode)
    {
        case 0:  Action_Lamp_Mood();             break;
        case 1:  Action_Lamp_Random();           break;
        default: Action_Error();
    }

    Action_TVConsole();
}

//......................................................................LED

void Action_Error()
{
    // flicker lights
    static ui32 time = millis();
    static bool flag;

    if (millis() - time > 500)
    {
        flag = !flag;
        time = millis();
    }

    CRGB c1 =  flag ? CRGB::Black : CRGB::Pink;
    CRGB c2 = !flag ? CRGB::Pink  : CRGB::Black;
    
    for (ui8 i = 0; i < 6; ++i)
        leds[i*2] = leds[i*2 + 1] = i % 2 ? c1 : c2;

    FastLED.show();
}

void Action_Lamp_Mood()
{
    CRGB col;
    col.setColorCode(app.lamp.color32);
    if (app.lamp.is_fliker)
        col = FlickerColor(col);

    fill_solid(leds, NUM_LEDS_DBALL, col);
    
    app.lamp.actualColor = rgb_2_UI32(col);
    FastLED.show();
}

void Action_Lamp_Random()
{
    static ui8 f = 0;
    if (++f % app.rand.skip != 0 ) return;  // skip every
    for (ui8 i = 0; i < NUM_LEDS_DBALL; ++i)
    {
        leds[i] = CRGB(random8(), random8(), random8());
    }
    FastLED.show();
}

void Action_TVConsole()
{
    if (!app.console.update && !app.console.mirrorLamp) return;
    
    CRGB rgb;
    auto appHSV = app.console.hsv;
    
    if (app.console.mirrorLamp)
    {
        rgb.setColorCode(app.lamp.actualColor);
        rgb.nscale8_video(app.console.hsv.v);
        fill_solid(leds_tvcon, NUM_LEDS_TVCON, rgb);
    }
    else
    {
        hsv2rgb( CHSV(appHSV.h, appHSV.s, appHSV.v) , rgb);
        fill_solid(leds_tvcon, NUM_LEDS_TVCON, rgb);
    }

    // CRGB rgb;
    // rgb.setColorCode(app.console.color32);
    // fill_solid(leds_tvcon, NUM_LEDS_TVCON, rgb);

    // if (app.console.bright != 0x00)
    //     for (ui8 i = 0; i < NUM_LEDS_TVCON; ++i)
    //         leds_tvcon[i].nscale8_video(app.console.bright);

    // app.console.bright = 0x00;

    app.console.update = 0x00;
    FastLED.show();
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

//     fadeToBlackBy(leds, NUM_LEDS_DBALL, runningFade);
//     FastLED.show();
//     CRGB bbb;
// }

// void ActionFixedColor()
// {
//     for (ui8 i = 0; i < NUM_LEDS_DBALL; ++i)
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
//         fadeToBlackBy(leds, NUM_LEDS_DBALL, runningFade);
//     FastLED.show();
// }

