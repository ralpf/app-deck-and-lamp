#include "asyncBackend.h"

#include <PrintHelper.h>
#include <assertCrash.h>

#include <WiFi.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>

#ifndef IP_ADDRESS_4
  #error "IP_ADDRESS_4 not defined. It sets the last byte of 192.168.100.* "
#endif

//...........................................................................STATIC

// think how to better store credentials
static const char* ssid = "StarNet - munteanu.v84";
static const char* password = "48575443A95B41AA";

// the html is hosted on esp32 in fylesystem as index.html

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


void init_frontend()
{
    bool ok = SPIFFS.begin(true);
    ASSERT(ok, "ERR: SPIFFS file system mount failed");
}


bool validate_endpoint(const char* endpoint)
{
    if (endpoint == nullptr) return false;
    if (endpoint[0] != '/') return false;
    return true;
}

//.........................................................................ENDPOINTS

void endpoint_test_handler(HttpRequest& request)
{
    SPrint("Received [test] request");
    // not sending respoce will cause auto responce 200 Ok
}


void endpoint_root_handler(HttpRequest& request)
{
    // this raw send does send 200 code itself
    // also using raw will bypass custom auto-send responce in asyncBackend_register_endpoint()
    request.raw()->send(SPIFFS, "/index.html", "text/html");
}

//........................................................................HEADER-FUNC

void asyncBackend_register_endpoint(HttpRequest::Method type, const char* endpoint, HttpRequestHandler handlerFunc)
{
    // just resolve method type
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
    // sanity check
    ASSERT(isInited, "ERR: init first before attaching endpoints");
    ASSERT(validate_endpoint(endpoint), "ERR: invalid endpoint");
    ASSERT(!isStarted, "ERR: can't attach more endpoints after server was started");
    SPrint("[AsyncBackend]: new endpoint -> %s: '%s'", methodName, endpoint);
    // do register
    server.on(endpoint, method,
        [handlerFunc](AsyncWebServerRequest* req) {     // lambda
            HttpRequest r(req);                         // wrapper class
            handlerFunc(r);                             // invoke hanlder with wrapper as arg
            if (r.wasResponceSent() == false)           // autoresponce on forget to respond
                r.send_ok("Ok");
        }
    );
}


void asyncBackend_register_endpoint(const char* endpoint, HttpRequestHandler handlerFunc)
{
    asyncBackend_register_endpoint(HttpRequest::Method::Get, endpoint, handlerFunc);
}


void asyncBackend_init()
{
    init_wifi_server();
    init_frontend();
    isInited = true;
}


void asyncBackend_start()
{
    ASSERT(isInited, "ERR: init server before starting it");
    ASSERT(!isStarted, "ERR: server was already started");

    // endpoints available in all projects
    asyncBackend_register_endpoint(HttpRequest::Method::Get, "/", endpoint_root_handler);
    asyncBackend_register_endpoint(HttpRequest::Method::Get, "/test", endpoint_test_handler);

    server.begin();
    isStarted = true;
}