#include <FS.h>

#include <FastLED.h>
#include "PrintHelper.h"
#include "Types.h"
#include "httpserver.h"

#define VERSION "0.6.1"
#define LED_PIN 14 // GPIO14 as your data pin
#define NUM_LEDS 12
#define NUM_COL 6

String index_html;
String style_css;
String script_js;

CRGB active = CRGB::Azure;
ui8 mode;

ui32 runningDelay = 1000;
ui8 runningFade = 1;

ui32 randomDelay = 400;
ui8 randomFade = 1;

CRGB leds[NUM_LEDS];

//............................................................................FORWARD DECLARATION

void ActionRunningDotFade();
void ActionRandomColor();
void ActionFixedColor();
void ActionError();

//...............................................................................................

// Convert HTML color string (#RRGGBB) to CRGB
CRGB htmlToCRGB(String htmlColor)
{
    int len = htmlColor.length();
    if (len != 6 && len != 7)
    {
        SPrint("Err: can't parse html color '%s'", htmlColor);
        return CRGB::Red;
    }
    if (len == 7) htmlColor = htmlColor.substring(1, 7);

    int r = strtol(htmlColor.substring(0, 2).c_str(), NULL, 16);
    int g = strtol(htmlColor.substring(2, 4).c_str(), NULL, 16);
    int b = strtol(htmlColor.substring(4, 6).c_str(), NULL, 16);

    return CRGB(r, g, b);
}

//..................................................................................HELPERS

void send_OK(String message) { server.send(200, "text/plain", message); }

void send_Fail(String message) { server.send(400, "text/plain", message); }

//..................................................................................HANDLES

void handleRoot() { server.send(200, "text/html", index_html); }

void handleCSS() { server.send(200, "text/css", style_css); }

void handleJS() { server.send(200, "application/javascript", script_js); }

void handleMood()
{
    if (server.hasArg("color") == false)
    {
        send_Fail("ERR: no expected param 'color'");
        return;
    }

    active = htmlToCRGB(server.arg("color"));
    mode = 2;
    send_OK("OK");
}

void handleSetOne()
{
    if (server.hasArg("color") && server.hasArg("idx"))
    {
        ui8 i = server.arg("idx").toInt();
        CRGB c = htmlToCRGB(server.arg("color"));
        leds[i] = c;
        FastLED.show();
        server.send(200, "text/plain", "LED at " + String(i) + " set to " + server.arg("color"));
        SPrint("set pixel %d to %s", i, server.arg("color"));
    }
    else
    {
        server.send(400, "text/plain", "Missing parameters. Use color=RRGGBB and idx=0-N");
    }
}

void handleGlobal()
{
    if (server.hasArg("bright"))
    {
        ui8 br = server.arg("bright").toInt();
        FastLED.setBrightness(br);
        server.send(200, "text/plain", "Global Brighntess set to " + String(br));
    }
}

void handleModeFixed()
{
    if (server.hasArg("color"))
    {
        active = htmlToCRGB(server.arg("color"));
        server.send(200, "text/plain", "LED ALL set to " + server.arg("color"));
    }
    mode = 2;
}

void handleModeRandom()
{
    if (server.hasArg("delay"))
    {
        randomDelay = server.arg("delay").toInt();
        server.send(200, "text/plain", "Random Mode set delay to " + String(randomDelay));
    }
    if (server.hasArg("fade"))
    {
        randomFade = server.arg("fade").toInt();
        server.send(200, "text/plain", "Random Mode set fade to " + String(randomFade));
    }
    server.send(200, "text/plain", "Mode set to Random");
    mode = 1;
}

void handleModeRunning()
{
    if (server.hasArg("delay"))
    {
        runningDelay = server.arg("delay").toInt();
        server.send(200, "text/plain", "Running Mode set delay to " + String(runningDelay));
    }
    if (server.hasArg("fade"))
    {
        runningFade = server.arg("fade").toInt();
        server.send(200, "text/plain", "Running Mode set fade to " + String(runningFade));
    }
    if (server.hasArg("color"))
    {
        active = htmlToCRGB(server.arg("color"));
        server.send(200, "text/plain", "Color set to " + server.arg("color"));
    }
    server.send(200, "text/plain", "Mode set to Running");
    mode = 0;
}

//........................................................................................INIT





void initWebServer()
{
    server.on("/", handleRoot);
    server.on("/style.css", handleCSS);
    server.on("/script.js", handleJS);

    server.on("/global", handleGlobal);
    server.on("/fixed", handleModeFixed);
    server.on("/run", handleModeRunning);
    server.on("/random", handleModeRandom);
    server.on("/mood", handleMood);

    // Start the server
    server.begin();
    Serial.println("Web server started OK");
}

//.........................................................................................ESP

void setup()
{
    // Fast LED
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    SPrint("\nStarting Levitating Disco Ball Leds\nVersion %s\n", VERSION);

    InitWiFiServer(201);      // ip adress 201
    InitHttpFrontend();
    initWebServer();

    mode = 0;
    active = CRGB::MediumSeaGreen;
}

void loop()
{
    delay(10);
    server.handleClient();
    switch (mode)
    {
    case 0:
        ActionRunningDotFade();
        break;
    case 1:
        ActionRandomColor();
        break;
    case 2:
        ActionFixedColor();
        break;
    default:
        ActionError();
    }
}

//......................................................................LED

void ActionRunningDotFade()
{
    static int idx = 0;
    static ui32 stime;

    if (millis() - stime > runningDelay)
    {
        stime = millis();
        leds[idx * 2] = leds[idx * 2 + 1] = active;
        idx = ++idx % NUM_COL;
    }

    fadeToBlackBy(leds, NUM_LEDS, runningFade);
    FastLED.show();
}

void ActionFixedColor()
{
    for (ui8 i = 0; i < NUM_LEDS; ++i)
        leds[i] = active;
    FastLED.show();
}

void ActionRandomColor()
{
    static ui32 stime;

    if (millis() - stime > randomDelay)
    {
        stime = millis();
        int idx = random(0, 6);
        leds[idx * 2] = CHSV(random8(), 255, 255);
        leds[idx * 2 + 1] = CHSV(random8(), 255, 255);
    }

    if (randomFade == 1)
        fadeToBlackBy(leds, NUM_LEDS, runningFade);
    FastLED.show();
}

void ActionError()
{
    // ??? lit them in a pattern
    for (ui8 i = 0; i < 6; ++i)
        leds[i + (i % 2 ? 1 : 0)] = CRGB::Pink;
    FastLED.show();
}