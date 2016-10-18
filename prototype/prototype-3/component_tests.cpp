#include "component_tests.h"

#ifdef COMPONENT_TEST_MOTOR_COAST
void component_test_motor_coast(Motor* motor) {
  for (int i = 0; i < 20; i++) {
    motor->base.deg = 0;
    motor_turn_deg(motor, 360);

    delay(1000);
    Serial.println(motor_get_degrees(motor)); // Value used for component testing.
    delay(500);                               // Should be 360 + coast degrees for the motor.
  }
}
#endif

#ifdef COMPONENT_TEST_MOTOR_TIME
void component_test_motor_time(Motor* motor) {
  for (int i = 0; i < 20; i++) {
    motor->deg = 0;
    unsigned long t_start = millis();
    motor_turn_deg(motor, 360);
    unsigned long t_end = millis();

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


    motor_turn(&motor1);
    delay(moter_wait);
    
    interrupt_count_start = motor_get_degrees(&motor1);
    time_start = micros();
    for (volatile long j = 0; j < iterations; j++);
    time_end = micros();
    interrupt_count_end = motor_get_degrees(&motor1);;

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
