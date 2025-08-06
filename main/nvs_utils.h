void nvs_init(void);
bool nvs_read_wifi_credentials(char *ssid, char *password);
bool nvs_save_wifi_credentials(const char *ssid, const char *password);
