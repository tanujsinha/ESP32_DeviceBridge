idf_component_register(SRCS
    "app_main.c"
    "wifi.c"
    "webui.c"
    "nvs_utils.c"
    "zigbee.c"
    INCLUDE_DIRS "."
    EMBED_TXTFILES
        "../html/style.css"
        "../html/index.html"
        "../html/wifi_config.html"
    REQUIRES
        "esp_matter"
        "esp-zigbee-lib"
        "esp_http_server"
)