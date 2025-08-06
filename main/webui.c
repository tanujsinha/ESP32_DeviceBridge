#include "esp_log.h"
#include "esp_http_server.h"
#include "webui.h"
#include "wifi.h"
#include "nvs_utils.h"
#include "esp_system.h"
#include <ctype.h>

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

extern const uint8_t wifi_config_html_start[] asm("_binary_wifi_config_html_start");
extern const uint8_t wifi_config_html_end[] asm("_binary_wifi_config_html_end");

extern const char style_css_start[] asm("_binary_style_css_start");
extern const char style_css_end[]   asm("_binary_style_css_end");

static const char *TAG_WEBUI = "webui";

esp_err_t serve_style_css(httpd_req_t *req) {
    extern const char style_css_start[] asm("_binary_style_css_start");
    extern const char style_css_end[]   asm("_binary_style_css_end");
    const size_t style_css_size = style_css_end - style_css_start;

    httpd_resp_set_type(req, "text/css");
    return httpd_resp_send(req, style_css_start, style_css_size);
}

esp_err_t root_get_handler(httpd_req_t *req)
{
    const size_t html_len = index_html_end - index_html_start;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, html_len);
    return ESP_OK;
}

esp_err_t config_get_handler(httpd_req_t *req)
{
    const size_t html_len = wifi_config_html_end - wifi_config_html_start;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)wifi_config_html_start, html_len);
    return ESP_OK;
}

esp_err_t portal_redirect_handler(httpd_req_t *req)
{
    httpd_resp_set_status(req, "302 Temporary Redirect");
    httpd_resp_set_hdr(req, "Location", "/config");
    return httpd_resp_sendstr(req, "Redirecting...");
}

void url_decode(char *dst, const char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a') a -= 'a' - 'A';
            if (a >= 'A') a -= ('A' - 10);
            else a -= '0';
            if (b >= 'a') b -= 'a' - 'A';
            if (b >= 'A') b -= ('A' - 10);
            else b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

esp_err_t config_post_handler(httpd_req_t *req)
{
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf));
    if (len <= 0) return ESP_FAIL;

    buf[len] = '\0';

    char raw_ssid[64] = "", raw_password[64] = "";
    char ssid[64] = "", password[64] = "";
    sscanf(buf, "ssid=%31[^&]&password=%63s", raw_ssid, raw_password);

    // Decode
    url_decode(ssid, raw_ssid);
    url_decode(password, raw_password);

    ESP_LOGI(TAG_WEBUI, "Saving ssid: %s, password: %s", ssid, password);
    nvs_save_wifi_credentials(ssid, password);

    httpd_resp_sendstr(req, "Wi-Fi credentials saved. Rebooting...");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_restart();
    return ESP_OK;
}

httpd_uri_t root = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler
};

httpd_uri_t config_get = {
    .uri = "/config",
    .method = HTTP_GET,
    .handler = config_get_handler
};

httpd_uri_t config_post = {
    .uri = "/config",
    .method = HTTP_POST,
    .handler = config_post_handler
};

httpd_uri_t root_redirect = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = portal_redirect_handler
};

httpd_uri_t uri_android = {
    .uri = "/generate_204",
    .method = HTTP_GET,
    .handler = portal_redirect_handler
};

httpd_uri_t uri_windows = {
    .uri = "/fwlink",
    .method = HTTP_GET,
    .handler = portal_redirect_handler
};

httpd_uri_t uri_ios = {
    .uri = "/hotspot-detect.html",
    .method = HTTP_GET,
    .handler = portal_redirect_handler
};

httpd_uri_t style_css_uri = {
    .uri       = "/style.css",
    .method    = HTTP_GET,
    .handler   = serve_style_css,
    .user_ctx  = NULL
};

void start_webserver(bool config_mode)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &style_css_uri);
        if (config_mode) {
            httpd_register_uri_handler(server, &root_redirect);
            httpd_register_uri_handler(server, &config_get);       // serve the /config page
            httpd_register_uri_handler(server, &config_post);      // handle form submission
            httpd_register_uri_handler(server, &uri_android);
            httpd_register_uri_handler(server, &uri_windows);
            httpd_register_uri_handler(server, &uri_ios);
            ESP_LOGI(TAG_WEBUI, "Config web server started");
        } else {
            httpd_register_uri_handler(server, &root);
            ESP_LOGI(TAG_WEBUI, "Main web server started");
        }
    }
}