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

void button_init(int pin, void(*func)(void))
{
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), func, HIGH);
}

void setup() {
  //exit(0);
  Serial.begin(9200);

  // Init hardware components, sensors and actuators
  //pinMode(BUTTON_INT_PIN, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(BUTTON_INT_PIN), instant_stop_interrupt, HIGH);
  button_init(BUTTON_INT_PIN, instant_stop_interrupt);

  motor_init(&motor_conveyor, 0.36, MOTOR_CONVEYOR_PIN, MOTOR_CONVEYOR_INT_PIN, 
              motor_conveyor_interrupt);
  motor_init(&motor_feeder, 1.0, MOTOR_FEEDER_PIN, MOTOR_FEEDER_INT_PIN, motor_feeder_interrupt);

  advanced_motor_init(&adv_motor_separator, 1.0, MOTOR_SEPARATOR_PIN1, MOTOR_SEPARATOR_PIN2, 
                      MOTOR_SEPARATOR_INT_PIN1, MOTOR_SEPARATOR_INT_PIN2, 
                      adv_motor_separator_interrupt1);

#ifdef PROGRAM

motor_turn_analog(&motor_conveyor, 255);

/*
int feeder_destinations[4] {
  0, 90, 180, 270
};

  for (int i = 0; i < 10; i++ ){
    motor_turn_to_deg(&motor_feeder, feeder_destinations[i % 4]);
    Serial.println(i);
    delay(1000);
  }

  exit(0);
  motor_turn_analog(&motor_conveyor, 255);
  delay(2000);
  motor_turn_analog(&motor_conveyor, 185);
  motor_turn(&motor_feeder);
  */
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

int separator_destinations[5] {
  0, 50, 100, 260, 310
};

byte next_feeder_destination = 0;
long next_iteration = 360;
void loop() {
#ifdef PROGRAM

  Serial.println(advanced_motor_get_degrees(&adv_motor_separator));

  advanced_motor_turn_to_deg(&adv_motor_separator, separator_destinations[millis() % 5]);
  motor_turn_to_deg(&motor_feeder, (next_feeder_destination % 4) * 90);
  next_feeder_destination++;

  while (motor_get_degrees(&motor_conveyor) < next_iteration) ;
  next_iteration += 360;
  
#else
  Serial.println("Plz help!");
#endif
}

void instant_stop_interrupt() {
  exit(0);
}

void motor_conveyor_interrupt() {
  motor_update_degrees(&motor_conveyor);
}

void motor_feeder_interrupt() {
  motor_update_degrees(&motor_feeder);
}

void adv_motor_separator_interrupt1() 
{
  advanced_motor_update_degrees(&adv_motor_separator);
  //Serial.println("int");
}


