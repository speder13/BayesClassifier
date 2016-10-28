// Motor 2: Control conveyor belt
// Motor 3: Control feeder
// Motor 4: Control separator

#ifndef DEFINES_H
#define DEFINES_H
#include <stddef.h>

#define MOTOR_CONVEYOR_PIN 6            // Power conveyor motor
#define MOTOR_FEEDER_PIN 52             // Power feeder motor
#define MOTOR_SEPARATOR_PIN1 22         // Control separator motor direction
#define MOTOR_SEPARATOR_PIN2 24         // Control separator motor direction

#define MOTOR_CONVEYOR_INT_PIN 18       // Interrupt conveyor motor
#define MOTOR_FEEDER_INT_PIN 3          // Interrupt feeder motor
#define MOTOR_SEPARATOR_INT_PIN1 2      // Interrupt separator motor
#define MOTOR_SEPARATOR_INT_PIN2 31     // Used to read direction, not an actual 
                                        // interrupt

#define BUTTON_INT_PIN 19               // Emergency stop int port

#define RANGE_ECHO 32                   // Read distance
#define RANGE_TRIG 33                   // Start the sensor

#define QUEUE_SIZE 6                    // Size of the conveyor queue

#define CALIBRACTION_ITERATIONS 100

#define DEBUGGING 0

#if DEBUGGING
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#define DEBUG_PRINT_VAR(var) \
DEBUG_PRINT(#var);  \
DEBUG_PRINT(": ");  \
DEBUG_PRINT(var)

#define DEBUG_PRINTLN_VAR(var) DEBUG_PRINT_VAR(var); DEBUG_PRINTLN("")


#endif // DEFINES_H