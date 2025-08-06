#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>
#include <stdio.h>

#define NVS_NAMESPACE "wifi_config"

void nvs_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated or needs to be upgraded
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to initialize NVS: %s", esp_err_to_name(err));
    }
}

bool nvs_read_wifi_credentials(char *ssid, char *password) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGW("NVS", "Failed to open NVS: %s", esp_err_to_name(err));
        return false;
    }

    size_t ssid_len = 0, pass_len = 0;

    // Step 1: Get required lengths first
    err = nvs_get_str(nvs_handle, "ssid", NULL, &ssid_len);
    if (err != ESP_OK || ssid_len <= 1 || ssid_len > 32) {
        nvs_close(nvs_handle);
        ESP_LOGW("NVS", "SSID not found or invalid (len=%d)", (int)ssid_len);
        return false;
    }

    err = nvs_get_str(nvs_handle, "password", NULL, &pass_len);
    if (err != ESP_OK || pass_len <= 1 || pass_len > 64) {
        nvs_close(nvs_handle);
        ESP_LOGW("NVS", "Password not found or invalid (len=%d)", (int)pass_len);
        return false;
    }

    // Step 2: Actually read the strings
    err = nvs_get_str(nvs_handle, "ssid", ssid, &ssid_len);
    if (err != ESP_OK) {
        nvs_close(nvs_handle);
        ESP_LOGW("NVS", "Failed to read SSID: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_get_str(nvs_handle, "password", password, &pass_len);
    nvs_close(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGW("NVS", "Failed to read password: %s", esp_err_to_name(err));
        return false;
    }

    // Optional: Extra validation
    if (strlen(ssid) == 0 || strlen(password) == 0) {
        ESP_LOGW("NVS", "SSID or password is empty");
        return false;
    }

    ESP_LOGI("NVS", "SSID: %s", ssid); // For debug only, remove in production
    return true;
}

bool nvs_save_wifi_credentials(const char *ssid, const char *password) {
    // Bonus: Input validation
    if (!ssid || !password || strlen(ssid) == 0 || strlen(password) == 0) {
        ESP_LOGW("NVS", "Invalid SSID or password (null or empty)");
        return false;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to open NVS: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_str(nvs_handle, "ssid", ssid);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to write SSID: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_set_str(nvs_handle, "password", password);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to write password: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE("NVS", "Failed to commit changes: %s", esp_err_to_name(err));
        return false;
    }

    ESP_LOGI("NVS", "Wi-Fi credentials saved successfully.");
    return true;
}
