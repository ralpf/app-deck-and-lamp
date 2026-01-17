#pragma once
#include <Types.h>


class AsyncWebServerRequest;    // forward declaration, lives in ESPAsyncWebServer.h


class HttpRequest
{
    public:     // TYPE
    enum class Method : ui8 { Get, Post };

    public:     // METHOD
    //Method method() const;
    bool   wasResponceSent() const;
    AsyncWebServerRequest* raw();   // get raw pointer to request. The user should send responce codes by itself

    void send(int code, const char* mimeType, const char* body);
    void send_ok(const char* msg);
    void send_fail(const char* msg);
    void send_notFound();
    //void send_json(const char* json);

    bool try_arg_i(const char* name, i32&   out) const;                             // ingegers
    bool try_arg_f(const char* name, float& out) const;                             // flaots
    bool try_arg_b(const char* name, bool&  out) const;                             // booleans
    bool try_arg_s(const char* name, char*  outBuffer, ui16 sz) const;              // strings
    bool try_many (const char* name, void (*func)(ui8 idx, const char* payload));   // many strings, aka delegate Action<int,string>

    void log_to_serial(ui16 responceCode, ui32 requestNumber);

    public:     // CTOR
    HttpRequest(AsyncWebServerRequest* req);

    private:
    AsyncWebServerRequest* request = nullptr;
    bool responceSent = false;  // this is required
};