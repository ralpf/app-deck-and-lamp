#include "settings.h"
#include "jsonWriter.h"



Settings app;   // main app state data


void GlobSettings::AddJsonData(JsonWriter& json)
{
    json.field_obj("glob");
    {
        json.field_ui("luma", luma);
    }
    json.end();
}


void LampSettings::AddJsonData(JsonWriter& json)
{
    json.field_obj("lamp");
    {
        json.field_ui("luma", luma);
        json.field_b("flickOn", flikOn);
        json.field_obj("flikHue");
        {
            json.field_ui("ampl", flikHue.ampl);
            json.field_ui("spd", flikHue.spd);
        }
        json.end();
        json.field_obj("flikLuma");
        {
            json.field_ui("ampl", flikLuma.ampl);
            json.field_ui("spd", flikLuma.spd);
        }
        json.end();

        json.field_ui("mode", (ui32)mode);
        json.field_obj("mood");
        {
            json.field_ui("color32", mood.color32);
        }
        json.end();

        json.field_obj("sliders");
        {
            json.field_ui("hue", sliders.hue);
            json.field_ui("sat", sliders.sat);
        }
        json.end();

        json.field_obj("palette");
        {
            json.field_ui("max", Palette::max);
            json.field_arr("colors");
            for (ui8 i = 0; i < Palette::max; ++i)
                if (pallete.colors[i] != 0)
                    json.field_ui("", pallete.colors[i]);
            json.end(); // arr
        }
        json.end();
    }
    json.end(); // lamp
}


void DeckSettings::AddJsonData(JsonWriter& json)
{
    json.field_obj("deck");
    {
        json.field_ui("luma", luma);
    }
    json.end();
}


void Settings::AddJsonData(JsonWriter& json)
{
    json.begin_root();
    {
        glob.AddJsonData(json);
        lamp.AddJsonData(json);
        deck.AddJsonData(json);
    }
    json.end();
}