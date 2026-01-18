#include "settings.h"

#include <jsonWriter.h>
#include <colorUtils.h>

#include <FastLED.h>



//..................................................................................STATIC-APP-STATE

Settings app;   // main app state data

//...........................................................................................TO-JSON

void Settings::emit_json(JsonWriter& json)
{
    // scopes present to support folding
    json.root(); {

        json.field_obj("glob"); {
            json.field_ui("luma", glob.luma);
            json.field_f("gamma", glob.gamma);
        json.end(); }

        json.field_obj("lamp"); {
            json.field_ui("luma", lamp.luma);

            json.field_obj("flick"); {
                json.field_b("isOn", lamp.flik.isOn);
                json.field_obj("hue"); {
                    json.field_f("spd", lamp.flik.hue.spd);
                    json.field_ui("ampl", lamp.flik.hue.ampl);
                json.end(); }
                json.field_obj("val"); {
                    json.field_f("spd", lamp.flik.val.spd);
                    json.field_ui("ampl", lamp.flik.val.ampl);
                json.end(); }
            json.end(); } // flick

            json.field_ui("mode", (ui32)lamp.mode);

            json.field_obj("mood"); {
                json.field_ui("col32", lamp.mood.rgb32);
            json.end(); }

            json.field_obj("sliders"); {
                json.field_ui("hue", lamp.sliders.hue);
                json.field_ui("sat", lamp.sliders.sat);
            json.end(); }

            json.field_obj("picker"); {
                json.field_ui("max", Pickers::max);
                json.field_arr("cols32"); {
                for (ui8 i = 0; i < Pickers::max; ++i)
                    if (lamp.pickers.cols32[i] != 0)
                        json.field_ui("", lamp.pickers.cols32[i]);
                json.end(); } // arr
            json.end(); } // picker
        json.end(); } // lamp

        json.field_obj("deck"); {
            json.field_ui("luma", deck.luma);

            json.field_obj("flick"); {
                json.field_b("isOn", deck.flik.isOn);
                json.field_obj("hue"); {
                    json.field_f ("spd", deck.flik.hue.spd);
                    json.field_ui("ampl", deck.flik.hue.ampl);
                json.end(); }
                json.field_obj("val"); {
                    json.field_f("spd", deck.flik.val.spd);
                    json.field_ui("ampl", deck.flik.val.ampl);
                json.end(); }
            json.end(); } // flick

            json.field_ui("mode", (ui32)deck.mode);

            json.field_obj("sliders"); {
                json.field_ui("hue", deck.sliders.hue);
                json.field_ui("sat", deck.sliders.sat);
            json.end(); }

            json.field_obj("picker"); {
                json.field_ui("max", Pickers::max);
                json.field_arr("cols32"); {
                for (ui8 i = 0; i < Pickers::max; ++i)
                    if (deck.pickers.cols32[i] != 0)
                        json.field_ui("", deck.pickers.cols32[i]);
                json.end(); } // arr
            json.end(); } // picker

            json.field_obj("palette"); {
                json.field_ui("idx", deck.palette.idx);
            json.end(); } // palette

        json.end(); } // deck

    json.end(); }
}

//.......................................................................................HEADER-IMPL

ui32 Settings::Pickers::to_hsv32()
{
    // just computes average color
    CRGB acum(cols32[0]);
    ui8  count = 0;

    for (ui8 i = 0; i < max; ++i)
    {
        if (cols32[i] == 0) continue;                           // skip 0
        nblend(acum, CRGB(cols32[i]), (ui8)(255 / ++count));    // mix proportionally
    }

    CHSV hsv = rgb_2_hsv_fast(acum);
    return hsv_2_ui32(hsv);
}


ui32 Settings::Sliders::to_hsv32()
{
    return hsv_2_ui32( CHSV(hue, sat, 255) );
}


ui32 Settings::Lamp::Mood::to_hsv32()
{
    CRGB rgb = ui32_2_rgb(rgb32);
    CHSV hsv = rgb_2_hsv_fast(rgb);
    return hsv_2_ui32(hsv);
}

