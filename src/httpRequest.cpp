#include "httpRequest.h"

#include <ESPAsyncWebServer.h>
#include <cstring>


HttpRequest::HttpRequest(AsyncWebServerRequest* req): request(req) { }


void HttpRequest::send(int code, const char* mimeType, const char* body)
{
    if (!request || responceSent) return;
    request->send(code, mimeType, body);
    responceSent = true;
}


bool HttpRequest::try_arg_i(const char* name, i32& out) const
{
    auto webParam = request->getParam(name);
    if (!webParam) return false;
    // note, conversion returns 0 on bad formating, so 'abc' and '0' is indestinguishible
    out = webParam->value().toInt();
    return true;
}


bool HttpRequest::try_arg_f(const char* name, float& out) const
{
    auto webParam = request->getParam(name);
    if (!webParam) return false;
    // note, conversion returns 0 on bad formating, so 'abc' and '0' is indestinguishible
    out = webParam->value().toFloat();
    return true;
}


bool HttpRequest::try_arg_b(const char* name, bool&  out) const
{
    auto webParam = request->getParam(name);
    if (!webParam) return false;

    auto buff = webParam->value().c_str();
    switch (buff[0])
    {
        case '0': out = false; break;
        case '1': out = true;  break;
        default : return false;
    }
    // add tests for 'true' and 'false' (maybe)
    return true;
}


bool HttpRequest::try_arg_s(const char* name, char* outBuffer, ui16 sz) const
{
    auto webParam = request->getParam(name);
    if (!webParam) return false;
    if (!outBuffer || sz == 0) return true;

    strlcpy(outBuffer, webParam->value().c_str(), sz);
    return true;
}


bool HttpRequest::try_many (const char* name, void (*func)(ui8 idx, const char* payload))
{
    ui8 k = 0;
    bool wasFound = false;
    for (ui8 i = 0; i < request->params(); ++i)
    {
        auto p = request->getParam(i);
        if (p->name().equals(name) == false) continue;
        auto s = p->value();   // store in a local string to prevent dangling pointer to c-str
        func(k++, s.c_str());
        wasFound = true;
    }
    return wasFound;
}


void HttpRequest::send_ok(const char* msg)    { this->send(200, "text/plain", msg); }
void HttpRequest::send_fail(const char* msg)  { this->send(400, "text/plain", msg); }
void HttpRequest::send_notFound()             { this->send(404, "text/plain", "Not Found"); }

bool HttpRequest::wasResponceSent()       const { return responceSent; }
AsyncWebServerRequest* HttpRequest::raw() { responceSent = true; return request; }


void HttpRequest::log_to_serial(ui16 responceCode, ui32 requestNumber)
{
    if (!request) return;
    Serial.printf("\n%5i [WS] %s <~~ %s", requestNumber, request->methodToString(), request->url());
    if (request->params())
    {
        for (ui8 i = 0; i < request->params(); ++i)
        {
            auto param = request->getParam(i);
            Serial.print( i ? '&' : '?');
            Serial.print(param->name());
            Serial.print('=');
            Serial.print(param->value());
        }
    }
    Serial.println();
}