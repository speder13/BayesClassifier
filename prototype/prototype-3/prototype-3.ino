#include "motor_api.h"
#include "component_tests.h"

//#define API_TEST_MOTOR_TURN_DEG
//#define KILLALL // Uncomment this to kill the system, and stop all components

#define MOTOR_1_PIN 4
#define MOTOR_2_PIN 11
#define MOTOR_3_PIN1 7
#define MOTOR_3_PIN2 8

#define MOTOR_1_INT_PIN 2
#define MOTOR_2_INT_PIN 2
#define MOTOR_3_INT_PIN1 2
#define MOTOR_3_INT_PIN2 6


Motor motor1, motor2;
Advanced_Motor motor3;

void setup() {
#ifdef KILLALL
  exit(0);
#endif
  Serial.begin(9200);

  /*
  motor1.pin = MOTOR_1_PIN;
  motor2.pin = MOTOR_2_PIN;
  motor3.pin1 = MOTOR_3_PIN1;
  motor3.pin2 = MOTOR_3_PIN2;

  motor_init(&motor1, MOTOR_1_PIN, MOTOR_1_INT_PIN, motor1_interrupt);
  motor_init(&motor2, MOTOR_2_PIN, MOTOR_2_INT_PIN, motor2_interrupt);
  */
/*
  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), instant_stop_interrupt, CHANGE);
  analogWrite(4, 50);

  advanced_motor_init(&motor3, MOTOR_3_PIN1, MOTOR_3_PIN2, 
                      MOTOR_3_INT_PIN1, MOTOR_3_INT_PIN2, 
                      motor3_interrupt1);

  advanced_motor_turn_deg(&motor3, 360, BACKWARD);
  delay(2000);
  Serial.println(motor3.base.deg);
  //advanced_motor_turn(&motor3, BACKWARDS);
  
  */

  advanced_motor_init(&motor3, MOTOR_3_PIN1, MOTOR_3_PIN2, 
                      MOTOR_3_INT_PIN1, MOTOR_3_INT_PIN2, 
                      motor3_interrupt1);
                      
#ifdef COMPONENT_TEST_MOTOR_COAST
  Serial.println("Component Test: Motor Coast");
  component_test_motor_coast(&motor1);
#endif

#ifdef COMPONENT_TEST_MOTOR_TIME
  Serial.println("Component Test: Motor Time");
  component_test_motor_time(&motor1);
#endif

#ifdef COMPONENT_TEST_INTERRUPT_COST
  Serial.println("Component Test: Interupt Cost");
  component_test_interupt_cost(&motor3);
#endif

#ifdef API_TEST_MOTOR_TURN_DEG
  Serial.println("API Test: Test Motor Deg");
  api_test_motor_turn_deg();
#endif
}

void loop() {
}

void instant_stop_interrupt() {
  //exit(0);
}

void motor1_interrupt() {
  motor1.base.deg++;
}

void motor2_interrupt() {
  motor2.base.deg++;
}

void motor3_interrupt1() 
{
  byte pin1_res = digitalRead(motor3.interrupt_pin1);
  byte pin2_res = digitalRead(motor3.interrupt_pin2);

  switch (pin1_res) 
  {
    case LOW:
      motor3.dir = pin2_res == LOW ? FORWARD : BACKWARD;
      break;
    default:
      motor3.dir = pin2_res == LOW ? BACKWARD : FORWARD;
      break;

  }

  motor3.base.deg += motor3.dir;
}


