#include "settings.h"
#include "jsonWriter.h"



Settings app;   // main app state data


void Settings::emit_json(JsonWriter& json)
{
    // scopes present to support folding
    json.begin_root(); {

        json.field_obj("glob"); {
            json.field_ui("luma", glob.luma);
        json.end(); }

        json.field_obj("lamp"); {
            json.field_ui("luma", lamp.luma);

            json.field_obj("flikHue"); {
                json.field_ui("ampl", lamp.flikHue.ampl);
                json.field_ui("spd", lamp.flikHue.spd);
            json.end(); }
            json.field_obj("flikLuma"); {
                json.field_ui("ampl", lamp.flikLuma.ampl);
                json.field_ui("spd", lamp.flikLuma.spd);
            json.end(); }
            json.field_b("flickOn", lamp.flikOn);

            json.field_ui("mode", (ui32)lamp.mode);

            json.field_obj("mood"); {
                json.field_ui("col32", lamp.mood.col32);
            json.end(); }

            json.field_obj("sliders"); {
                json.field_ui("hue", lamp.sliders.hue);
                json.field_ui("sat", lamp.sliders.sat);
            json.end(); }

            json.field_obj("palette"); {
                json.field_ui("max", Pickers::max);
                json.field_arr("cols32"); {
                for (ui8 i = 0; i < Pickers::max; ++i)
                    if (lamp.pickers.cols32[i] != 0)
                        json.field_ui("", lamp.pickers.cols32[i]);
                json.end(); } // arr
            json.end(); } // pallete
        json.end(); } // lamp

        json.field_obj("deck"); {
            json.field_ui("luma", deck.luma);
        json.end(); }

    json.end(); }
}
