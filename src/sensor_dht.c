#include "sensor_dht.h"
#include <sensor.h>
#include <gpio.h>

/* Get temperature values from DHT sensor*/
int on_get_temperature(void)
{
	struct sensor_value temp;

	sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
	temperature = temp.val1;
	LOG_INF("Temperature value: %d \n", temperature);

	return 1;
}

/* Get relative humidity values from DHT sensor*/
int on_get_humidity(void)
{
	struct sensor_value humidity;

	sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &humidity);
	relative_humidity = humidity.val1;
	LOG_INF("Relative humidity value: %d \n", relative_humidity);
	
	return 1;
}

/* Plot Datatime */
static const char *now_str(void)
{
	static char buf[16]; /* ...HH:MM:SS.MMM */
	u32_t now = k_uptime_get_32();
	unsigned int ms = now % MSEC_PER_SEC;
	unsigned int s;
	unsigned int min;
	unsigned int h;

	now /= MSEC_PER_SEC;
	s = now % 60U;
	now /= 60U;
	min = now % 60U;
	now /= 60U;
	h = now;

	snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u",
		 h, min, s, ms);
	return buf;
}