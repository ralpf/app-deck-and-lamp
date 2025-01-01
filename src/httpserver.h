#pragma once

#include <WebServer.h>
#include <SPIFFS.h>


extern WebServer server;

extern String index_html;
extern String style_css;
extern String script_js;

// Serial should be inited before calling this
void InitWiFiServer(uint8_t ip_adress_last_block);
void InitHttpFrontend();