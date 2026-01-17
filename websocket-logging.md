Understood.
Reset.
**Only shape. Only setup. Only binding. No extras. No commentary.**

---

### WebSocket setup (ESPAsyncWebServer, ESP32)

```cpp
// global / static
AsyncWebSocket wsLog("/ws/log");
```

Attach to existing server:

```cpp
server.addHandler(&wsLog);
```

Bind event handler **(no lambda)**:

```cpp
wsLog.onEvent(WsLogManager::OnEvent);
```

---

### Event handler contract (exact signature)

```cpp
struct WsLogManager
{
    static void OnEvent(
        AsyncWebSocket* server,
        AsyncWebSocketClient* client,
        AwsEventType type,
        void* arg,
        uint8_t* data,
        size_t len
    );
};
```

That’s it.
Nothing else is required to have an async WebSocket endpoint ready.

---

### URL

```
ws://<esp-ip>/ws/log
```




Example
---
### Inside the event handler
```cpp
void WsLogManager::OnEvent(
    AsyncWebSocket* server,
    AsyncWebSocketClient* client,
    AwsEventType type,
    void*,
    uint8_t*,
    size_t
)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("[WS] client %u connected\n", client->id());
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("[WS] client %u disconnected\n", client->id());
    }
}
```
That’s the **only place** where connect / disconnect events exist.
Those two `AwsEventType` values are the signals.


These are **all WebSocket event types** you will see in `ESPAsyncWebServer`
(`AwsEventType` enum). Nothing else exists.

```cpp
WS_EVT_CONNECT
WS_EVT_DISCONNECT
WS_EVT_DATA
WS_EVT_PONG
WS_EVT_ERROR
```

### Meaning (one-liners)

* **WS_EVT_CONNECT**
  Client opened the WebSocket.

* **WS_EVT_DISCONNECT**
  Client closed / dropped connection.

* **WS_EVT_DATA**
  Incoming frame from client
  (text / binary / fragmented).

* **WS_EVT_PONG**
  Pong received (reply to ping).

* **WS_EVT_ERROR**
  Transport or protocol error.

That’s the complete contract.
