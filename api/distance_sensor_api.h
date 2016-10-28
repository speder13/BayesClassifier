#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include "Arduino.h"
#include <stdint.h>

/*
 * Read the distance on the ultra sound sensor
 */
int32_t get_range(uint8_t trigger_pin, uint8_t echo_pin);

#endif
