#define COMPONENT_TEST_MOTOR_COAST
//#define COMPONENT_TEST_MOTOR_TIME
//#define COMPONENT_TEST_INTERRUPT_COST
//#define API_TEST_MOTOR_TURN_DEG
//#define KILLALL // Uncomment this to kill the system, and stop all components

#define MOTOR_1_PIN 4
#define MOTOR_2_PIN 11
#define MOTOR_3_PIN

#define MOTOR_1_INT_PIN 2
#define MOTOR_2_INT_PIN 2
#define MOTOR_3_INT_PIN

enum Turning_Direction {
  CLOCKWISE         = -1,
  COUNTERCLOCKWISE  = 1
};

struct Motor {
  int pin = 0;
  volatile long deg = 0;
  byte lock = 0;
  Turning_Direction dir = COUNTERCLOCKWISE;
};

struct Advanced_Motor {
  Motor base;
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

/*******************************
   COMPONENT TESTING FUNCTIONS
 *******************************/
#ifdef COMPONENT_TEST_MOTOR_COAST
void component_test_motor_coast(Motor* motor) {
  for (int i = 0; i < 20; i++) {
    motor->deg = 0;
    motor_turn_deg(motor, 360, COUNTERCLOCKWISE);

    delay(1000);
    Serial.println(motor->deg); // Value used for component testing.
    delay(500);                 // Should be 360 + coast degrees for the motor.
  }
}
#endif

#ifdef COMPONENT_TEST_MOTOR_TIME
void component_test_motor_time(Motor* motor) {
  for (int i = 0; i < 20; i++) {
    motor->deg = 0;
    unsigned long t_start = millis();
    motor_turn_deg(motor, 360, COUNTERCLOCKWISE);
    unsigned long t_end = millis();
    if (motor->deg < 360) {
      Serial.println("ERROR");
      exit(0);
    }

    Serial.println(t_end-t_start);
    delay(1000);
  }
}
#endif

#ifdef COMPONENT_TEST_INTERRUPT_COST
void print_formatted_number(long number, int total_size){
  bool leading_zeros = true;
  
  for (; total_size > 0; total_size--){
    long power = power_function(10, total_size - 1);
    long digit = number / power;

    
    if (leading_zeros && digit != 0)
      leading_zeros = false;

    if (leading_zeros)
      Serial.print(" ");
    else{
      Serial.print(digit);
    }
    
    number -= digit * power;
  }
}

long power_function(long number, long expo){
  if (expo == 0)
    return 1;

  long res = number;
  
  for (; expo > 1; expo--){
    res *= number; 
  }

  return res;
}

void print_table_row(long with, long without, long delta, long count, double est_cost, double est_cost_percent){
  Serial.print("| ");
  print_formatted_number(with, 10);
  Serial.print(" | ");
  print_formatted_number(without, 10);
  Serial.print(" | ");
  print_formatted_number(delta, 10);
  Serial.print(" | ");
  print_formatted_number(count, 10);
  Serial.print(" | ");
  Serial.print(est_cost, 8);
  Serial.print(" | ");
  Serial.print(est_cost_percent, 7);
  Serial.println("% |");
}


void component_test_interupt_cost(){
  const int test_count = 50;
  const long moter_wait = 1500;
  const long iterations = 100000;

  long time_start, time_end, interrupt_count_start, interrupt_count_end, 
       no_interrupt_time, interrupt_time, interrupt_count, time_delta,
       average_no_interrupt_time = 0, average_interrupt_time = 0, 
       average_interrupt_count = 0;
  
  Serial.println("| with       | without    | delta      | count      | est cost   | est cost % |");
  for (int i = 0; i < test_count + 1; i++){
    time_start = micros();
    for (volatile long j = 0; j < iterations; j++);
    time_end = micros();
  
    no_interrupt_time = time_end - time_start;


    motor_turn(&motor1, COUNTERCLOCKWISE);
    delay(moter_wait);
    
    interrupt_count_start = motor1.deg;
    time_start = micros();
    for (volatile long j = 0; j < iterations; j++);
    time_end = micros();
    interrupt_count_end = motor1.deg;

    interrupt_time = time_end - time_start;
    interrupt_count = interrupt_count_end - interrupt_count_start;
    
    motor_stop(&motor1);

  if (i != 0){
    time_delta = interrupt_time - no_interrupt_time;

    print_table_row(interrupt_time, no_interrupt_time, time_delta, interrupt_count,
                    time_delta / (double)interrupt_count, time_delta / (double)interrupt_time);

    average_interrupt_time += interrupt_time;
    average_no_interrupt_time += no_interrupt_time;
    average_interrupt_count += interrupt_count;
  }
    
    delay(moter_wait);
  }

  average_no_interrupt_time /= test_count;
  average_interrupt_time /= test_count;
  average_interrupt_count /= test_count; 
  time_delta = average_interrupt_time - average_no_interrupt_time;
  
  Serial.println("-------------------------------------------------------------------------------");
  print_table_row(average_interrupt_time, average_no_interrupt_time, time_delta, average_interrupt_count,
                  time_delta / (double)average_interrupt_count, time_delta / (double)average_interrupt_time);


  return;
  /*
  long time_start, time_end, interrupt_count_start, interrupt_count_end;
  long no_interrupt_times[test_count], interrupt_times[test_count], interrupt_counts[test_count];
  
  delay(2500); //HACK: this makes it so micros() gives us currect result on first iteration for some reason...
  // run measurements without a motor running
  for (int i = 0; i < test_count; i++){  
    time_start = micros();
    for (volatile long j = 0; j < iterations; j++);
    time_end = micros();
  
    no_interrupt_times[i] = time_end - time_start;
  }
  // run measurements with a motor running
  motor_turn(&motor1, COUNTERCLOCKWISE);
  delay(2500);
  for (int i = 0; i < test_count; i++){
    interrupt_count_start = motor1.deg;
    time_start = micros();
    for (volatile long j = 0; j < iterations; j++);
    time_end = micros();
    interrupt_count_end = motor1.deg;

    interrupt_times[i] = time_end - time_start;
    interrupt_counts[i] = interrupt_count_end - interrupt_count_start;
  }
    
  motor_stop(&motor1);
  
  long average_no_interrupt_time = 0, average_interrupt_time = 0, average_interrupt_count = 0, delta;

  // print result table
  Serial.println("| with       | without    | delta      | count      | est cost   | est cost % |");
  for (int i = 0; i < test_count; i++){
    delta = interrupt_times[i] - no_interrupt_times[i];
    print_table_row(interrupt_times[i], no_interrupt_times[i], delta, interrupt_counts[i],
                    delta / (double)interrupt_counts[i], delta / (double)interrupt_times[i]);

    average_interrupt_time += interrupt_times[i];
    average_no_interrupt_time += no_interrupt_times[i];
    average_interrupt_count += interrupt_counts[i];
  }

  average_no_interrupt_time /= test_count;
  average_interrupt_time /= test_count;
  average_interrupt_count /= test_count; 
  delta = average_interrupt_time - average_no_interrupt_time;
  
  Serial.println("-------------------------------------------------------------------------------");
  print_table_row(average_interrupt_time, average_no_interrupt_time, delta, average_interrupt_count,
                  delta / (double)average_interrupt_count, delta / (double)average_interrupt_time);
  */
}
#endif
