#include "httpRequest.h"

#include <ESPAsyncWebServer.h>



HttpRequest::HttpRequest(AsyncWebServerRequest* req): request(req) { }


void HttpRequest::send_ok(const char* msg)
{
    if (!request || responceSent) return;
    request->send(200, "text/plain", msg);
    responceSent = true;
}


void HttpRequest::send_fail(const char* msg)
{
    if (!request || responceSent) return;
    request->send(400, "text/plain", msg);
    responceSent = true;
}


bool HttpRequest::wasResponceSent() const
{
    return responceSent;
}