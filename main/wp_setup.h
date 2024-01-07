#ifndef WP_SETUP_H_
#define WP_SETUP_H_

#include "wp_base.h"



void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
}

esp_err_t init_storage()
{
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());

    return ESP_OK;
}

esp_err_t init_network()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_configuration = { 
        .sta = {
            .ssid = WIFI_SSID,
            .password= WIFI_PASSWORD
        }
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    return ESP_OK;
}

esp_err_t init_time()
{
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config);
    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK)
    {
        printf("Failed to update system time within 10s timeout");
        return ESP_FAIL;
    }

    return ESP_OK;
}

#endif // WP_SETUP_H_