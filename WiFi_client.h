#ifndef _WIFI_CLIENT_H
#define _WIFI_CLIENT_H

void WiFi_setup(const char* ssid, const char* pass);
void WiFi_UDPsend(const char* ip, uint16_t port, uint8_t udp_packet[], uint16_t size);

#endif
