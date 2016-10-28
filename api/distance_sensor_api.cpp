#include "distance_sensor_api.h"

int32_t ping_and_get(uint8_t trigger_pin, uint8_t echo_pin)
{
    // Ping the ultra sound sensor
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    // Get the time until the sound returns, and convert that time to a distance
    // using the speed of sound
    return (pulseIn(echo_pin, HIGH)/2) * 0.34;
}

int32_t get_range(uint8_t trigger_pin, uint8_t echo_pin)
{
    int32_t distance1, distance2 = ping_and_get(trigger_pin, echo_pin);

    /*
    do
    {
        distance1 = distance2;
        distance2 = ping_and_get(trigger_pin, echo_pin);
    } while(distance1 != distance2);
    */

    return distance2;
}
