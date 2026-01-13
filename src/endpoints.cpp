#include "endpoints.h"
#include "settings.h"
#include "asyncBackend.h"
#include "httpRequest.h"
#include "utils.h"


//..........................................................SETTINGS HANDLERS

void on_global(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("luma", i)) app.glob.luma = i;
}

//..............................................................LAMP HANDLERS


void on_lamp(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("mode", i)) app.lamp.mode = (Settings::Lamp::EMode)i;
    if (req.try_arg_i("luma", i)) app.lamp.luma = i;
}

void on_lamp_mood(HttpRequest& req)
{
    const ui8 SZ = 16;
    char buff[SZ];
    if (req.try_arg_s("col", buff, SZ)) app.lamp.mood.col32 = html_2_ui32(buff);
    app.lamp.mode = Settings::Lamp::EMode::Mood;
}

void on_lamp_hsv(HttpRequest& req)
{
    i32 i;
    if (req.try_arg_i("hue", i)) app.lamp.sliders.hue = i;
    if (req.try_arg_i("sat", i)) app.lamp.sliders.sat = i;
    app.lamp.mode = Settings::Lamp::EMode::Sliders;
}

void on_lamp_flicker(HttpRequest& req)
{
    i32 i; bool b;
    if (req.try_arg_b("on",    b)) app.lamp.flikOn = b;
    if (req.try_arg_i("hSpd",  i)) app.lamp.flikHue.spd = i;
    if (req.try_arg_i("hAmpl", i)) app.lamp.flikHue.ampl = i;
    if (req.try_arg_i("lSpd",  i)) app.lamp.flikLuma.spd = i;
    if (req.try_arg_i("lAmpl", i)) app.lamp.flikLuma.ampl = i;
}

//.............................................................HEADER

void endpoints_init()
{
    asyncBackend_register_endpoint("/esp/glob", on_global);
    asyncBackend_register_endpoint("/esp/lamp", on_lamp);
    asyncBackend_register_endpoint("/esp/lamp/mood", on_lamp_mood);
    asyncBackend_register_endpoint("/esp/lamp/hsv", on_lamp_hsv);
    asyncBackend_register_endpoint("/esp/lamp/flik", on_lamp_flicker);
}