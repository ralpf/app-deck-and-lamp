#include "backend.h"
#include "utils.h"

extern bool recalculateLUT;

//............................................................................HELPER FUNC

void send_OK  (const String& msg)  { server.send(200, "text/plain", msg); }

void send_Fail(const String& msg)  { server.send(400, "text/plain", msg); }

void send_Json(const String& json) { server.send(200, "application/json", json); }

//........................................................................................

bool tryServerArgS(const String& arg, String& out)
{
    if (server.hasArg(arg) == false) return false;
    out = server.arg(arg);
    return true;
}

bool tryServerArgI(const String& arg, int& out)
{
    if (server.hasArg(arg) == false) return false;
    out = server.arg(arg).toInt();
    return true;
}

bool tryServerArgB(const String& arg, bool& out)
{
    if (server.hasArg(arg) == false) return false;
    out = server.arg(arg).toInt() != 0;
    return true;
}

bool tryServerArgF(const String& arg, float& out)
{
    if (server.hasArg(arg) == false) return false;
    out = server.arg(arg).toFloat();
    return true;
}

//..............................................................................MAIN HANDLES

void handleRoot() { server.send(200, "text/html", index_html); }

void handleCSS()  { server.send(200, "text/css", style_css); }

void handleJS()   { server.send(200, "application/javascript", script_js); }

//............................................................................GLOBAL HANDLES

void handle_global()
{
    int i = 255;

    if (tryServerArgI("bright", i))   app.brightness = i;
    if (tryServerArgI("gamma", i))  { app.gamma = i / 10.0; recalculateLUT = true; }
    if (tryServerArgI("animSkip", i)) app.animSkip = i;
    
    send_OK("OK globals. Bright: " + String(app.brightness) + " Gamma: " + String(app.gamma) );
}

//............................................................................BLAZAR HANDLES

void handle_mode_Mood()
{
    int i = 0;
    String html_color;

    if (tryServerArgS("color", html_color)) app.lamp.color32 = html_2_UI32(html_color);
    if (tryServerArgI("is_flicker", i))     app.lamp.is_fliker = i != 0;
    if (tryServerArgI("hue_ampl", i))       app.lamp.noiseHue.ampl = i;
    if (tryServerArgI("hue_ts", i))         app.lamp.noiseHue.timeScale = i / 1000.0;
    if (tryServerArgI("brt_ampl", i))       app.lamp.noiseBrt.ampl = i;
    if (tryServerArgI("brt_ts", i))         app.lamp.noiseBrt.timeScale = i / 1000.0;
    if (tryServerArgI("bright", i))         app.lamp.bright = i;

    send_OK("OK mode Mood");
}


//............................................................................TEST HANDLES

void handle_test()
{
    String s = "entering testing mode";
}

//...........................................................................TVCON HANDLES

void handle_tvcon()
{
    int i = 0;
    bool b = false;
    
    if (tryServerArgI("bright", i))  app.console.bright = constrain(i, 0, 255);
    if (tryServerArgI("blend", i))   app.console.blend  = constrain(i, 0, 255);
    if (tryServerArgI("anim", i))    app.console.anim   = constrain(i, 0, 255);
    
    send_OK("Received /tvcon");
}


void handle_tvcon_mirror()
{
    app.console.mode = TVConsole::Mode::MirrorLamp;
    send_OK("Received /tvcon/mirror");
}


void handle_tvcon_palette()
{
    int i = 0;
    bool b = false;
    String s;
    
    if (tryServerArgI("idx", i))    app.console.palette.idx = i;
    if (tryServerArgB("irand", b))  app.console.palette.irand = b;

    app.console.mode = TVConsole::Mode::Palette;
    send_OK("Received /tvcon/palette");
}


void handle_tvcon_palette_names()
{
    send_Json(app.console.palette.namesJson);
}


void handle_tvcon_hsv()
{
    int i = 0;
    
    if (tryServerArgI("hue", i))    app.console.hsv.h = constrain(i, 0, 255);
    if (tryServerArgI("sat", i))    app.console.hsv.s = constrain(i, 0, 255);
    
    app.console.mode = TVConsole::Mode::HSV;
    send_OK("Received /tvcon/hsv");
}   

//...............................................................................APP HANDLES

void InitBackend()
{
    server.on("/",          handleRoot);
    server.on("/style.css", handleCSS);
    server.on("/script.js", handleJS);

    server.on("/global",                handle_global);
    server.on("/mood",                  handle_mode_Mood);
    server.on("/test",                  handle_test);
    server.on("/tvcon",                 handle_tvcon);
    server.on("/tvcon/mirror",          handle_tvcon_mirror);
    server.on("/tvcon/palette",         handle_tvcon_palette);
    server.on("/tvcon/hsv",             handle_tvcon_hsv);
    server.on("/tvcon/palette/names",   handle_tvcon_palette_names);

    server.begin();
    Serial.println("OK: Web server started\n");
}
