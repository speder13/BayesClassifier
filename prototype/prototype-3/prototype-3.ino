#include "defines.h"

#include "motor_api.h"
#include "distance_sensor_api.h"

enum Ball_Color 
{
  GREEN, 
  YELLOW, 
  RED, 
  BLUE, 
  EMPTY
};

int32_t default_dist = 0;

Motor motor_conveyor, motor_feeder;
Advanced_Motor adv_motor_separator;

void setup() 
{
  //exit(0);
  Serial.begin(9200);

  // Init hardware components, sensors and actuators
  pinMode(RANGE_ECHO, INPUT);
  pinMode(RANGE_TRIG, OUTPUT);
  
  button_init(BUTTON_INT_PIN, instant_stop_interrupt);

  motor_init(&motor_conveyor, 0.36, MOTOR_CONVEYOR_PIN, MOTOR_CONVEYOR_INT_PIN, 
    motor_conveyor_interrupt);
  motor_init(&motor_feeder, 1.0, MOTOR_FEEDER_PIN, MOTOR_FEEDER_INT_PIN, 
    motor_feeder_interrupt);

  advanced_motor_init(&adv_motor_separator, 1.0, MOTOR_SEPARATOR_PIN1, 
    MOTOR_SEPARATOR_PIN2,MOTOR_SEPARATOR_INT_PIN1, MOTOR_SEPARATOR_INT_PIN2, 
    adv_motor_separator_interrupt1);

  motor_turn_analog(&motor_conveyor, 255);
  default_dist = get_range(RANGE_TRIG, RANGE_ECHO);
}

void loop() 
{
  static int16_t bucket_pos [5] = { 0, 50, 100, 260, 310 };
  static Ball_Color segments[QUEUE_SIZE] = { EMPTY };
  static uint8_t segments_start = 0;
  static uint8_t segments_end = QUEUE_SIZE - 1;
  static Ball_Color last_ball = EMPTY;
  static int32_t conveyor_target = 90;
  static uint8_t feed_counter = 1;

  int32_t test_dist = get_range(RANGE_TRIG, RANGE_ECHO);

  // Tests if a ball is in front of sensor
  if (test_dist < default_dist-1)
  {
      //Serial.println(default_dist);
      //Serial.println(test_dist);
      Serial.println("ball found");
      segments[segments_end] = (Ball_Color)random(4);
  } 
  else 
  {
    segments[segments_end] = EMPTY;
  }
  
  segments_end++;
  if (segments_end >= QUEUE_SIZE)
  {
    segments_end = 0;
  }

  if (feed_counter == 1) 
  {
    feed_ball();
    feed_counter++;
  } 
  else 
  {
    feed_counter = 1;
  }

  Ball_Color current_ball = segments[segments_start];

  segments_start++;
  if (segments_start >= QUEUE_SIZE)
  {
    segments_start = 0;
  }

  if (current_ball != EMPTY && current_ball != last_ball)
  {
    Serial.print("ejecting: ");
    Serial.println(current_ball);
    advanced_motor_turn_to_degree(&adv_motor_separator, 
      bucket_pos[current_ball]);  
  }

  while(motor_get_degrees(&motor_conveyor) < conveyor_target);
  conveyor_target += 90;
}

void instant_stop_interrupt() 
{
  exit(0);
}

void motor_conveyor_interrupt() 
{
  motor_update_degrees(&motor_conveyor);
}

void motor_feeder_interrupt() 
{
  motor_update_degrees(&motor_feeder);
}

void adv_motor_separator_interrupt1() 
{
  advanced_motor_update_degrees(&adv_motor_separator);
  //Serial.println("int");
}

void feed_ball() 
{
  static int16_t deg = 90;
  
  motor_turn_to_degree(&motor_feeder, deg);
  deg+= 90;
  if (deg == 360) 
  {
    deg = 0;
  }
}

void button_init(int16_t pin, void(*func)(void))
{
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), func, HIGH);
}
