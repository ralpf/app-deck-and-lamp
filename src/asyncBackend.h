#pragma once
#include <Types.h>
#include "httpRequest.h"


// delegate
using HttpRequestHandler = void (*)(HttpRequest& request);

// call once before _start()
void asyncBackend_init();

// use this to add endpoints
void asyncBackend_register_endpoint(HttpRequest::Method type, const char* endpoint, HttpRequestHandler handlerFunc);

// call after all endpoints are attached
void asyncBackend_start();



// note: I like the C style feel when we have exposed small funcs instead of overengineering the usecase
// C simplicity is awesome