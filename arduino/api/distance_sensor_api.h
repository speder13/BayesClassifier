#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include "Arduino.h"
#include <stdint.h>

/*
 * The data for a ultra sound distance sensor
 */
struct Ultra_Sound_Sensor 
{
    uint8_t trigger_pin;
    uint8_t echo_pin;
};

/*
 * Initialize a ultra sound sensor.
 */
void distance_sensor_init(Ultra_Sound_Sensor* distance_sensor, 
    uint8_t trigger_pin, uint8_t echo_pin);

/*
 * Read the distance on the ultra sound sensor
 */
int32_t distance_sensor_measure_distance(Ultra_Sound_Sensor* distance_sensor);

#endif // _DISTANCE_SENSOR_H_