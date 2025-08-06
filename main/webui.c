#include "esp_log.h"
#include "esp_event.h"
#include "esp_http_server.h"

#include "webui.h"

static const char *TAG_WEBUI = "web_ui";

esp_err_t root_get_handler(httpd_req_t *req)
{
    const size_t html_len = index_html_end - index_html_start;

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, html_len);

    return ESP_OK;
}

httpd_uri_t root = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = root_get_handler,
    .user_ctx  = NULL
};

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &root);
        ESP_LOGI(TAG_WEBUI, "Web server started");
    } else {
        ESP_LOGE(TAG_WEBUI, "Failed to start web server");
    }
}