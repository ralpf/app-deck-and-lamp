#include "asyncBackend.h"

#include <PrintHelper.h>
#include <assertCrash.h>

#include <WiFi.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>

#ifndef IP_ADDRESS_4
  #error "IP_ADDRESS_4 symbol not #defined !! It sets the last byte of 192.168.100.* "
#endif

//............................................................................................STATIC

// think how to better store credentials
static const char* ssid = "StarNet - munteanu.v84";
static const char* password = "48575443A95B41AA";

// the html is hosted on esp32 in fylesystem as index.html

// the fancy async web server
static AsyncWebServer server(80);
static AsyncWebSocket wsocket("/ws/log");

// some sanity check
static bool isInited;
static bool isStarted;

// some fun
static ui32 count_req;

//.................................................................................WEBSOCKET-HANLDER

void on_websocket_event(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t data_len)
{
    switch (type)
    {
        case WS_EVT_CONNECT:
        Serial.printf("[WS] client %u connected\n", client->id());
        break;
        case WS_EVT_DISCONNECT:
        Serial.printf("[WS] client %u disconnected\n", client->id());
        break;
        // other available events, just for info
        case WS_EVT_DATA:
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
        default:
        break;
    }
}

//..................................................................................WEBSOCKET-HEADER

void websocket_log(const char* message)
{
    wsocket.textAll(message);
}

//........................................................................................LOCAL-FUNC

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


void init_websocket()
{
    server.addHandler(&wsocket);
    wsocket.onEvent(on_websocket_event);
}


bool validate_endpoint(const char* endpoint)
{
    if (endpoint == nullptr) return false;
    if (endpoint[0] != '/') return false;
    return true;
}

//.........................................................................................ENDPOINTS

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

//.........................................................................................NOT-FOUND

void register_endpoint_not_found()
{
    server.onNotFound([](AsyncWebServerRequest* req) {
        HttpRequest r(req);
        r.log_to_serial(404, ++count_req);
        r.send_notFound();
    });
}

//............................................................................................HEADER

void asyncBackend_init()
{
    init_wifi_server();
    init_frontend();
    init_websocket();
    isInited = true;
}


void asyncBackend_start()
{
    ASSERT(isInited, "ERR: init server before starting it");
    ASSERT(!isStarted, "ERR: server was already started");

    // endpoints available in all projects
    asyncBackend_register_endpoint("/", endpoint_root_handler);
    asyncBackend_register_endpoint("/test", endpoint_test_handler);
    // invalid endpoints
    register_endpoint_not_found();

    server.begin();
    isStarted = true;
}


void asyncBackend_register_endpoint(const char* endpoint, HttpRequestHandler handlerFunc, HttpRequest::Method type)
{
    // just resolve method type block
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
    // sanity check and report
    ASSERT(isInited, "ERR: init first before attaching endpoints");
    ASSERT(validate_endpoint(endpoint), "ERR: invalid endpoint");
    ASSERT(!isStarted, "ERR: can't attach more endpoints after server was started");
    SPrint("[AsyncBackend]: new endpoint -> %s: '%s'", methodName, endpoint);
    // do register function
    server.on(endpoint, method,
        [handlerFunc](AsyncWebServerRequest* req) {     // lambda
            HttpRequest r(req);                         // wrapper class
            r.log_to_serial(200, ++count_req);          // write using Serial from arduino
            handlerFunc(r);                             // invoke hanlder with wrapper as arg
            if (r.wasResponceSent() == false)           // autoresponce on forget to respond
                r.send_ok("Ok");
        }
    );
}
