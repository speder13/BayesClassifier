#include "motor_api.h"

/************************
 * non public functions *
 ************************/
long base_motor_get_degrees(Base_Motor* motor) 
{
  long res;

  // mark reading as true, so no race conditions occur
  motor->reading = true;
  res = motor->deg; 
  motor->reading = false;

  return res * motor->degree_ratio;
}

/********************
 * public functions *
 ********************/
void motor_init(Motor* motor, float degree_ratio, byte pin, byte interrupt_pin, 
  void (*interrupt_handler)(void)) 
{
  motor->pin = pin;
  motor->base.degree_ratio = degree_ratio;

  pinMode(interrupt_pin, INPUT_PULLUP);
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), interrupt_handler, 
    CHANGE);
}

void advanced_motor_init(Advanced_Motor* motor, float degree_ratio, byte pin1, 
  byte pin2, byte interrupt_pin1, byte interrupt_pin2, 
  void (*interrupt_handler1)(void))
{
  motor->pin1 = pin1;
  motor->pin2 = pin2;
  motor->interrupt_pin1 = interrupt_pin1;
  motor->interrupt_pin2 = interrupt_pin2;
  motor->base.degree_ratio = degree_ratio;

  pinMode(interrupt_pin1, INPUT_PULLUP);
  pinMode(interrupt_pin2, INPUT_PULLUP);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin1), interrupt_handler1, 
    CHANGE);
}

void advanced_motor_turn_to_deg(Advanced_Motor* motor, int deg) {
  // turn to degree should only accept a degree value between 
  // 0 and 359 inclusive
  if (deg < 0 || deg >= 360) {
    Serial.print("advanced_motor_turn_to_deg was supplied ");
    Serial.print("with an invalid degree: ");
    Serial.println(deg);
    return;
  }

  long current_pos = advanced_motor_get_degrees(motor);
  long turns = (current_pos / 360);

  // we need to choose two goal, one above and one below our current_pos.
  // these goal needs to be the full number representaion of the degree number 
  // that has to be reached.
  long goal2, goal1 = 360 * turns + deg;
  int distance_forward, distance_backward;

  if (goal1 > current_pos) {
    goal2 = 360 * (turns - 1) + deg;
    distance_forward = current_pos - goal2;
    distance_backward = goal1 - current_pos;
  } else if (goal1 < current_pos) {
    goal2 = 360 * (turns + 1) + deg;
    distance_forward = current_pos - goal1;
    distance_backward = goal2 - current_pos;
  } else {
    return;
  }
  
  // choose which direction to turn based on which distance is shortest
  if (distance_forward < distance_backward) {
    Serial.println(distance_forward);
    advanced_motor_turn_deg(motor, distance_forward, FORWARD);
  } else {
    Serial.println(distance_backward);
    advanced_motor_turn_deg(motor, distance_backward, BACKWARD);
  }
}

void motor_turn_to_deg(Motor* motor, int deg) {
  // turn to degree should only accept a degree value between 
  // 0 and 359 inclusive
  if (deg < 0 || deg >= 360) {
    Serial.print("motor_turn_to_deg was supplied with an invalid degree: ");
    Serial.println(deg);
    return;
  }

  long current_pos = motor_get_degrees(motor);
  long turns = (current_pos / 360);
  long goal = 360 * turns + deg;

  // if goal is lower than current_pos, then we need to choose a new goal, 
  // 360 degrees bigger
  if (goal < current_pos) {
    goal = 360 * (turns + 1) + deg;
  }

  // turn motor by the distance to the goal
  motor_turn_deg(motor, goal - current_pos);
}

void advanced_motor_turn_deg(Advanced_Motor* motor, int deg, 
  Turning_Direction dir)
{
  // calculate the goal that the motor should reach
  long goal = advanced_motor_get_degrees(motor) + (deg * dir);

  advanced_motor_turn(motor, dir);

Serial.println(goal);

  // wait for the motor to reach the goal
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
  // calculate the goal that the motor should reach
  int goal = motor_get_degrees(motor) + (deg);

  motor_turn(motor);

  // wait for the motor to reach the goal
  while (goal > motor_get_degrees(motor));

  motor_stop(motor);
}

void advanced_motor_stop(Advanced_Motor* motor)
{
  // HACK: to break, we turn the motor in the other direction for a 
  // fix amount of time.
  // It is not perfect, but it does reduce the coasting by a lot
  if (motor->dir == FORWARD) 
  {
      advanced_motor_turn(motor, BACKWARD);
  } 
  else 
  {
      advanced_motor_turn(motor, FORWARD);
  }
  delay(50); // 50ms seems like a good delay
  digitalWrite(motor->pin1, LOW);
  digitalWrite(motor->pin2, LOW);
  delay(100); // give the motor time to stop coasting
}

void motor_stop(Motor* motor)
{
  digitalWrite(motor->pin, LOW);
}

void advanced_motor_turn(Advanced_Motor* motor, Turning_Direction dir) 
{
  switch(dir)
  {
    case FORWARD:
      digitalWrite(motor->pin1, HIGH);
      digitalWrite(motor->pin2, LOW);
      break;
    case BACKWARD:
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

void advanced_motor_turn_analog(Advanced_Motor* motor, Turning_Direction dir, 
  byte value) 
{
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

void motor_turn_analog(Motor* motor, byte value)
{
  analogWrite(motor->pin, value);
}

long motor_get_degrees(Motor* motor) 
{
  return base_motor_get_degrees(&motor->base);
}

long advanced_motor_get_degrees(Advanced_Motor* motor) 
{
  return base_motor_get_degrees(&motor->base);
}

void motor_update_degrees(Motor* motor) {
  // if degrees are being read, update a buffer instead to avoid race condtions
  if (motor->base.reading) {
    motor->base.buffer++;
  } else {
    // we need to add and reset the buffer when we are allowed to 
    // write to the degrees
    motor->base.deg += 1 + motor->base.buffer;
    motor->base.buffer = 0;
  }
}

void advanced_motor_update_degrees(Advanced_Motor* motor) 
{
  byte pattern = digitalRead(motor->interrupt_pin1);
  pattern |= (digitalRead(motor->interrupt_pin2) << 1);

  // we determin the direction by a pattern formed by the values 
  // read from pin1 and pin2
  // LOW = 0, HIGH = 1
  // 00 and 11 = FORWARD
  // 01 and 10 = BACKWARD
  switch (pattern) {
    case 0b11: case 0b00:
      motor->dir = FORWARD;
      break;
    case 0b10: case 0b01:
      motor->dir = BACKWARD;
      break;
    default:
      Serial.println("Error in determin direction of motor");
      break;
  }

  // if degrees are being read, update a buffer instead to avoid race condtions
  if (motor->base.reading) {
    motor->base.buffer += motor->dir;
  } else {
    // we need to add and reset the buffer when we are allowed to 
    // write to the degrees
    motor->base.deg += motor->dir + motor->base.buffer;
    motor->base.buffer = 0;
  }
}