/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
    https://github.com/nkolban/ESP32_BLE_Arduino
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>

#include "BLE_server.h"
#include "sensors.h"
#include "LED.h"
#include "file_utils.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

class MyServerCallbacks: public BLEServerCallbacks 
{
    void onConnect(BLEServer* pServer) 
    {
      LED_setColor(BLUE);
      delay(1000);
      LED_setColor(BLACK);
      //deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) 
    {
      LED_setColor(PURPLE);
      delay(200);
      LED_setColor(BLACK);
      //deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks 
{
  void onRead(BLECharacteristic *pCharacteristic) 
  {
    LED_setColor(GREEN);
    delay(200);
    LED_setColor(BLACK);
  }
  
  void onWrite(BLECharacteristic *pCharacteristic) 
  {
    String rxData = pCharacteristic->getValue().c_str();
    Serial.print("Rx -> ");
    Serial.println(rxData);

    if (rxData == "READ")
    {
      LED_setColor(RED);

      sensor_data sd;
      getSensorData(&sd);
      printSensorData(sd);
      pCharacteristic->setValue((uint8_t *)&sd, sizeof(sensor_data));

      LED_setColor(BLACK);
    }
  }
};

void BLE_serverSetup()
{
  Serial.println("Starting BLE work!");
  BLEDevice::init("IO3T-SU");

  char json[1024];
  FS_readFile(SPIFFS, "/BLE_UUID.json", json);

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  const char* SERV_UUID = doc["SERV_UUID"];
  const char* CHAR_UUID = doc["CHAR_UUID"];
  
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERV_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHAR_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERV_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined!");
}
