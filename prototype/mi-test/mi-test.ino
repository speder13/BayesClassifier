#include "defines.h"
#include "motor_api.h"
#include "distance_sensor_api.h"

Motor motor_conveyor, motor_feeder;
Ultra_Sound_Sensor distance_sensor;

int32_t distance_to_wall;

void setup()
{
    motor_init(&motor_conveyor, 0.36, MOTOR_CONVEYOR_PIN, 
        MOTOR_CONVEYOR_INT_PIN, motor_conveyor_interrupt);
    motor_init(&motor_feeder, 1.0, MOTOR_FEEDER_PIN, MOTOR_FEEDER_INT_PIN, 
        motor_feeder_interrupt);
    distance_sensor_init(&distance_sensor, RANGE_TRIG, RANGE_ECHO);
    
    motor_turn(&motor_conveyor);
    distance_to_wall = calibrate_ultra_sound_sensor();
}

void loop()
{
    int32_t test_dist = distance_sensor_measure_distance(&distance_sensor);
    if (test_dist >= distance_to_wall)
    {
        delay(100);
        return;
    }
    uint32_t pos = motor_get_degrees(&motor_conveyor);
    // Change TARGET1 for delay before requesting image
    while(pos + TARGET1 > motor_get_degrees(&motor_conveyor)) {}
    Serial.print("b");
    // Change TARGET2 for delay after requesting image
    while(pos + TARGET2 > motor_get_degrees(&motor_conveyor)) {}
    feed_ball();
}

void motor_conveyor_interrupt()
{
    motor_update_degrees(&motor_conveyor);
}

void motor_feeder_interrupt()
{
    motor_update_degrees(&motor_feeder);
}

void feed_ball() 
{
  const uint8_t feed_iteration = 8;
  static uint8_t feed_counter = feed_iteration;
  static int16_t deg = 90;
  
  // We only feed a ball every x iterations
  if (feed_counter == feed_iteration) 
  {
    motor_turn_to_degree(&motor_feeder, deg);

    deg += 90;
    if (deg == 360) 
      deg = 0;

    feed_counter = 0;
  }
  
  feed_counter++;
}

int32_t calibrate_ultra_sound_sensor()
{
  int32_t min = distance_sensor_measure_distance(&distance_sensor);
  int32_t current;
  for (uint8_t i = 0; i < CALIBRACTION_ITERATIONS - 1; i++)
  {
    DEBUG_PRINTLN_VAR(min);
    current = distance_sensor_measure_distance(&distance_sensor);

    if (current < min)
      min = current;
  }

  return min;
}