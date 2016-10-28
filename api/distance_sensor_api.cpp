#include "distance_sensor_api.h"

int32_t get_range(uint8_t trigger_pin, uint8_t echo_pin)
{
    // Ping the ultra sound sensor
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    // Get the time until the sound returns, and convert that time to a distance
    // using the speed of sound
    return (pulseIn(echo_pin, HIGH)/2) * 0.034;
}
