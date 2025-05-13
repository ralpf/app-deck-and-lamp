#include "httpserver.h"


const char *ssid = "StarNet - munteanu.v84";
const char *password = "48575443A95B41AA";

//..................................................................................

WebServer server(80);

String index_html;
String style_css;
String script_js;


//..................................................................................

void readFile(const char* path, String& target);
void initWiFi(uint8_t ip_address_last_block);
void initFileData();

//..................................................................................

// callback called just to signal awaiting status. Float is time in sec
void InitWiFiServer(uint8_t ip_address_last_block, void(*callback)(float))
{
    const auto delayMs = 10;                                   // just for 'keep-alive' animation
    static float f;
    IPAddress local_IP(192, 168, 100, ip_address_last_block);   // static IP
    IPAddress gateway(192, 168, 100, 1);                        // Router IP
    IPAddress subnet(255, 255, 255, 0);                         // Subnet mask

    // Set static IP
    if (!WiFi.config(local_IP, gateway, subnet)) 
        Serial.println("ERR: Failed to configure static IP");

    WiFi.begin(ssid, password);
    Serial.print("... Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        f += 1.0 / delayMs;
        if (callback) callback(f);
        delay(delayMs);
    }

    Serial.print("\nOK: WiFi Connected IP=");
    Serial.println(WiFi.localIP());
    Serial.println();
}


void InitHttpFrontend()
{
    if (SPIFFS.begin(true) == false)
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }

    Serial.println("OK: SPIFFS FS mounted successfully");
    Serial.println("... Listing SPIFFS files:");

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    // ??? ok to not close files ?
    while (file)
    {
        Serial.print("\tFile: ");
        Serial.print(file.name());
        Serial.print(", Size: ");
        Serial.println(file.size());
        file = root.openNextFile();
    }

    readFile("/index.html", index_html);
    readFile("/style.css", style_css);
    readFile("/script.js", script_js);

    Serial.println("OK: Loaded index.html, script.js, style.css\n");
}

//..................................................................................

void readFile(const char* path, String& target)
{
    auto file = SPIFFS.open(path, "r");
    target = file.readString();
    file.close();
}