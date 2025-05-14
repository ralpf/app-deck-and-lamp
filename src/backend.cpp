#include "backend.h"
#include "utils.h"

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
    int i = 0xFF;
    if (tryServerArgI("bright", i))
        app.brightness = i;

    if (tryServerArgI("gamma", i))
        app.gamma = i / 10.0;

    app.globalChanged = true;
    send_OK("OK globals " + millis());  // just to see updates
}

void handle_mode_Mood()
{
    String html_color;
    if (tryServerArgS("color", html_color))
        app.lamp.color32 = html_2_UI32(html_color);

    int i = 0;
    if (tryServerArgI("is_flicker", i))
        app.lamp.is_fliker = i != 0;

    if (tryServerArgI("hue_ampl", i))
        app.lamp.noiseHue.ampl = i;

    if (tryServerArgI("hue_ts", i))
        app.lamp.noiseHue.timeScale = i / 1000.0;

    if (tryServerArgI("brt_ampl", i))
        app.lamp.noiseBrt.ampl = i;

    if (tryServerArgI("brt_ts", i))
        app.lamp.noiseBrt.timeScale = i / 1000.0;

    app.curr_mode = 0;
    send_OK("OK mode Mood");
}

void handle_mode_Random()
{
    int skip = 1000;
    if (tryServerArgI("skip", skip))
        app.rand.skip = skip;

    int vari = 1;
    if (tryServerArgI("vari", vari))
        app.rand.vari = vari;

    app.curr_mode = 1;
    send_OK("OK mode Random");
}

void handle_test()
{
    String s = "entering testing mode | prev mode was ";
    send_OK(s + app.curr_mode);
    app.curr_mode = 255;
}

void handle_tvcon()
{
    String html_color;
    int i = 0;
    bool b = false;

    if (tryServerArgI("hue", i))
        app.console.hsv.h = constrain(i, 0, 255);
    if (tryServerArgI("sat", i))
        app.console.hsv.s = constrain(i, 0, 255);
    if (tryServerArgI("val", i))
        app.console.hsv.v = constrain(i, 0, 255);

    if (tryServerArgB("mirror", b))
        app.console.mode = b ? TVConsole::Mode::MirrorLamp : TVConsole::Mode::HSV;

    app.console.update = true;
    send_OK("TV console HSV -> color: " + html_color + " mode: " + app.console.mode);
}

void handle_tvcon_palette()
{
    int i = 0;
    bool b = false;
    
    if (tryServerArgI("idx", i))         // palette index
        app.console.paletteIdx = i;
    
    if (tryServerArgB("rand", b))        // palette randomization
        app.console.paletteRand = true;

    app.console.update = true;
    app.console.mode = TVConsole::Mode::Palette;
    send_OK("TV-console PALETTE -> idx: " + String(app.console.paletteIdx) + " mode: " + app.console.mode);
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
    server.on("/test", handle_test);
    server.on("/tvcon", handle_tvcon);
    server.on("/tvcon/palette", handle_tvcon_palette);
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