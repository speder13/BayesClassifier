#include "distance_sensor_api.h"

void distance_sensor_init(Ultra_Sound_Sensor* distance_sensor, 
    uint8_t trigger_pin, uint8_t echo_pin)
{
    distance_sensor->trigger_pin = trigger_pin;
    distance_sensor->echo_pin = echo_pin;

    pinMode(echo_pin, INPUT);
    pinMode(trigger_pin, OUTPUT);
}

int32_t distance_sensor_measure_distance(Ultra_Sound_Sensor* distance_sensor)
{
    // Ping the ultra sound sensor
    digitalWrite(distance_sensor->trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(distance_sensor->trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(distance_sensor->trigger_pin, LOW);

    // Get the time until the sound returns, and convert that time to a distance
    // using the speed of sound
    return (pulseIn(distance_sensor->echo_pin, HIGH)/2) * 0.34;
}
