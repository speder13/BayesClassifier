#ifndef MOTOR_API_H
#define MOTOR_API_H

#include "Arduino.h"

/*
 * the directions a motor can turn.
 * these are also used by an advanced motor to increment or derement the degrees
 */
enum Turning_Direction {
  FORWARD = -1,
  BACKWARD = 1
};

/*
typedef signed char Turning_Direction;
#define FORWARD -1
#define BACKWARD 1
*/

/*
 * the base data for all motor types
 */
struct Base_Motor {
  volatile long deg = 0;
  volatile float degree_ratio = 1;
  volatile long buffer = 0;
  volatile bool reading = false;
};

/*
 * the data for a motor
 */
struct Motor {
  Base_Motor base;
  byte pin;
};

/*
 * the data for an advanced motor.
 * an advanced motor can turn in both directions
 */
struct Advanced_Motor {
  Base_Motor base;
  byte pin1;
  byte pin2;
  byte interrupt_pin1;
  byte interrupt_pin2;
  volatile Turning_Direction dir;
};

/*
 * initializes a standard motor
 */
void motor_init(Motor *motor, float degree_ratio, byte pin, byte interrupt_pin, void (*interrupt_handler)(void));

/*
 * initializes an advanced motor
 */
void advanced_motor_init(Advanced_Motor *motor, float degree_ratio, byte pin1, byte pin2, 
                         byte interrupt_pin1, byte interrupt_pin2, 
                         void (*interrupt_handler1)(void));

/*
 * turns an advanced motor to an angle
 */
void advanced_motor_turn_to_deg(Advanced_Motor* motor, int deg);

/*
 * turns an advanced motor a number of degrees in a certin direction
 */
void advanced_motor_turn_deg(Advanced_Motor* motor, int deg, Turning_Direction dir);

/*
 * turns a motor to an angle
 */
void motor_turn_to_deg(Motor* motor, int deg);

/*
 * turns a motor a number of degrees
 */
void motor_turn_deg(Motor* motor, int deg);

/*
 * tries to stop an advanced motor with minimum coasting
 */
void advanced_motor_stop(Advanced_Motor* motor);

/*
 * stops a motor
 */
void motor_stop(Motor* motor);

/*
 * make an advanced motor turn in a certin direction by an analog value
 */
void advanced_motor_turn_analog(Advanced_Motor *motor, Turning_Direction dir, byte value);

/*
 * make an advanced motor turn in a certin direction
 */
void advanced_motor_turn(Advanced_Motor *motor, Turning_Direction dir);

/*
 * make a motor turn by an analog value
 */
void motor_turn_analog(Motor *motor, byte value);

/*
 * make a motor turn
 */
void motor_turn(Motor* motor);

/*
 * safely retriving(locking) the degrees from a motor
 */
long motor_get_degrees(Motor *motor);

/*
 * safely retriving(locking) the degrees from an advanced motor
 */
long advanced_motor_get_degrees(Advanced_Motor *motor);

/*
 * updates the degree value on a motor. 
 * should either be scheduled or attached to an interrupt
 */
void motor_update_degrees(Motor* motor);

/*
 * updates the degree and direction value on an advanced motor. 
 * should either be scheduled or attached to an interrupt
 */
void advanced_motor_update_degrees(Advanced_Motor* motor);

#endif