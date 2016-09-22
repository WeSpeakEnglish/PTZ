#include "systimer.h"
#include "variables.h"

volatile uint32_t timestamp = 0;

// 1 kHz timer
void HAL_SYSTICK_Callback(void){

timestamp++;

}