#include "httpRequest.h"

#include <ESPAsyncWebServer.h>


HttpRequest::HttpRequest(AsyncWebServerRequest* req): request(req) { }


void HttpRequest::send(int code, const char* mimeType, const char* body)
{
    if (!request || responceSent) return;
    request->send(code, mimeType, body);
    responceSent = true;
}


void HttpRequest::send_ok(const char* msg)    { this->send(200, "text/plain", msg); }
void HttpRequest::send_fail(const char* msg)  { this->send(400, "text/plain", msg); }

bool HttpRequest::wasResponceSent()       const { return responceSent; }
AsyncWebServerRequest* HttpRequest::raw() { responceSent = true; return request; }