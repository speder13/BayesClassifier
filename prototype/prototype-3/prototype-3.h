#ifndef _PROTOTYPE_3_H_
#define _PROTOTYPE_3_H_

enum Ball_Color 
{
  GREEN, 
  YELLOW, 
  RED, 
  BLUE, 
  EMPTY
};

struct Segment_Queue 
{
  Ball_Color data[QUEUE_SIZE] = { EMPTY };
  uint8_t index = 0;
};

Motor motor_conveyor, motor_feeder;
Advanced_Motor adv_motor_separator;
volatile bool running = true;
volatile bool stopped = false;
int32_t distance_to_wall;

#endif // _PROTOTYPE_3_H_