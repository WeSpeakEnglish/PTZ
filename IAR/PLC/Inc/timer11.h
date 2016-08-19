#ifndef __TIMER11_H
#define __TIMER11_H

#include "variables.h"


extern uint32_t Counter_TIM11; // base for 13+ years overflow timer with 0.1 sec tick

void Timer11_Init(void);
void TIM11_IRQHandler(void);


#endif
