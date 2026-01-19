#include <uniPrinter.h>
#include <colorUtils.h>
#include <jsonWriter.h>
#include <asyncBackend.h>
#include <asyncWebsocket.h>
#include <timeService.h>

#include "leds.h"
#include "palettes.h"
#include "perlin.h"

#include "settings.h"
#include "endpoints.h"

#include <FastLED.h>
#include <ArduinoOTA.h>
#include "endpoints.h"




#define VERSION "0.9.0"
#define LED_LAMP_PIN   14          // GPIO14 data pin
#define LED_LAMP_COUNT 12          // led ring of 6x2
#define LED_DECK_PIN   26
#define LED_DECK_COUNT 128         // led strip



//........................................................................................ALLOC-LEDS

Leds<LED_DECK_PIN, LED_DECK_COUNT> ledsDeck;
Leds<LED_LAMP_PIN, LED_LAMP_COUNT> ledsLamp;

//........................................................................................GAMMA-CORR

ui8  gammaLUT[256];                // global gamma table
JsonWriter json;

//...............................................................................FORWARD-DECLARATION

void recaluculate_gamma_LUT();
void loop_lamp();
void loop_deck();
void loop_debug();
CHSV flicker_color_hsv(Settings::Flicker flik, CHSV hsv);

//...............................................................................................ESP

void setup()
{
    // fancy header, also this starts Serial
    SPrint("\n\n--------------------[[ ESP32 \"Blazar\" Lamp ]]--------------------");
    SPrint(  "\n---------------------[[ + TV Console supp ]]----------------------");
    SPrint("                                                     Version %s\n", VERSION);

    asyncBackend_init();
    endpoints_init();
    asyncBackend_start();

    ArduinoOTA.begin();     // other-the-air update
    SPrint("OK: OTA ready\n");

    // init leds
    ledsLamp.SetColor(CRGB::Red);
    ledsDeck.SetColor(CRGB::Blue);
    FastLED.setBrightness(app.glob.luma);
    FastLED.show();
    SPrint("OK: Leds inited Blazar (%u leds) Deck (%u leds)\n", LED_LAMP_COUNT, LED_DECK_COUNT);
    // update LUT gamma correction
    // recaluculate_gamma_LUT();
}

void loop()
{

    delay(1);
    ArduinoOTA.handle();
    timeSrv.update();
    websocket_maintain();

    FastLED.setBrightness(app.glob.luma);
    if (app.glob.update_gamma) recaluculate_gamma_LUT();

    loop_lamp();
    loop_deck();
    loop_debug();

    FastLED.show();
}

//.............................................................................................LOOPS

void loop_debug()
{
    if (app.flagPrintJson)
    {
        app.flagPrintJson = false;
        app.emit_json(json);
        SPrint("%s", json.get_cstring());
    }
}


void loop_lamp()
{
    ledsLamp.bright = app.lamp.luma;
    ledsLamp.anim.update();

    switch (app.lamp.mode)
    {
        case Settings::Lamp::EMode::Mood:       // 0
        app.lamp.hsv32_target = app.lamp.mood.to_hsv32();
        break;

        case Settings::Lamp::EMode::Sliders:    // 1
        app.lamp.hsv32_target = app.lamp.sliders.to_hsv32();
        break;

        case Settings::Lamp::EMode::Picker:     // 2
        app.lamp.hsv32_target = app.lamp.pickers.to_hsv32();
        break;
    }

    if (app.lamp.flik.isOn)
    {
        CHSV hsv = ui32_2_hsv(app.lamp.hsv32_target);
        hsv = flicker_color_hsv(app.lamp.flik, hsv);
        app.lamp.hsv32_actual = hsv_2_ui32(hsv);
    }
    else
    {
        app.lamp.hsv32_actual = app.lamp.hsv32_target;
    }

    ledsLamp.SetColor(ui32_2_hsv(app.lamp.hsv32_actual));
}


void loop_deck()
{
    ledsDeck.bright = app.deck.luma;
    ledsDeck.anim.update();
    ledsDeck.rand.isOn = app.deck.palette.isRandz;

    switch (app.deck.mode)
    {
        case Settings::Deck::EMode::MirrorLamp:     // 0
        // this contains lamp flicker, but more flicker can be added below
        app.deck.hsv32_target = app.lamp.hsv32_actual;
        break;

        case Settings::Deck::EMode::Sliders:        // 1
        app.deck.hsv32_target = app.deck.sliders.to_hsv32();
        break;

        case Settings::Deck::EMode::Picker:         // 2
        app.deck.hsv32_target = app.deck.pickers.to_hsv32();
        break;

        case Settings::Deck::EMode::Palette: {      // 3
        CRGBPalette16 pal = palette_from_idx(app.deck.palette.idx);
        // display with FX
        if (app.deck.palette.isStillBlending)
            app.deck.palette.isStillBlending = ledsDeck.SetPaletteFX(pal);
        return;     // for palette mode no flicker is available
        }

        default:
        SPrint("ERR: Unexpected Deck mode: %i", app.deck.mode);
        break;
    }

    if (app.deck.flik.isOn)
    {
        CHSV hsv = ui32_2_hsv(app.deck.hsv32_target);
        hsv = flicker_color_hsv(app.deck.flik, hsv);
        app.deck.hsv32_actual = hsv_2_ui32(hsv);
    }
    else
    {
        app.deck.hsv32_actual = app.deck.hsv32_target;
    }

    ledsDeck.SetColor(ui32_2_hsv(app.deck.hsv32_actual));
}

//.........................................................................FX

CHSV flicker_color_hsv(Settings::Flicker flik, CHSV hsv)
{
    // affect hue
    if (flik.hue.spd > 0 && flik.hue.ampl > 0)
    {
        Perlin noise(flik.hue.spd, flik.hue.ampl);
        hsv.hue += noise.compute(Perlin::EMode::Ballance);
    }
    // affect value
    if (flik.val.spd > 0 && flik.val.ampl > 0)
    {
        Perlin noise(flik.val.spd, flik.val.ampl);
        hsv.val += noise.compute(Perlin::EMode::Substractive);
    }
    return hsv;
}


void recaluculate_gamma_LUT()
{
    SPrint("[GAMMA] is set to: %f", app.glob.gamma);
    update_gammaLUT_table256(gammaLUT, app.glob.gamma);
    ledsDeck.SetGammaLutTable(gammaLUT);
    ledsLamp.SetGammaLutTable(gammaLUT);
    app.glob.update_gamma = false;
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

//     fadeToBlackBy(leds, LED_LAMP_COUNT, runningFade);
//     FastLED.show();
//     CRGB bbb;
// }

// void ActionFixedColor()
// {
//     for (ui8 i = 0; i < LED_LAMP_COUNT; ++i)
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
//         fadeToBlackBy(leds, LED_LAMP_COUNT, runningFade);
//     FastLED.show();
// }

