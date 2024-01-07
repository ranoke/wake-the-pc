#include "wp_setup.h"
#include "wp_http.h"
#include "wp_wake.h"

void app_main(void)
{
    if (init_storage())
    {
        printf("Failed to init storage\n");
        esp_restart();
    }

    if (init_network())
    {
        printf("Failed to init network\n");
        esp_restart();
    }

    if (init_time())
    {
        printf("Failed to init time\n");
        esp_restart();
    }

    while (true)
    {
        bool on = false;
        ESP_ERROR_CHECK(poll_on_status(&on));
        ESP_ERROR_CHECK(update_status());

        if (on)
        {
            ESP_ERROR_CHECK(turn_on_pc());
            ESP_ERROR_CHECK(reset_on_status());
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    esp_restart();
}
