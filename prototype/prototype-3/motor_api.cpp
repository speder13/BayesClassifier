#include "motor_api.h"

/************************
 * non public functions *
 ************************/
long base_motor_get_degrees(Base_Motor *motor) 
{
  long res;
  
  noInterrupts();
  res = motor->deg;
  interrupts();

  return res;
}

/********************
 * public functions *
 ********************/
void motor_init(Motor *motor, byte pin, byte interrupt_pin, void (*interrupt_handler)(void)) 
{
  motor->pin = pin;
  
  pinMode(interrupt_pin, INPUT_PULLUP);
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), interrupt_handler, CHANGE);
}

void advanced_motor_init(Advanced_Motor *motor, byte pin1, byte pin2, 
                         byte interrupt_pin1, byte interrupt_pin2, 
                         void (*interrupt_handler1)(void))
{
  motor->pin1 = pin1;
  motor->pin2 = pin2;
  motor->interrupt_pin1 = interrupt_pin1;
  motor->interrupt_pin2 = interrupt_pin2;
  
  pinMode(interrupt_pin1, INPUT_PULLUP);
  pinMode(interrupt_pin2, INPUT_PULLUP);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin1), interrupt_handler1, CHANGE);
}

void advanced_motor_turn_deg(Advanced_Motor* motor, int deg, Turning_Direction dir)
{
  long goal = advanced_motor_get_degrees(motor) + (deg * dir);

  advanced_motor_turn(motor, dir);

  switch (dir) {
    case FORWARD:
      while (goal < advanced_motor_get_degrees(motor));
      break;
    case BACKWARD:
      while (goal > advanced_motor_get_degrees(motor));
      break;
  }
  
  advanced_motor_stop(motor);
}

void motor_turn_deg(Motor* motor, int deg)
{
  int goal = motor_get_degrees(motor) + (deg);

  motor_turn(motor);
  while (goal > motor_get_degrees(motor));
  motor_stop(motor);
}

void advanced_motor_stop(Advanced_Motor* motor)
{
  //long stop_deg = advanced_motor_get_degrees(motor);

  switch(motor->dir)
  {
    case FORWARD:
      advanced_motor_turn(motor, BACKWARD);
      while (motor->dir != BACKWARD);
      break;  
    case BACKWARD:
      advanced_motor_turn(motor, FORWARD);
      while (motor->dir != FORWARD);
      break;
  }
  digitalWrite(motor->pin1, LOW);
  digitalWrite(motor->pin2, LOW);
  
  /*
  enum Turning_Direction {
  FORWARD    = -1,
  BACKWARD  = 1
};

  */
  /*
  do {
    deg_traveled = stop_deg - advanced_motor_get_degrees(motor);
    Serial.println(analog);

    if (deg_traveled > 0) {

      advanced_motor_turn(motor, BACKWARD);
      while(stop_deg > advanced_motor_get_degrees(motor));

    } else if (deg_traveled < 0) {
      
      advanced_motor_turn(motor, FORWARD);
      while(stop_deg < advanced_motor_get_degrees(motor));
    }

    digitalWrite(motor->pin1, LOW);
    digitalWrite(motor->pin2, LOW);
    delay(200);
  } while (stop_deg != advanced_motor_get_degrees(motor));

  digitalWrite(motor->pin1, LOW);
  digitalWrite(motor->pin2, LOW);
  */
}

void motor_stop(Motor* motor)
{
  digitalWrite(motor->pin, LOW);
}

void advanced_motor_turn(Advanced_Motor *motor, Turning_Direction dir) 
{
  advanced_motor_turn_analog(motor, dir, 255);
}

void advanced_motor_turn_analog(Advanced_Motor *motor, Turning_Direction dir, byte value) {
  switch(dir)
  {
    case FORWARD:
      analogWrite(motor->pin1, value);
      digitalWrite(motor->pin2, LOW);
      break;
    case BACKWARD:
      digitalWrite(motor->pin1, LOW);
      analogWrite(motor->pin2, value);
      break;
    default:
      Serial.println("Not supported direction!");
      delay(1000);
      exit(0);
      break;
  }
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




