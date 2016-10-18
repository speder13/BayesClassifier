#include "motor_api.h"

void motor_turn_deg(Motor* motor, int deg, Turning_Direction dir)
{
  int goal = motor->deg + (deg * dir);
  bool error = false;
  bool waiting = true;

  motor_turn(motor, dir);

  switch (motor->dir) {
    case CLOCKWISE:
      do {
        noInterrupts();
        waiting = goal < motor->deg;
        interrupts();
      } while (waiting);

      error = goal < motor->deg;
      break;
    case COUNTERCLOCKWISE:
      do {
        noInterrupts();
        waiting = goal > motor->deg;
        interrupts();
      } while (waiting);
      
      error = goal > motor->deg;
      break;
    default:
      error = true;
      break;
  }
  
  motor_stop(motor);

  if (error) { 
    Serial.println("ERROR: motor_turn_deg"); 
    Serial.print("goal: "); Serial.println(goal);
    Serial.print("motor->deg: "); Serial.println(motor->deg);
    Serial.print("dir: "); 
    
    switch (motor->dir) {
      case CLOCKWISE:
        Serial.println("CLOCKWISE");
        Serial.print("goal < motor->deg = "); Serial.println(error ? "true" : "false");
        break;
      case COUNTERCLOCKWISE:
        Serial.println("COUNTERCLOCKWISE");
        Serial.print("goal > motor->deg = "); Serial.println(error ? "true" : "false");
        break;
      default:
        Serial.println("NOT SUPPORTED DIR");
        break;
    }
    
    delay(1000);
    exit(0);        
  }
}

void motor_stop(Motor* motor)
{
  digitalWrite(motor->pin, LOW);
}

void motor_turn(Motor* motor, Turning_Direction dir)
{
  motor->dir = dir;
  digitalWrite(motor->pin, HIGH);
}
