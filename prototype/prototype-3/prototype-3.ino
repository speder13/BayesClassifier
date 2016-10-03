enum Turning_Direction {
  CLOCKWISE        = -1,
  COUNTERCLOCKWISE = 1
};

struct Motor {
  int pin = 0;
  volatile int deg = 0;
  Turning_Direction dir = CLOCKWISE;
};

Motor motor1;

void setup() { 
  motor1.pin = 12;

  // put your setup code here, to run once:
  Serial.begin(9200);
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(18, INPUT);

  pinMode(motor1.pin, OUTPUT);

  //attachInterrupt(digitalPinToInterrupt(2), int2, CHANGE); // 0 is interrupt number, listens at pin 2
  attachInterrupt(digitalPinToInterrupt(3), motor1_interrupt, CHANGE); // 1 is interrupt number, listens at pin 3
  //attachInterrupt(digitalPinToInterrupt(18), button_clicked, CHANGE);

  //motor_turn(&motor1, CLOCKWISE);
  //motor_stop(&motor1);
  motor_turn_deg(&motor1, 360, CLOCKWISE);
}

void button_clicked() {
  Serial.println("I was clicked!");
  motor_stop(&motor1);
}

void int2() {
  // Serial.println("Int2");
}

void motor1_interrupt() {
  motor1.deg += motor1.dir;
}

void motor_turn_deg(Motor* motor, int deg, Turning_Direction dir) 
{
  int goal = motor->deg + (deg * dir);

  motor_turn(motor, dir);

  switch (motor->dir) {
    case CLOCKWISE:
      while (goal <= motor->deg) ;
      break;
    case COUNTERCLOCKWISE:
      while (goal >= motor->deg) ;
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
