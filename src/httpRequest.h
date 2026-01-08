#pragma once
#include <Types.h>


class AsyncWebServerRequest;    // forward declaration, lives in ESPAsyncWebServer.h


class HttpRequest
{
    public:     // TYPE
    enum class Method : ui8 { Get, Post };

    public:     // METHOD
    Method method() const;
    bool   wasResponceSent() const;

    void send_ok(const char* msg);
    void send_fail(const char* msg);
    void send_json(const char* json);

    bool try_arg_i(const char* name, i32&   out) const;
    bool try_arg_f(const char* name, float& out) const;
    bool try_arg_s(const char* name, char*  outBuffer, ui16 sz) const;

    public:     // CTOR
    HttpRequest(AsyncWebServerRequest* req);

    private:
    AsyncWebServerRequest* request = nullptr;
    bool responceSent = false;  // this is required
};