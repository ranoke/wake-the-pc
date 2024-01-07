#ifndef WP_WAKE_H_
#define WP_WAKE_H_

#include "wp_base.h"

esp_err_t turn_on_pc()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Socket creation failed");
        return ESP_FAIL;
    }

    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(BROADCAST_MACADDR);
    addr.sin_port = htons(9);

    unsigned char packet[102] = {};
    memset(packet, 0xFF, 6); 
    unsigned char mac[6];
    sscanf(PC_MACADDR, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", 
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    
    for(int i = 1; i <= 16; ++i) {
        memcpy(&packet[i * 6], mac, 6);
    }

    if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        printf("Packet send failed\n");
    } else {
        printf("Magic Packet sent successfully\n");
    }

    close(sock);

    return ESP_OK;
}

#endif // WP_WAKE_H_