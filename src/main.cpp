#include <FastLED.h>
#include <WebServer.h>
#include "PrintHelper.h"
#include "Types.h"

const char *ssid = "StarNet - munteanu.v84";
const char *password = "48575443A95B41AA";

#define VERSION "0.5.1"
#define LED_PIN 14 // GPIO14 as your data pin
#define NUM_LEDS 12
#define NUM_COL 6


CRGB active = CRGB::Azure;
ui8 mode;

ui32 runningDelay = 1000;
ui8 runningFade = 1;

ui32 randomDelay = 400;
ui8 randomFade = 1;

CRGB leds[NUM_LEDS];
WebServer server(80); // HTTP server on port 80

//............................................................................FORWARD DECLARATION

void ActionRunningDotFade();
void ActionRandomColor();
void ActionFixedColor();
void ActionError();

//...............................................................................................

// Convert HTML color string (#RRGGBB) to CRGB
CRGB htmlToCRGB(String htmlColor)
{
    if (htmlColor.length() != 6)
    {
        SPrint("Err: can't parse html color '%s'", htmlColor);
        return CRGB::Red;
    }

    int r = strtol(htmlColor.substring(0, 2).c_str(), NULL, 16);
    int g = strtol(htmlColor.substring(2, 4).c_str(), NULL, 16);
    int b = strtol(htmlColor.substring(4, 6).c_str(), NULL, 16);

    return CRGB(r, g, b);
}

//..................................................................................HANDLES

void handleRoot()
{
    String html = "<html><body>";
    html += "<h1>ESP32 Web Server</h1>";
    html += "<p>Lev Discobal LEDs v" + String(VERSION) + "</p>";
    html += "<p>Baud rate = " + String(PRINTER_BAUD) + "</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
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
        active = htmlToCRGB( server.arg("color") );
        server.send(200, "text/plain", "Color set to " + server.arg("color"));
    }
    server.send(200, "text/plain", "Mode set to Running");
    mode = 0;
}

//.........................................................................................ESP

void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    SPrint("\nStarting Levitating Disco Ball Leds\nVersion %s\n", VERSION);

    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Set up web server routes
    server.on("/", handleRoot);
    server.on("/global", handleGlobal);
    server.on("/fixed", handleModeFixed);
    server.on("/run", handleModeRunning);
    server.on("/random", handleModeRandom);

    // Start the server
    server.begin();
    Serial.println("Web server started.");

    mode = 0;
    active = CRGB::MediumSeaGreen;
}

void loop()
{
    delay(10);
    server.handleClient();
    switch (mode)
    {
        case 0:  ActionRunningDotFade();    break;
        case 1:  ActionRandomColor();       break;
        case 2:  ActionFixedColor();        break;
        default: ActionError();
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
    for (ui8 i = 0; i < 6; ++i)
        leds[i + (i % 2 ? 1 : 0)] = CRGB::Pink;
    FastLED.show();
}