#include "motor_api.h"
#include "component_tests.h"

//#define API_TEST_MOTOR_TURN_DEG

// Motor 2: Control conveyor belt
// Motor 3: Control feeder
// Motor 4: Control separator

#define MOTOR_CONVEYOR_PIN 6          // Power conveyor motor
#define MOTOR_FEEDER_PIN 52           // Power feeder motor
#define MOTOR_SEPARATOR_PIN1 22       // Control separator motor direction
#define MOTOR_SEPARATOR_PIN2 24       // Control separator motor direction

#define MOTOR_CONVEYOR_INT_PIN 18   // Interrupt conveyor motor
#define MOTOR_FEEDER_INT_PIN 3      // Interrupt feeder motor
#define MOTOR_SEPARATOR_INT_PIN1 2  // Interrupt separator motor
#define MOTOR_SEPARATOR_INT_PIN2 31  // Used to read direction, not an actual interrupt

#define BUTTON_INT_PIN 21           // Emergency stop int port

Motor motor_conveyor, motor_feeder;
Advanced_Motor adv_motor_separator;

void setup() {
  Serial.begin(9200);

  // Init hardware components, sensors and actuators
  pinMode(BUTTON_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_INT_PIN), instant_stop_interrupt, HIGH);

  motor_init(&motor_conveyor, MOTOR_CONVEYOR_PIN, MOTOR_CONVEYOR_INT_PIN, 
              motor_conveyor_interrupt);
  motor_init(&motor_feeder, MOTOR_FEEDER_PIN, MOTOR_FEEDER_INT_PIN, motor_feeder_interrupt);

  advanced_motor_init(&adv_motor_separator, MOTOR_SEPARATOR_PIN1, MOTOR_SEPARATOR_PIN2, 
                      MOTOR_SEPARATOR_INT_PIN1, MOTOR_SEPARATOR_INT_PIN2, 
                      adv_motor_separator_interrupt1);

#ifdef PROGRAM
  // Start feeder and conveyor belt                
  motor_turn(&motor_conveyor);
  motor_turn(&motor_feeder);
#endif

  /*********************
  *  COMPONENT TESTS  *
  *********************/
  // Please do not put init code not related to tests below this

#ifdef COMPONENT_TEST_MOTOR_COAST
  Serial.println("Component Test: Motor Coast");
  component_test_motor_coast(&motor_conveyor);
#endif

#ifdef COMPONENT_TEST_MOTOR_TIME
  Serial.println("Component Test: Motor Time");
  component_test_motor_time(&motor_conveyor);
#endif

#ifdef COMPONENT_TEST_INTERRUPT_COST
  Serial.println("Component Test: Interupt Cost");
  component_test_interupt_cost(&adv_motor_separator);
#endif

#ifdef COMPONENT_TEST_MOTOR_DEGREE
  Serial.println("Component Test: Motor Degree");
  component_test_motor_degree(&adv_motor_separator);
#endif

//#ifdef API_TEST_MOTOR_TURN_DEG
//  Serial.println("API Test: Test Motor Deg");
//  api_test_motor_turn_deg();
//#endif
}

void loop() {
#ifdef PROGRAM
  Serial.println("No component test to tie me down!");
#else
  Serial.println("Plz help!");
#endif
}

void instant_stop_interrupt() {
  exit(0);
}

void motor_conveyor_interrupt() {
  motor_conveyor.base.deg++;
}

void motor_feeder_interrupt() {
  motor_feeder.base.deg++;
}

void adv_motor_separator_interrupt1() 
{
  byte pin1_res = digitalRead(adv_motor_separator.interrupt_pin1);
  byte pin2_res = digitalRead(adv_motor_separator.interrupt_pin2);

  switch (pin1_res) 
  {
    case LOW:
      adv_motor_separator.dir = pin2_res == LOW ? FORWARD : BACKWARD;
      break;
    default:
      adv_motor_separator.dir = pin2_res == LOW ? BACKWARD : FORWARD;
      break;
  }

  adv_motor_separator.base.deg += adv_motor_separator.dir;
}


