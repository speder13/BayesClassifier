#include "motor_api.h"

//#define API_TEST_MOTOR_TURN_DEG
//#define KILLALL // Uncomment this to kill the system, and stop all components

#define MOTOR_1_PIN 4
#define MOTOR_2_PIN 11
#define MOTOR_3_PIN1 7
#define MOTOR_3_PIN2 8

#define MOTOR_1_INT_PIN 2
#define MOTOR_2_INT_PIN 2
#define MOTOR_3_INT_PIN 2

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
  advanced_motor_init(&motor3, MOTOR_3_PIN1, MOTOR_3_PIN2, MOTOR_3_INT_PIN, motor3_interrupt);

  advanced_motor_turn_deg(&motor3, 360, BACKWARDS);
  
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
  component_test_interupt_cost();
#endif

#ifdef API_TEST_MOTOR_TURN_DEG
  Serial.println("API Test: Test Motor Deg");
  api_test_motor_turn_deg();
#endif
}

void loop() {
}

void motor1_interrupt() {
  motor1.base.deg++;
}

void motor2_interrupt() {
  motor2.base.deg++;
}

void motor3_interrupt() {
  motor3.base.deg += motor3.dir;
}


