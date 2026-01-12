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
    }
    json.end();
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