#include "endpoints.h"
#include "settings.h"
#include "asyncBackend.h"
#include "httpRequest.h"
#include "utils.h"


//..........................................................SETTINGS HANDLERS

void on_global(HttpRequest& req)
{
    i32 i; float f;
    if (req.try_arg_i("luma", i))  app.glob.luma = i;
    if (req.try_arg_f("gamma", f)) app.glob.gamma = f;
}

//..............................................................LAMP HANDLERS


void on_lamp(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("mode", i)) app.lamp.mode = (Settings::Lamp::EMode)i;
    if (req.try_arg_i("luma", i)) app.lamp.luma = i;
}

void on_lamp_flicker(HttpRequest& req)
{
    i32 i; bool b; float f;
    if (req.try_arg_b("on",    b)) app.lamp.flik.isOn = b;
    if (req.try_arg_f("hSpd",  f)) app.lamp.flik.hue.spd = f;
    if (req.try_arg_i("hAmpl", i)) app.lamp.flik.hue.ampl = i;
    if (req.try_arg_f("lSpd",  f)) app.lamp.flik.val.spd = f;
    if (req.try_arg_i("lAmpl", i)) app.lamp.flik.val.ampl = i;
}

void on_lamp_mood(HttpRequest& req)
{
    const ui8 SZ = 16;
    char buff[SZ];
    if (req.try_arg_s("col", buff, SZ)) app.lamp.mood.rgb32 = html_2_rgb_2_ui32(buff);
    app.lamp.mode = Settings::Lamp::EMode::Mood;
}

void on_lamp_sliders(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("hue", i)) app.lamp.sliders.hue = i;
    if (req.try_arg_i("sat", i)) app.lamp.sliders.sat = i;
    app.lamp.mode = Settings::Lamp::EMode::Sliders;
}

void on_lamp_pickers(HttpRequest& req)
{
    for (ui8 i = 0; i < Settings::Pickers::max; ++i) app.lamp.pickers.cols32[i] = 0;    // reset all
    // non-capturing lambda only
    req.try_many("col", [](ui8 i, const char* str) {
        if (i >= Settings::Pickers::max) return;
        app.lamp.pickers.cols32[i] = html_2_rgb_2_ui32(str);
    });
    app.lamp.mode = Settings::Lamp::EMode::Picker;
}

//..............................................................DECK HANDLERS

void on_deck(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("mode", i)) app.deck.mode = (Settings::Deck::EMode)i;
    if (req.try_arg_i("luma", i)) app.deck.luma = i;
}

void on_deck_flicker(HttpRequest& req)
{
    i32 i; bool b; float f;
    if (req.try_arg_b("on",    b)) app.deck.flik.isOn = b;
    if (req.try_arg_f("hSpd",  f)) app.deck.flik.hue.spd = f;
    if (req.try_arg_i("hAmpl", i)) app.deck.flik.hue.ampl = i;
    if (req.try_arg_f("lSpd",  f)) app.deck.flik.val.spd = f;
    if (req.try_arg_i("lAmpl", i)) app.deck.flik.val.ampl = i;
}

void on_deck_mirror(HttpRequest& req)
{
    app.deck.mode = Settings::Deck::EMode::MirrorLamp;
}

void on_deck_sliders(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("hue", i)) app.deck.sliders.hue = i;
    if (req.try_arg_i("sat", i)) app.deck.sliders.sat = i;
    app.deck.mode = Settings::Deck::EMode::Sliders;
}

void on_deck_pickers(HttpRequest& req)
{
    for (ui8 i = 0; i < Settings::Pickers::max; ++i) app.deck.pickers.cols32[i] = 0;    // reset all
    // non-capturing lambda only
    req.try_many("col", [](ui8 i, const char* str) {
        if (i >= Settings::Pickers::max) return;
        app.deck.pickers.cols32[i] = html_2_rgb_2_ui32(str);
    });
    app.deck.mode = Settings::Deck::EMode::Picker;
}

void on_deck_palette(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("pal", i)) app.deck.palette.idx = i;
    app.deck.palette.isStillBlending = true;          // force reload palette
    app.deck.mode = Settings::Deck::EMode::Palette;
}

//.....................................................................HEADER

void endpoints_init()
{
    // glob
    asyncBackend_register_endpoint("/esp/glob", on_global);
    // lamp
    asyncBackend_register_endpoint("/esp/lamp", on_lamp);
    asyncBackend_register_endpoint("/esp/lamp/flik", on_lamp_flicker);
    asyncBackend_register_endpoint("/esp/lamp/mood", on_lamp_mood);
    asyncBackend_register_endpoint("/esp/lamp/sliders", on_lamp_sliders);
    asyncBackend_register_endpoint("/esp/lamp/pickers", on_lamp_pickers);
    // deck
    asyncBackend_register_endpoint("/esp/deck", on_deck);
    asyncBackend_register_endpoint("/esp/deck/flik", on_deck_flicker);
    asyncBackend_register_endpoint("/esp/deck/mirror", on_deck_mirror);
    asyncBackend_register_endpoint("/esp/deck/sliders", on_deck_sliders);
    asyncBackend_register_endpoint("/esp/deck/pickers", on_deck_pickers);
    asyncBackend_register_endpoint("/esp/deck/palette", on_deck_palette);
}