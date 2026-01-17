#pragma once
#include <Types.h>
#include "httpRequest.h"


// delegate
using HttpRequestHandler = void (*)(HttpRequest& request);

// call once before _start()
void asyncBackend_init();

// use this to add endpoints
void asyncBackend_register_endpoint(const char* endpoint, HttpRequestHandler handlerFunc, HttpRequest::Method type = HttpRequest::Method::Get);

// call after all endpoints are attached
void asyncBackend_start();



// note: I like the C style feel when we have exposed small funcs instead of overengineering the usecase
// C simplicity is awesome