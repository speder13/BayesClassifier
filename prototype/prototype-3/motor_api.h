#ifndef MOTOR_API_H
#define MOTOR_API_H

#include "Arduino.h"

enum Turning_Direction {
  CLOCKWISE         = -1,
  COUNTERCLOCKWISE  = 1
};

struct Motor {
  byte pin = 0;
  volatile long deg = 0;
  Turning_Direction dir = COUNTERCLOCKWISE;
};

struct Advanced_Motor {
  byte pin1 = 0;
  byte pin2 = 0;
  volatile long deg = 0;
  Turning_Direction dir = COUNTERCLOCKWISE;
};

void motor_turn_deg(Motor* motor, int deg, Turning_Direction dir);

void motor_stop(Motor* motor);

void motor_turn(Motor* motor, Turning_Direction dir);

#endif
