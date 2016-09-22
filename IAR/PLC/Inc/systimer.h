#ifndef SYSTIMER_H
#define SYSTIMER_H
#include "variables.h"
extern volatile uint32_t timestamp;

void HAL_SYSTICK_Callback(void);

#endif