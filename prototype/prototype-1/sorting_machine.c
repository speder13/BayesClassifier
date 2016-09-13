#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#include <stdint.h>

// DeclareTask(TaskLight);
// DeclareTask(TaskPing);
DeclareAlarm(AlarmLight);
DeclareEvent(EventLight);
DeclareCounter(SysTimerCnt);

void ecrobot_device_initialize(void) {
  nxt_motor_set_speed(NXT_PORT_A, 0, 1);
  ecrobot_init_nxtcolorsensor(NXT_PORT_S1, NXT_COLORSENSOR);
}

void ecrobot_device_terminate(void) {
  nxt_motor_set_speed(NXT_PORT_A, 0, 1);
  ecrobot_term_nxtcolorsensor(NXT_PORT_S1);
}

TASK(TaskLight) {
  nxt_motor_set_speed(NXT_PORT_A, -30, 1);
 
  while(1) {

    WaitEvent(EventLight);
    ClearEvent(EventLight);
    
    S16 rgb[3];
    ecrobot_get_nxtcolorsensor_rgb(NXT_PORT_S1, rgb);

    display_clear(0);
    display_goto_xy(0,0);
    display_int(rgb[0], 0);
    display_goto_xy(0,1);
    display_int(rgb[1], 0);
    display_goto_xy(0,2);
    display_int(rgb[2], 0);
    display_update();
  
    
  }
 
  TerminateTask();
}

TASK(TaskSort1) {
  systick_wait_ms(500);
}

TASK(TaskSort2){
  systick_wait_ms(500);
}

TASK(TaskPing) {
  SetRelAlarm(AlarmLight, 1, 100);
  while(1) {
    ecrobot_process_bg_nxtcolorsensor();
  }
}

void user_1ms_isr_type2(void) {
  (void)SignalCounter(SysTimerCnt);
}
