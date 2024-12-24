#include <FastLED.h>
#include <WebServer.h>
#include "PrintHelper.h"
#include "Types.h"

const char *ssid = "StarNet - munteanu.v84";
const char *password = "48575443A95B41AA";

#define VERSION "0.5.1"
#define LED_PIN 14 // GPIO14 as your data pin
#define NUM_LEDS 12

CRGB leds[NUM_LEDS];
WebServer server(80); // HTTP server on port 80

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

// Handle root URL
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


void handleSetAll()
{
    if (server.hasArg("color"))
    {
        CRGB c = htmlToCRGB(server.arg("color"));
        for (ui8 i = 0; i < NUM_LEDS; ++i)
            leds[i] = c;
        FastLED.show();
        server.send(200, "text/plain", "LED ALL set to " + server.arg("color"));
        SPrint("set color %s", server.arg("color"));
    }
    else
    {
        server.send(400, "text/plain", "Missing parameter. Use color=RRGGBB");
    }
}

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
    server.on("/setOne", handleSetOne);
    server.on("/setAll", handleSetAll);

    // Start the server
    server.begin();
    Serial.println("Web server started.");

    for (int i = 0; i < NUM_LEDS; ++i)
        leds[i] = CRGB::Red;
    FastLED.show();
}

void loop()
{
    delay(10);
    server.handleClient();
}


void cycleLedsWithFade()
{
    static int idx = 0;
    static ui32 stime;

    if (millis() - stime > 1000)
    {
        stime = millis();
        leds[idx] = CRGB::Green;
        idx = ++idx % NUM_LEDS;
    }

    fadeToBlackBy(leds, NUM_LEDS, 1);
    FastLED.show();
}
