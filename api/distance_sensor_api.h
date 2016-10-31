#ifndef _DISTANCE_SENSOR_H_
#define _DISTANCE_SENSOR_H_

#include "Arduino.h"
#include <stdint.h>

/*
 * Read the distance on the ultra sound sensor
 */
int32_t get_range(uint8_t trigger_pin, uint8_t echo_pin);

#endif // _DISTANCE_SENSOR_H_