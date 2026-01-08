#include "asyncBackend.h"

#include <PrintHelper.h>
#include <assertCrash.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// here we expect per project defines (use platform.ini for this)
#ifndef HTML_URL
  #error "HTML_URL not defined. Contains URL to the real index.html"
#endif

#ifndef IP_ADDRESS_4
  #error "IP_ADDRESS_4 not defined. It sets the last byte of 192.168.100.* "
#endif

//#define HTML_URL ""

// this will be moved away and sent as function args
static const char* ssid = "StarNet - munteanu.v84";
static const char* password = "48575443A95B41AA";

// this is the html stub
static const char* index_html;

// the fancy async web server
static AsyncWebServer server(80);

// some sanity check
static bool isInited;
static bool isStarted;

//........................................................................LOCAL-FUNC

void init_wifi_server()
{
    IPAddress localIp(192, 168, 100, IP_ADDRESS_4);
    IPAddress routerIp(192, 168, 100, 1);
    IPAddress subnetMask(255, 255, 255, 0);
    
    WiFi.mode(WIFI_STA);
    bool ok = WiFi.config(localIp, routerIp, subnetMask);
    ASSERT(ok, "ERR: Failed to configure WiFi with static IP");

    SPrint("... Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        SPrint(".");
        delay(300);
    }
    SPrint("OK: connected WiFi\nto: '%s'\nip: %s\n", ssid, localIp.toString().c_str());
}


void init_http_frontend()
{
    // NOTE, we don't host anymore files in SPIFFS,
    // but provide a small html stub instead
    // also note here we use C compiler feature of string concatination (auto s = "hello " "compiler";)
    index_html = 
R"HTML(
<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <title>Loading Frontend</title>
  <meta http-equiv="refresh"
        content="0; url=")HTML" HTML_URL R"HTML(">

  <style>
    html, body {
      margin: 0;
      padding: 0;
      height: 100%;
      font-family: sans-serif;
      background: #111;
      color: #ddd;
    }

    #disclaimer {
      display: flex;
      align-items: center;
      justify-content: center;
      height: 100%;
      font-size: 16px;
    }
  </style>
</head>
<body>
  <div id="disclaimer">Caching Frontend Files…</div>
</body>
</html>
)HTML";
}

bool validate_endpoint(const char* endpoint)
{
    if (endpoint == nullptr) return false;
    if (endpoint[0] != '/') return false;
    return true;
}

//..........................................................................TEST-FUNC

void test_handler(HttpRequest& request)
{
    SPrint("Received [test] request");
    // not sending respoce will cause auto responce 200 Ok
}

//........................................................................HEADER-FUNC

void asyncBackend_register_endpoint(HttpRequest::Method type, const char* endpoint, HttpRequestHandler handlerFunc)
{
    const char* methodName = "";
    WebRequestMethod method = HTTP_ANY;
    switch (type)
    {
        case HttpRequest::Method::Get:
        method = HTTP_GET;
        methodName = "GET";
        break;
        case HttpRequest::Method::Post:
        method = HTTP_POST;
        methodName = "POST";
        break;
        default: ASSERT(false, "unexpected method type");
    }
    
    ASSERT(isInited, "ERR: init first before attaching endpoints");
    ASSERT(validate_endpoint(endpoint), "ERR: invalid endpoint");
    ASSERT(!isStarted, "ERR: can't attach more endpoints after server was started");
    SPrint("[AsyncBackend]: new endpoint -> %s: '%s'", methodName, endpoint);

    server.on(endpoint, method,
        [handlerFunc](AsyncWebServerRequest* req) {     // lambda
            HttpRequest r(req);                         // wrapper class
            handlerFunc(r);                             // invoke hanlder with wrapper as arg
            if (r.wasResponceSent() == false)           // autoresponce on forget to respond
                r.send_ok("Ok");
        }
    );
}


void asyncBackend_init()
{
    init_wifi_server();
    init_http_frontend();
    isInited = true;
    // attach a test func
    asyncBackend_register_endpoint(HttpRequest::Method::Get, "/test", test_handler);
}


void asyncBackend_start()
{
    ASSERT(isInited, "ERR: init server before starting it");
    ASSERT(!isStarted, "ERR: server was already started");

    server.begin();
    isStarted = true;
}