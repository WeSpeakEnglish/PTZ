#include "timer11.h"
#include "tim.h"
#include "rtc.h"
#include "initial.h"
#include "core.h"
#include "gui.h"
#include "lcd.h"
#include "keyboard.h"
#include "sound.h"
#include "initial.h"



// 
void Timer11_Init(void){
  TIM11->PSC = 21600;
  TIM11->ARR = 1000; //10 times in one second
  TIM11->DIER |= TIM_DIER_UIE; //разрешаем прерывание от таймера
  TIM11->CR1 |= TIM_CR1_CEN; // Начать отсчёт!
  NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
return;
}

void TIM11_IRQHandler(void){
static uint32_t Counter = 0;

if(Counter%8 == 0) Test1();
Test2();

UpdateScreen = 1;
 
Counter++; 
 return;
}