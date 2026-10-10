#include "devdiag.h"

#include <PrintHelper.h>
#include <stdio.h>
#include <esp_timer.h>
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <Esp.h>
#include <WiFi.h>




const char* unpack_reset_reason(int r)
{
    switch (r)
    {
        case ESP_RST_UNKNOWN:   return "ESP_RST_UNKNOWN";
        case ESP_RST_POWERON:   return "ESP_RST_POWERON";
        case ESP_RST_EXT:       return "ESP_RST_EXT";
        case ESP_RST_SW:        return "ESP_RST_SW";
        case ESP_RST_PANIC:     return "ESP_RST_PANIC";
        case ESP_RST_INT_WDT:   return "ESP_RST_INT_WDT";
        case ESP_RST_TASK_WDT:  return "ESP_RST_TASK_WDT";
        case ESP_RST_WDT:       return "ESP_RST_WDT";
        case ESP_RST_DEEPSLEEP: return "ESP_RST_DEEPSLEEP";
        case ESP_RST_BROWNOUT:  return "ESP_RST_BROWNOUT";
        case ESP_RST_SDIO:      return "ESP_RST_SDIO";
    }
    return "UNDEF";
}


const char* unpack_wifi_rssi(int rssi)
{
    if (rssi >= -67) return "Good";
    if (rssi >= -70) return "Okay";
    if (rssi >= -80) return "Weak";
    return                  "Very weak";
}


int DevDiag::PrintBuffer(char* buff, ui16 sz)
{
    if (!buff || sz == 0) return 0;

    ui64 totalSec = (ui64)(esp_timer_get_time() / 1000000);
    ui32 uptimeH = totalSec / 3600;
    ui32 uptimeM = (totalSec / 60) % 60;
    ui32 uptimeS = totalSec % 60;
    int rssi = WiFi.RSSI();

    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_8BIT);

    auto N = snprintf(buff, sz,
        "Uptime:           %02u:%02u:%02u\n"
        "Total heap:       %u KB\n"
        "Free heap:        %u KB\n"
        "Min free heap:    %u KB\n"
        "Largest block:    %u KB\n"
        "Free blocks:      %zu\n"
        "Allocated blocks: %zu\n"
        "CPU frequency:    %u MHz\n"
        "WiFi RSSI:        %d dBm (%s)\n"
        "Flash size:       %u KB\n"
        "Firmware size:    %u KB\n"
        "Chip model:       %s\n"
        "Reset reason:     %s",
        uptimeH, uptimeM, uptimeS,
        ESP.getHeapSize() / 1024,
        ESP.getFreeHeap() / 1024,
        ESP.getMinFreeHeap() / 1024,
        ESP.getMaxAllocHeap() / 1024,
        info.free_blocks,
        info.allocated_blocks,
        ESP.getCpuFreqMHz(),
        rssi, unpack_wifi_rssi(rssi),
        ESP.getFlashChipSize() / 1024,
        ESP.getSketchSize() / 1024,
        ESP.getChipModel(),
        unpack_reset_reason( esp_reset_reason() )
    );

    return N;
}


void DevDiag::PrintSerial()
{
    char buff[PRINTER_BUFFER_SZ];
    this->PrintBuffer(buff, PRINTER_BUFFER_SZ);
    SPrint("%s", buff);
}