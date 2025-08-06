
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_log.h"
#include "esp_event.h"

#if CONFIG_APP_WIFI_SUPPORT
#include "wifi.h"
#include "nvs_utils.h"
#endif

#if CONFIG_APP_WEB_UI_SUPPORT
#include "webui.h"
#endif

#if CONFIG_APP_ZIGBEE_SUPPORT
#include "zigbee.h"
#endif

static const char *TAG = "ESP32";

void app_main(void)
{
    bool has_credentials = false;
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s%s\n",
        CONFIG_IDF_TARGET,
        chip_info.cores,
        (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
        (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
        (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    nvs_init();

#if CONFIG_APP_WIFI_SUPPORT
    char ssid[32], pass[64];
    has_credentials = nvs_read_wifi_credentials(ssid, pass);
    ESP_LOGI(TAG, "Read credentials: %s, has_credentials=%s", ssid, has_credentials ? "true" : "false");
    wifi_auto_init();
#endif

#if CONFIG_APP_ZIGBEE_SUPPORT
    zigbee_init();
#endif

#if CONFIG_APP_WEB_UI_SUPPORT
    if (has_credentials)
    {
        ESP_LOGI(TAG, "Starting web server in normal mode");
        start_webserver(false); // Show index.html
    } 
    else
    {
        ESP_LOGI(TAG, "Starting web server in config mode");
        start_webserver(true); // Show Wi-Fi config page
    }
#endif

    while (1) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
