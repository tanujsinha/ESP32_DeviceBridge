idf_component_register(SRCS
    "app_main.c"
    "wifi.c"
    "webui.c"
    "zigbee.c"
    INCLUDE_DIRS "."
    EMBED_TXTFILES "../html/index.html"
)