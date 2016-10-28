#include "defines.h"

#include "motor_api.h"

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

  motor_turn_analog(&motor_conveyor, 255);
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


