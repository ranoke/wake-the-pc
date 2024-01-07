#ifndef WP_BASE_H_
#define WP_BASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_dpp.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_check.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_netif_sntp.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define PC_MACADDR CONFIG_PC_MACADDR
#define BROADCAST_MACADDR CONFIG_BROADCAST_MACADDR
#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASSWORD CONFIG_WIFI_PASSWORD
#define DB_ENDPOINT CONFIG_DB_ENDPOINT
#define MAX_HTTP_OUTPUT_BUFFER CONFIG_MAX_HTTP_OUTPUT_BUFFER

#endif // WP_BASE_H_