//#define COMPONENT_TESTING_ROTATION_COAST
//#define KILLALL

#define MOTOR_1_PIN 12
#define MOTOR_2_PIN 11
#define MOTOR_3_PIN

#define MOTOR_1_INT_PIN 3
#define MOTOR_2_INT_PIN 2
#define MOTOR_3_INT_PIN

enum Turning_Direction {
  CLOCKWISE         = -1,
  COUNTERCLOCKWISE  = 1
};

struct Motor {
  int pin = 0;
  volatile int deg = 0;
  Turning_Direction dir = CLOCKWISE;
};

Motor motor1, motor2, motor3;

void setup() {
#ifdef KILLALL
  exit(0);
#endif

  motor1.pin = MOTOR_1_PIN;
  motor2.pin = MOTOR_2_PIN;
  //motor3.pin = MOTOR_3_PIN;

  Serial.begin(9200);

  pinMode(MOTOR_1_INT_PIN, INPUT_PULLUP);
  pinMode(MOTOR_2_INT_PIN, INPUT_PULLUP);
  //pinMode(MOTOR_3_INT_PIN, INPUT_PULLUP);

  pinMode(motor1.pin, OUTPUT);
  pinMode(motor2.pin, OUTPUT);
  //pinMode(motor3.pin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(MOTOR_1_INT_PIN), motor1_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR_2_INT_PIN), motor2_interrupt, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(MOTOR_3_INT_PIN), motor3_interrupt, CHANGE);

#ifdef COMPONENT_TESTING_ROTATION_COAST
  component_test_motor_coast(&motor1);
#endif
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

void motor_turn_deg(Motor* motor, int deg, Turning_Direction dir)
{
  int goal = motor->deg + (deg * dir);

  motor_turn(motor, dir);

  switch (motor->dir) {
    case CLOCKWISE:
      while (goal <= motor->deg) {
        delay(1);
      }
      break;
    case COUNTERCLOCKWISE:
      while (goal >= motor->deg) {
        delay(1);
      }
      break;
    default:
      Serial.println("NO ROTATION");
      break;
  }
  motor_stop(motor);
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

void loop() {
  // put your main code here, to run repeatedly:
}

/*****************************
   COMPONENT TESTING METHODS
 *****************************/

void component_test_motor_coast(Motor* motor) {
  for (int i = 0; i < 20; i++) {
    motor->deg = 0;
    motor_turn_deg(motor, 360, COUNTERCLOCKWISE);
    if (motor->deg < 360) {     // In case it happens that the loop breaks out before 360 degrees
      Serial.println("ERROR");  // thrown an error, because that is not supposed to happen.
      exit(0);                  // The error should be fixed, but cant be sure yet.
    }

    delay(1000);
    Serial.println(motor->deg); // Value used for component testing.
    delay(500);                 // Should be 360 + coast degrees for the motor.
  }
}
