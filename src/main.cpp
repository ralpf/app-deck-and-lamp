
#include <FastLED.h>
#include <ArduinoOTA.h>
#include "PrintHelper.h"

#include "httpserver.h"
#include "backend.h"
#include "state.h"

#define VERSION "0.7.3"
#define LED_PIN 14 // GPIO14 as your data pin
#define NUM_LEDS 12
#define NUM_COL 6

CRGB leds[NUM_LEDS];

//............................................................................FORWARD DECLARATION

void Action_Error();
void Action_ConnectingToWiFi();
void Action_Mode_Mood();
void Action_Mode_Random();

ui8 SampleNoise(const CmpNoise& noise, ui8 value);
CRGB FlickerColor(const CRGB col);


//.......................................................................................STATE

void InitState()
{
    app.mode_Mood.is_fliker = true;

    app.mode_Mood.noiseBrt.ampl = 80;
    app.mode_Mood.noiseBrt.timeScale = 1;

    app.mode_Mood.noiseHue.ampl = 30;
    app.mode_Mood.noiseHue.timeScale = 1;
    app.mode_Mood.noiseHue.offset = 1000;
}

//.........................................................................................ESP

void setup()
{
    // Starts Serial
    SPrint("\n\n--------------------[[ ESP32 \"Blazar\" Lamp ]]--------------------");
    SPrint("                                                   Version %s\n", VERSION);
    // Init Fast LED
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.setBrightness(app.brightness);
    FastLED.show();
    SPrint("OK: FastLED %i leds on pin %i\n", NUM_LEDS, LED_PIN);
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
        case 0:  Action_Mode_Mood();             break;
        case 1:  Action_Mode_Random();           break;
        default: Action_Error();
    }
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

void Action_Mode_Mood()
{
    CRGB col;
    col.setColorCode(app.mode_Mood.color32);
    if (app.mode_Mood.is_fliker) col = FlickerColor(col);
    fill_solid(leds, NUM_LEDS, col);
    FastLED.show();
}

void Action_Mode_Random()
{
    
}

//.........................................................................FX

ui8 SampleNoise(const CmpNoise& noise, ui8 value, bool isWrap)
{
    float f = inoise8((millis() * noise.timeScale) + noise.offset) / 255.0;   // [ 0, 1]
    f = (f - 0.5) * 2.0;                                                      // [-1, 1]
    int result = value + noise.ampl * f;
    return isWrap ? (ui8)result : constrain(result, 0, 255);
}

CRGB FlickerColor(const CRGB col)
{
    static ui32 ii;
    CHSV hsv = rgb2hsv_approximate(col);
    ui8 h = SampleNoise(app.mode_Mood.noiseHue, hsv.h, true);
    ui8 v = SampleNoise(app.mode_Mood.noiseBrt, hsv.v, false);
    return CHSV(h, hsv.s, v);
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

//     fadeToBlackBy(leds, NUM_LEDS, runningFade);
//     FastLED.show();
//     CRGB bbb;
// }

// void ActionFixedColor()
// {
//     for (ui8 i = 0; i < NUM_LEDS; ++i)
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
//         fadeToBlackBy(leds, NUM_LEDS, runningFade);
//     FastLED.show();
// }

