#ifndef _SENSORS_H
#define _SENSORS_H

typedef struct
{
  //air parameters
  int16_t air_temp;
  int16_t air_hum;
  int32_t air_pres;
  //soil parameters
  int16_t soil_temp_1;
  int16_t soil_temp_2;
  int8_t soil_moist_1;
  int8_t soil_moist_2;
  //luminosity
  uint16_t lum;
} sensor_data;

void initSensors();
void getSensorData(sensor_data *sd);
void printSensorData(sensor_data sd);

#endif
