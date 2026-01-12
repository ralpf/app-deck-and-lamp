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


void HttpRequest::send_ok(const char* msg)    { this->send(200, "text/plain", msg); }
void HttpRequest::send_fail(const char* msg)  { this->send(400, "text/plain", msg); }
void HttpRequest::send_notFound()             { this->send(404, "text/plain", "Not Found"); }

bool HttpRequest::wasResponceSent()       const { return responceSent; }
AsyncWebServerRequest* HttpRequest::raw() { responceSent = true; return request; }


void HttpRequest::log_to_serial()
{
    if (!request) return;
    Serial.print("[WS] ");
    Serial.print(request->methodToString());
    Serial.print("   ");
    Serial.println(request->url());
}