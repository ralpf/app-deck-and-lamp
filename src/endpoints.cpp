#include "endpoints.h"
#include "settings.h"

#include <asyncBackend.h>
#include <httpRequest.h>
#include <colorUtils.h>
#include <uniPrinter.h>


constexpr ui8 kPrintDebug = 0;     // set to 1 for debug print. Shoud stript branch by compiler on 0

//.................................................................................SETTINGS HANDLERS

void on_global(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i; float f;
    if (req.try_arg_i("luma", i))  app.glob.luma = i;
    if (req.try_arg_f("gamma", f))
    {
        app.glob.gamma = f;
        app.glob.update_gamma = true;
    }
}

//.....................................................................................LAMP HANDLERS


void on_lamp(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i;
    if (req.try_arg_i("mode", i)) app.lamp.mode = (Settings::Lamp::EMode)i;
    if (req.try_arg_i("luma", i)) app.lamp.luma = i;
}

void on_lamp_flicker(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i; bool b; float f;
    if (req.try_arg_b("on",    b)) app.lamp.flik.isOn = b;
    if (req.try_arg_f("hSpd",  f)) app.lamp.flik.hue.spd = f;
    if (req.try_arg_i("hAmpl", i)) app.lamp.flik.hue.ampl = i;
    if (req.try_arg_f("lSpd",  f)) app.lamp.flik.val.spd = f;
    if (req.try_arg_i("lAmpl", i)) app.lamp.flik.val.ampl = i;
}

void on_lamp_mood(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    const ui8 SZ = 16;
    char buff[SZ];
    if (req.try_arg_s("col", buff, SZ)) app.lamp.mood.rgb32 = html_2_rgb_2_ui32(buff);
    app.lamp.mode = Settings::Lamp::EMode::Mood;
}

void on_lamp_hsv(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i;
    if (req.try_arg_i("hue", i)) app.lamp.sliders.hue = i;
    if (req.try_arg_i("sat", i)) app.lamp.sliders.sat = i;
    app.lamp.mode = Settings::Lamp::EMode::Sliders;
}


void on_lamp_picker(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    for (ui8 i = 0; i < Settings::Pickers::max; ++i) app.lamp.pickers.cols32[i] = 0;    // reset all
    // non-capturing lambda only
    req.try_many("col", [](ui8 i, const char* str) {
        if (i >= Settings::Pickers::max) return;
        app.lamp.pickers.cols32[i] = html_2_rgb_2_ui32(str);
    });
    app.lamp.mode = Settings::Lamp::EMode::Picker;
}

//.....................................................................................DECK HANDLERS

void on_deck(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i;
    if (req.try_arg_i("mode", i)) app.deck.mode = (Settings::Deck::EMode)i;
    if (req.try_arg_i("luma", i)) app.deck.luma = i;
}

void on_deck_flicker(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i; bool b; float f;
    if (req.try_arg_b("on",    b)) app.deck.flik.isOn = b;
    if (req.try_arg_f("hSpd",  f)) app.deck.flik.hue.spd = f;
    if (req.try_arg_i("hAmpl", i)) app.deck.flik.hue.ampl = i;
    if (req.try_arg_f("lSpd",  f)) app.deck.flik.val.spd = f;
    if (req.try_arg_i("lAmpl", i)) app.deck.flik.val.ampl = i;
}

void on_deck_mirror(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    app.deck.mode = Settings::Deck::EMode::MirrorLamp;
}

void on_deck_hsv(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i;
    if (req.try_arg_i("hue", i)) app.deck.sliders.hue = i;
    if (req.try_arg_i("sat", i)) app.deck.sliders.sat = i;
    app.deck.mode = Settings::Deck::EMode::Sliders;
}

void on_deck_picker(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    for (ui8 i = 0; i < Settings::Pickers::max; ++i) app.deck.pickers.cols32[i] = 0;    // reset all
    // non-capturing lambda only
    req.try_many("col", [](ui8 i, const char* str) {
        if (i >= Settings::Pickers::max) return;
        app.deck.pickers.cols32[i] = html_2_rgb_2_ui32(str);
    });
    app.deck.mode = Settings::Deck::EMode::Picker;
}

void on_deck_palette(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    i32 i;
    if (req.try_arg_i("pal", i)) app.deck.palette.idx = i;
    app.deck.palette.isStillBlending = true;          // force reload palette
    app.deck.mode = Settings::Deck::EMode::Palette;
}

//.....................................................................................DEBUG-HANDLER

void on_debug(HttpRequest& req, const char* ep)
{
    app.flagPrintJson = true;
}

void on_sync_state(HttpRequest& req, const char* ep)
{
    if (kPrintDebug) SPrint("ENDPOINT: %s", ep);
    JsonWriter json;
    app.emit_json(json);
    req.send(200, "application/json", json.get_cstring());
}

//............................................................................................HEADER


void endpoints_init()
{
    // NOTE: the order is important for matching!
    // glob
    asyncBackend_register_endpoint("/esp/glob", on_global);
    // lamp
    asyncBackend_register_endpoint("/esp/lamp/picker", on_lamp_picker);
    asyncBackend_register_endpoint("/esp/lamp/hsv", on_lamp_hsv);
    asyncBackend_register_endpoint("/esp/lamp/mood", on_lamp_mood);
    asyncBackend_register_endpoint("/esp/lamp/flick", on_lamp_flicker);
    asyncBackend_register_endpoint("/esp/lamp", on_lamp);
    // deck
    asyncBackend_register_endpoint("/esp/deck/flick", on_deck_flicker);
    asyncBackend_register_endpoint("/esp/deck/mirror", on_deck_mirror);
    asyncBackend_register_endpoint("/esp/deck/hsv", on_deck_hsv);
    asyncBackend_register_endpoint("/esp/deck/picker", on_deck_picker);
    asyncBackend_register_endpoint("/esp/deck/palette", on_deck_palette);
    asyncBackend_register_endpoint("/esp/deck", on_deck);
    // debug
    asyncBackend_register_endpoint("/esp/debug", on_debug);
    // sync
    asyncBackend_register_endpoint("/esp/sync/state", on_sync_state);

}
