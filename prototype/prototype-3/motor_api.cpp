#include "motor_api.h"

// non public functions
long base_motor_get_degrees(Base_Motor *motor) 
{
  long res;
  
  noInterrupts();
  res = motor->deg;
  interrupts();

  return res;
}

void advanced_motor_turn_without_turning(Advanced_Motor *motor, Turning_Direction dir) 
{
  motor->dir = dir;
  
  switch(dir)
  {
    case FORWARD:
      digitalWrite(motor->pin1, HIGH);
      digitalWrite(motor->pin2, LOW);
      break;
    case BACKWARDS:
      digitalWrite(motor->pin1, LOW);
      digitalWrite(motor->pin2, HIGH);
      break;
    default:
      Serial.println("Not supported direction!");
      delay(1000);
      exit(0);
      break;
  }
}

// public functions
void motor_init(Motor *motor, byte pin, byte interrupt_pin, void (*interrupt_handler)(void)) 
{
  motor->pin = pin;
  
  pinMode(interrupt_pin, INPUT_PULLUP);
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), interrupt_handler, CHANGE);
}

void advanced_motor_init(Advanced_Motor *motor, byte pin1, byte pin2, byte interrupt_pin, void (*interrupt_handler)(void))
{
  motor->pin1 = pin1;
  motor->pin2 = pin2;
  
  pinMode(interrupt_pin, INPUT_PULLUP);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), interrupt_handler, CHANGE);
}

void advanced_motor_turn_deg(Advanced_Motor* motor, int deg, Turning_Direction dir)
{
  int goal = advanced_motor_get_degrees(motor) + (deg * dir);
  Turning_Direction next_dir;

  advanced_motor_turn(motor, dir);

  do {
    switch (motor->dir) {
      case FORWARD:
        while (goal < advanced_motor_get_degrees(motor));
        next_dir = BACKWARDS;
        break;
      case BACKWARDS:
        while (goal > advanced_motor_get_degrees(motor));
        next_dir = FORWARD;
        break;
    }
    
    advanced_motor_stop(motor);
    dir = next_dir;
  } while (goal != advanced_motor_get_degrees(motor));
  

  
}

void motor_turn_deg(Motor* motor, int deg)
{
  int goal = motor_get_degrees(motor) + (deg);
  bool error = false;

  motor_turn(motor);
  while (goal > motor_get_degrees(motor));
  motor_stop(motor);
}

void advanced_motor_stop(Advanced_Motor* motor)
{
  digitalWrite(motor->pin1, LOW);
  digitalWrite(motor->pin2, LOW);
  
  volatile long current_deg, prev_deg;

  do {
  
    prev_deg = advanced_motor_get_degrees(motor);
    delay(50);
    current_deg = advanced_motor_get_degrees(motor);
  } while(current_deg != prev_deg);
}

void motor_stop(Motor* motor)
{
  digitalWrite(motor->pin, LOW);
}

void advanced_motor_turn(Advanced_Motor *motor, Turning_Direction dir) 
{
  advanced_motor_turn_without_turning(motor, dir);
}

void motor_turn(Motor* motor)
{
  digitalWrite(motor->pin, HIGH);
}

long motor_get_degrees(Motor *motor) 
{
  return base_motor_get_degrees(&motor->base);
}

long advanced_motor_get_degrees(Advanced_Motor *motor) 
{
  return base_motor_get_degrees(&motor->base);
}




