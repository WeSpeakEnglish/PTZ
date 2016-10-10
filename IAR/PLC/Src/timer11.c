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

uint32_t Counter_TIM11 = 0; // base for 13+ years overflow timer with 0.1 sec tick

void Timer11_Init(void){
 TIM11->PSC = 21600;
 TIM11->ARR = 1000; //10 times in one second
 TIM11->DIER |= TIM_DIER_UIE; //разрешаем прерывание от таймера
 TIM11->CR1 |= TIM_CR1_CEN; // Начать отсчёт!
 NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
 return;
}

void TIM11_IRQHandler(void){
 TIM11->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF
 Counter_TIM11++;
 //F_push(Run_GUI);
// F_push(Show_GUI);
 UpdateScreen = 1;
 return;
}