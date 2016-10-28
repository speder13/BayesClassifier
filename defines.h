// Motor 2: Control conveyor belt
// Motor 3: Control feeder
// Motor 4: Control separator

#define MOTOR_CONVEYOR_PIN 6            // Power conveyor motor
#define MOTOR_FEEDER_PIN 52             // Power feeder motor
#define MOTOR_SEPARATOR_PIN1 22         // Control separator motor direction
#define MOTOR_SEPARATOR_PIN2 24         // Control separator motor direction

#define MOTOR_CONVEYOR_INT_PIN 18       // Interrupt conveyor motor
#define MOTOR_FEEDER_INT_PIN 3          // Interrupt feeder motor
#define MOTOR_SEPARATOR_INT_PIN1 2      // Interrupt separator motor
#define MOTOR_SEPARATOR_INT_PIN2 31     // Used to read direction, not an actual 
                                        // interrupt

#define BUTTON_INT_PIN 21               // Emergency stop int port

#define RANGE_ECHO 32                   // Read distance
#define RANGE_TRIG 33                   // Start the sensor

#define QUEUE_SIZE 6                    // Size of the conveyor queue