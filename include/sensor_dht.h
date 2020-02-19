
#ifndef _SENSOR_DHT_H_
#define _SENSOR_DHT_H_

#include <device.h>
#include <gpio.h>
#include <sensor.h>

/* Get temperature values from DHT sensor*/
int on_get_temperature(int id);

/* Get relative humidity values from DHT sensor*/
int on_get_humidity(int id);

/* Plot Datatime */
static const char *now_str(void);


