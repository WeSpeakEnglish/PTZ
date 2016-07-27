#include "initial.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  NVIC_ClearPendingIRQ (EXTI0_IRQn);
  EXTI->PR |= (1<<0);
  Timer14_Init_Deal(350, 1);
  return;
}

void   EXTI9_5_IRQHandler(void)        //; EXTI Line 9..5
{
  TimeIsReady = 1;   
  EXTI->PR |= EXTI_PR_PR7; 
  NVIC_ClearPendingIRQ (EXTI9_5_IRQn);
  return;
}


