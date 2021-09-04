#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include <WiFi.h>
#include <ArduinoJson.h>

#include "sensors.h"
#include "RS485.h"
#include "BLE_server.h"
#include "LED.h"
#include "file_utils.h"
#include "WiFi_client.h"

uint8_t udp_packet[6 + sizeof(sensor_data)];
DynamicJsonDocument config(1024);

void GetMacAddress()
{
  //https://techtutorialsx.com/2018/03/09/esp32-arduino-getting-the-bluetooth-device-address/
  btStart();
  esp_bluedroid_init();
  esp_bluedroid_enable();

  Serial.print("BLE MAC Address:  ");
  const uint8_t* point = esp_bt_dev_get_address();
  for (int i = 0; i < 6; i++)
  { 
    char str[3];
   
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
    if (i < 5)
      Serial.print(":");
    udp_packet[i] = point[i];
  }
  Serial.println();
  
  String mac = WiFi.macAddress();
  
  Serial.print("WiFi MAC Address: ");
  Serial.println(mac);

  for (int i = 0; i < 6; i++)
  {
    uint8_t hi, lo;
    char c = mac.charAt(3 * i); 
    hi = c > '9' ? c - 'A' + 10 : c - '0';
    c = mac.charAt(3 * i + 1);
    lo = c > '9' ? c - 'A' + 10 : c - '0';
    //udp_packet[i] = (hi << 4) | lo;
  }
}

void WiFi_loop()
{
  //LED_setSaturation(32);
  while (1)
  {
    //Serial.println("Press button to measure and transmit...");
    //while(digitalRead(0));

    LED_setColor(GREEN);
    sensor_data *sd = (sensor_data *)(&udp_packet[6]);
    getSensorData(sd);
    printSensorData(*sd);

    const char* ip = config["udp_server"]["ip"]; 
    uint16_t port = config["udp_server"]["port"];
    LED_setColor(RED);
    WiFi_UDPsend(ip, port, udp_packet, sizeof(udp_packet));
    LED_setColor(BLACK);

    uint32_t t0 = millis();
    while (millis() - t0 < 300000)
    {
      LED_setColor(BLUE);
      delay(100);
      LED_setColor(BLACK);
      delay(900);
    }
  }

}

void setup()
{
  Serial.begin(115200);
  Serial.println("--- Sensing Unit ---");

  initSensors();
  GetMacAddress();
    
  LED_init();
  LED_setSaturation(255);
  FS_setup();
  
  char json[1024];
  FS_readFile(SPIFFS, "/config.json", json);

  deserializeJson(config, json);

  const char* _dev_mode = config["mode"];
  String dev_mode = String(_dev_mode);
  if (dev_mode == "WiFi standalone")
  {
    const char* _ssid = config["wifi"]["ssid"];
    const char* _pass = config["wifi"]["pass"];
    WiFi_setup(_ssid, _pass);

    WiFi_loop();
  }

  BLE_serverSetup();
  
  RS485_begin(RS485_TX);
}

void loop()
{
  //TODO: RS-485 code
  Serial.print("Press button to measure...");
  while(digitalRead(0));
  
  LED_setColor(RED);
  delay(1000);
  LED_setColor(BLACK);

  sensor_data sd;
  getSensorData(&sd);
  printSensorData(sd);
/*
  uint8_t *ptr = (uint8_t *)(&sd);
  for (uint8_t i = 0; i < sizeof(sensor_data); i++)
  {
    RS485.write(*ptr);
    ptr++;
  }
*/
}
