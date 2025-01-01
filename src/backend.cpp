#include "backend.h"

//........................................................................................

ui32 html_2_UI32(String htmlColor)
{
    ui32 err_col = 0xFF8080;

    auto len = htmlColor.length();
    if (len != 6 && len != 7) return err_col;
    if (len == 7 && htmlColor[0] != '#') return err_col;

    if (len == 7) htmlColor = htmlColor.substring(1, 7);

    ui8 r = strtol(htmlColor.substring(0, 2).c_str(), NULL, 16);
    ui8 g = strtol(htmlColor.substring(2, 4).c_str(), NULL, 16);
    ui8 b = strtol(htmlColor.substring(4, 6).c_str(), NULL, 16);

    return (r << 16) | (g << 8) | b;
}

//........................................................................................

void send_OK(String message) { server.send(200, "text/plain", message); }

void send_Fail(String message) { server.send(400, "text/plain", message); }

//........................................................................................

bool tryServerArgS(const String& arg, String& out)
{
    if (server.hasArg(arg))
    {
        out = server.arg(arg);
        return true;
    }
    return false;
}

bool tryServerArgI(const String& arg, int& out)
{
    if (server.hasArg(arg))
    {
        out = server.arg(arg).toInt();
        return true;
    }
    return false;
}

bool tryServerArgB(const String& arg, bool& out)
{
    if (server.hasArg(arg))
    {
        out = server.arg(arg).toInt() != 0;
        return true;
    }
    return false;
}

//..............................................................................MAIN HANDLES

void handleRoot() { server.send(200, "text/html", index_html); }

void handleCSS() { server.send(200, "text/css", style_css); }

void handleJS() { server.send(200, "application/javascript", script_js); }

//...............................................................................APP HANDLES

void handle_global()
{
    int bright = 0xFF;
    if (tryServerArgI("bright", bright))
        app.brightness = bright;

    send_OK("OK global");
}

void handle_mode_Mood()
{
    String html_color;
    if (tryServerArgS("color", html_color))
    {
        app.mode_Mood.color32 = html_2_UI32(html_color);
    }

    app.curr_mode = 0;
    send_OK("OK mode Mood");
}

void handle_mode_Random()
{
    int delay = 1000;
    if (tryServerArgI("delay", delay))
        app.mode_Rand.delay = delay;

    int fade = 1;
    if (tryServerArgI("fade", fade))
        app.mode_Rand.fade = fade;

    app.curr_mode = 1;
    send_OK("OK mode Random");
}

//...............................................................................APP HANDLES

void InitBackend()
{
    server.on("/", handleRoot);
    server.on("/style.css", handleCSS);
    server.on("/script.js", handleJS);

    server.on("/global", handle_global);
    server.on("/mood", handle_mode_Mood);
    server.on("/random", handle_mode_Random);
    //server.on("/fixed", handleModeFixed);
    //server.on("/run", handleModeRunning);

    // Start the server
    server.begin();
    Serial.println("OK: Web server started\n");
}





// void handleSetOne()
// {
//     if (server.hasArg("color") && server.hasArg("idx"))
//     {
//         ui8 i = server.arg("idx").toInt();
//         CRGB c = htmlToCRGB(server.arg("color"));
//         leds[i] = c;
//         FastLED.show();
//         server.send(200, "text/plain", "LED at " + String(i) + " set to " + server.arg("color"));
//         SPrint("set pixel %d to %s", i, server.arg("color"));
//     }
//     else
//     {
//         server.send(400, "text/plain", "Missing parameters. Use color=RRGGBB and idx=0-N");
//     }
// }


// void handleModeFixed()
// {
//     if (server.hasArg("color"))
//     {
//         active = htmlToCRGB(server.arg("color"));
//         server.send(200, "text/plain", "LED ALL set to " + server.arg("color"));
//     }
//     mode = 2;
// }



// void handleModeRunning()
// {
//     if (server.hasArg("delay"))
//     {
//         runningDelay = server.arg("delay").toInt();
//         server.send(200, "text/plain", "Running Mode set delay to " + String(runningDelay));
//     }
//     if (server.hasArg("fade"))
//     {
//         runningFade = server.arg("fade").toInt();
//         server.send(200, "text/plain", "Running Mode set fade to " + String(runningFade));
//     }
//     if (server.hasArg("color"))
//     {
//         active = htmlToCRGB(server.arg("color"));
//         server.send(200, "text/plain", "Color set to " + server.arg("color"));
//     }
//     server.send(200, "text/plain", "Mode set to Running");
//     mode = 0;
// }