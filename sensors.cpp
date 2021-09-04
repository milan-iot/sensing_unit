#include <Arduino.h>
#include <DallasTemperature.h>
#include <BH1750FVI.h>
#include <Wire.h>
#include <SparkFunBME280.h>
#include "sensors.h"

//DS18B20 soil temperature 
OneWire DS18B20_onewire_1(18);
OneWire DS18B20_onewire_2(19);
DallasTemperature DS18B20_1(&DS18B20_onewire_1);
DallasTemperature DS18B20_2(&DS18B20_onewire_2);

//soil moisture sensors
#define SOIL_IN_1  35
#define SOIL_IN_2  4
#define SOIL_DRY 4096
#define SOIL_WET 0
typedef enum {SOIL_MOISTURE_1, SOIL_MOISTURE_2} sm_sensor;

//BH1750FVI
BH1750FVI::eDeviceAddress_t DEVICEADDRESS = BH1750FVI::k_DevAddress_L;
BH1750FVI::eDeviceMode_t DEVICEMODE = BH1750FVI::k_DevModeContHighRes;
BH1750FVI LightSensor(23, DEVICEADDRESS, DEVICEMODE);

//BME280
BME280 bme280; //Uses I2C address 0x76 (jumper closed)

void initSensors()
{
  //sensor init
  DS18B20_1.begin();
  DS18B20_2.begin();
  LightSensor.begin();

  Wire.begin();
  bme280.setI2CAddress(0x76); //Connect to a second sensor
  if(bme280.beginI2C() == false) 
    Serial.println("BME280 connect failed");

  //dummy measurement
  sensor_data sd;
  getSensorData(&sd);
}

int soil_moisture(sm_sensor sms)
{
  int moist, val;
  if (sms == SOIL_MOISTURE_1)
    val = analogRead(SOIL_IN_1);
  else
    val = analogRead(SOIL_IN_2);

  //Serial.print("VAL=");
  //Serial.println(val);

  if (val < SOIL_WET)
    moist = 100;
  else if (val > SOIL_DRY)
    moist = 0;
  else
    moist = ((SOIL_DRY - val) * 100) / (SOIL_DRY - SOIL_WET);
    
  /*Serial.print(val);
  Serial.print(',');
  Serial.println(moist);*/
  
  return moist;
}

void getSensorData(sensor_data *sd)
{
  //BH1750FVI
  sd->lum = LightSensor.GetLightIntensity();
  
  // DS18B20
  DS18B20_1.requestTemperatures();
  DS18B20_2.requestTemperatures();
  sd->soil_temp_1 = DS18B20_1.getTempCByIndex(0) * 100;
  sd->soil_temp_2 = DS18B20_2.getTempCByIndex(0) * 100;

  // Soil moisture
  sd->soil_moist_1 = soil_moisture(SOIL_MOISTURE_1);
  sd->soil_moist_2 = soil_moisture(SOIL_MOISTURE_2);

  // BME280
  sd->air_hum = bme280.readFloatHumidity() * 100;
  sd->air_temp = bme280.readTempC() * 100;
  sd->air_pres = bme280.readFloatPressure();
}


void printSensorData(sensor_data sd)
{
  String s;
  
  //BH1750FVI
  Serial.println("*** BH1750FVI ***");
  Serial.println("\tL = " + (String)(sd.lum) + " lux");
  
  // DS18B20
  Serial.println("*** DS18B20 ***");
  Serial.println("\tT = " + (String)(sd.soil_temp_1 / 100) + "." + (String)(sd.soil_temp_1 % 100) + " C");
  Serial.println("\tT = " + (String)(sd.soil_temp_2 / 100) + "." + (String)(sd.soil_temp_2 % 100) + " C");

  // Soil moisture
  Serial.println("*** Soil moisture ***");
  Serial.println("\tH = " + (String)sd.soil_moist_1 + " %");
  Serial.println("\tH = " + (String)sd.soil_moist_2 + " %");

  // BME280
  Serial.println("*** BME280 ***");
  Serial.println("\tH = " + String(sd.air_hum / 100) + "." + (String)(sd.air_hum % 100) + " \%");
  Serial.println("\tT = " + String(sd.air_temp / 100) + "." + (String)(sd.air_temp % 100) + " C");
  Serial.println("\tP = " + String(sd.air_pres / 100) + "." + (String)(sd.air_pres % 100) + " mBar");
}
