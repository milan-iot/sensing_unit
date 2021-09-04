#include <Arduino.h>
#include "WiFi_client.h"
#include "LED.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>

WiFiUDP udp;
WiFiMulti wifiMulti;

void WiFi_setup(const char* ssid, const char* pass)
{
  Serial.print("Connecting to WiFi");
  wifiMulti.addAP(ssid, pass);
  do
  {
    Serial.print('.');
    LED_setColor(CYAN);
    delay(100);
    LED_setColor(BLACK);
  }
  while (!(wifiMulti.run() == WL_CONNECTED));
  Serial.println(" OK!");
  Serial.print("IP address: "); 
  LED_setColor(CYAN);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  delay(1000);
  LED_setColor(BLACK);
}

void WiFi_UDPsend(const char* ip, uint16_t port, uint8_t udp_packet[], uint16_t size)
{
  udp.beginPacket(ip, port);
  //TX packet
  for (int i = 0; i < size; i++)
    udp.write(udp_packet[i]);
  udp.endPacket();

  //RX packet
  uint32_t t0 = millis();
  while(!udp.available() && (millis() - t0 < 5000))
    udp.parsePacket();
  char rx_buffer[32];
  size = udp.available();
  udp.read(rx_buffer, size);
  rx_buffer[size] = '\0';
  Serial.print("RX -> ");
  Serial.println(rx_buffer);
}
