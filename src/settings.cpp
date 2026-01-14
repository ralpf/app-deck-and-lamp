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

            json.field_obj("flick"); {
                json.field_b("isOn", lamp.flik.isOn);
                json.field_obj("hue"); {
                    json.field_ui("spd", lamp.flik.hue.spd);
                    json.field_ui("ampl", lamp.flik.hue.ampl);
                json.end(); }
                json.field_obj("sat"); {
                    json.field_ui("spd", lamp.flik.sat.spd);
                    json.field_ui("ampl", lamp.flik.sat.ampl);
                json.end(); }
            json.end(); } // flick

            json.field_ui("mode", (ui32)lamp.mode);

            json.field_obj("mood"); {
                json.field_ui("col32", lamp.mood.col32);
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
                    json.field_ui("spd", deck.flik.hue.spd);
                    json.field_ui("ampl", deck.flik.hue.ampl);
                json.end(); }
                json.field_obj("sat"); {
                    json.field_ui("spd", deck.flik.sat.spd);
                    json.field_ui("ampl", deck.flik.sat.ampl);
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
