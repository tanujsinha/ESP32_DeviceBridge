
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"

#if CONFIG_APP_WIFI_SUPPORT
#include "wifi.h"
#endif

#if CONFIG_APP_WEB_UI_SUPPORT
#include "webui.h"
#endif

#if CONFIG_APP_ZIGBEE_SUPPORT
#include "zigbee.h"
#endif

void app_main(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s%s\n",
        CONFIG_IDF_TARGET,
        chip_info.cores,
        (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
        (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
        (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

#if CONFIG_APP_WIFI_SUPPORT
    wifi_init_sta();
#endif

#if CONFIG_APP_ZIGBEE_SUPPORT
    zigbee_init();
#endif

#if CONFIG_APP_WEB_UI_SUPPORT
    start_webserver();
#endif

    while (1) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
