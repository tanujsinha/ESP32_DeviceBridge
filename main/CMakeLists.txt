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
)