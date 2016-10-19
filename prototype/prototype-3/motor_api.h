#ifndef MOTOR_API_H
#define MOTOR_API_H

#include "Arduino.h"

enum Turning_Direction {
  FORWARD    = -1,
  BACKWARDS  = 1
};

struct Base_Motor {
  volatile long deg = 0;
};

struct Motor {
  Base_Motor base;
  byte pin;
};

struct Advanced_Motor {
  Base_Motor base;
  byte pin1;
  byte pin2;
  byte interrupt_pin1;
  byte interrupt_pin2;
  Turning_Direction dir;
};

void motor_init(Motor *motor, byte pin, byte interrupt_pin, void (*interrupt_handler)(void));
void advanced_motor_init(Advanced_Motor *motor, byte pin1, byte pin2, 
                         byte interrupt_pin1, byte interrupt_pin2, 
                         void (*interrupt_handler1)(void));

void advanced_motor_turn_deg(Advanced_Motor* motor, int deg, Turning_Direction dir);
void motor_turn_deg(Motor* motor, int deg);

void advanced_motor_stop(Advanced_Motor* motor);
void motor_stop(Motor* motor);

void advanced_motor_turn(Advanced_Motor *motor, Turning_Direction dir);
void motor_turn(Motor* motor);

long motor_get_degrees(Motor *motor);
long advanced_motor_get_degrees(Advanced_Motor *motor);

#endif