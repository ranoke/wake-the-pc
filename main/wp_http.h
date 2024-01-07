#ifndef WP_HTTP_H_
#define WP_HTTP_H_

#include "wp_base.h"


#define HTTP_STATUS_OK 200
#define MIN(a, b) (a < b ? a : b)

typedef struct
{
    char* buf;
    size_t size;
    size_t read_size;
} http_event_userdata_t;

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    http_event_userdata_t *ud = evt->user_data;

    switch(evt->event_id)
    {
        case HTTP_EVENT_ON_DATA: {
            memset(ud->buf, 0, ud->size);
            int64_t copy_len = MIN(ud->size, esp_http_client_get_content_length(evt->client));
            memcpy(ud->buf, evt->data, copy_len);
            ud->read_size = copy_len;
        } break;
        case HTTP_EVENT_ERROR: break;
        case HTTP_EVENT_ON_CONNECTED: break;
        case HTTP_EVENT_HEADER_SENT: break;
        case HTTP_EVENT_ON_HEADER: break;
        case HTTP_EVENT_ON_FINISH: break;
        case HTTP_EVENT_DISCONNECTED: break;
        case HTTP_EVENT_REDIRECT: break;
    }
    return ESP_OK;
}

esp_err_t reset_on_status()
{
    http_event_userdata_t ud = { .buf = NULL, .size = 0 };

    esp_http_client_config_t cfg = {
        .url = DB_ENDPOINT ".json",
        .event_handler = _http_event_handler,
        .user_data = &ud,
        .method = HTTP_METHOD_PATCH,
    };

    char payload[32] = {};
    size_t payload_size = sprintf(payload, "{\"on\":false}");

    esp_http_client_handle_t handle = esp_http_client_init(&cfg);
    ESP_ERROR_CHECK(esp_http_client_set_post_field(handle, payload, payload_size));
    ESP_ERROR_CHECK(esp_http_client_perform(handle));
    printf("Reset status: %d\n", esp_http_client_get_status_code(handle));
    if (esp_http_client_get_status_code(handle) != HTTP_STATUS_OK)
        return ESP_FAIL;

    ESP_ERROR_CHECK(esp_http_client_cleanup(handle));

    return ESP_OK;

}

esp_err_t update_status()
{
    http_event_userdata_t ud = { .buf = NULL, .size = 0 };
    time_t now = time(NULL);

    esp_http_client_config_t cfg = {
        .url = DB_ENDPOINT ".json",
        .event_handler = _http_event_handler,
        .user_data = &ud,
        .method = HTTP_METHOD_PATCH,
    };

    char payload[32] = {};
    size_t payload_size = sprintf(payload, "{\"update\":%llu}", now);

    esp_http_client_handle_t handle = esp_http_client_init(&cfg);
    ESP_ERROR_CHECK(esp_http_client_set_post_field(handle, payload, payload_size));
    ESP_ERROR_CHECK(esp_http_client_perform(handle));
    if (esp_http_client_get_status_code(handle) != HTTP_STATUS_OK)
        return ESP_FAIL;

    ESP_ERROR_CHECK(esp_http_client_cleanup(handle));

    return ESP_OK;
}

esp_err_t poll_on_status(bool* on)
{
    char response[MAX_HTTP_OUTPUT_BUFFER] = {};

    http_event_userdata_t ud = { .buf = response, .size = sizeof(response) };

    esp_http_client_config_t cfg = {
        .url = DB_ENDPOINT "on.json",
        .event_handler = _http_event_handler,
        .user_data = &ud,
    };

    esp_http_client_handle_t handle = esp_http_client_init(&cfg);
    ESP_ERROR_CHECK(esp_http_client_perform(handle));

    if (esp_http_client_get_status_code(handle) != HTTP_STATUS_OK)
        return ESP_FAIL;

    printf("Status: %s\n", ud.buf);
    *on = strcmp(response, "true") == 0;

    ESP_ERROR_CHECK(esp_http_client_cleanup(handle));

    return ESP_OK;
}

#endif // WP_HTTP_H_