#include "distance_sensor_api.h"

long getRange(int triggerPin, int echoPin){
    long duration, distance;

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distance = (duration/2) * 0.0034;

    return distance;
}
