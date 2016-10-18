#include "motor_api.h"


//#define API_TEST_MOTOR_TURN_DEG
//#define KILLALL // Uncomment this to kill the system, and stop all components

#define MOTOR_1_PIN 4
#define MOTOR_2_PIN 11
#define MOTOR_3_PIN

#define MOTOR_1_INT_PIN 2
#define MOTOR_2_INT_PIN 2
#define MOTOR_3_INT_PIN

Motor motor1, motor2;
Advanced_Motor motor3;

void setup() {
#ifdef KILLALL
  exit(0);
#endif

  motor1.pin = MOTOR_1_PIN;
  motor2.pin = MOTOR_2_PIN;
  //motor3.pin = MOTOR_3_PIN;

  Serial.begin(9200);

  pinMode(MOTOR_1_INT_PIN, INPUT_PULLUP);
  //pinMode(MOTOR_2_INT_PIN, INPUT_PULLUP);
  //pinMode(MOTOR_3_INT_PIN, INPUT_PULLUP);

  pinMode(motor1.pin, OUTPUT);
  //pinMode(motor2.pin, OUTPUT);
  //pinMode(motor3.pin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(MOTOR_1_INT_PIN), motor1_interrupt, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(MOTOR_2_INT_PIN), motor2_interrupt, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(MOTOR_3_INT_PIN), motor3_interrupt, CHANGE);

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

  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);

  delay(5000);
  
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  delay(50);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
}

void loop() {
}

void motor1_interrupt() {
  motor1.deg += motor1.dir;
}

void motor2_interrupt() {
  motor2.deg += motor2.dir;
}

void motor3_interrupt() {
  motor3.deg += motor3.dir;
}


