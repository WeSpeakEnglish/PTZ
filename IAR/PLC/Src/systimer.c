#include "systimer.h"
#include "variables.h"

volatile uint32_t timestamp = 0;
volatile uint32_t UNIXTime = 0;


// 1 kHz timer
void HAL_SYSTICK_Callback(void){
timestamp++;
if(!(timestamp % 999))UNIXTime++;
}