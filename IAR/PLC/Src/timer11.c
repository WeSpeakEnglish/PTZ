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

void Timer11_Init(void){
 TIM11->PSC = 21600;
 TIM11->ARR = 1000; //10 times in one second
 TIM11->DIER |= TIM_DIER_UIE; //��������� ���������� �� �������
 TIM11->CR1 |= TIM_CR1_CEN; // ������ ������!
 NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn); //���������� TIM6_DAC_IRQn ����������
 return;
}

void TIM11_IRQHandler(void){
 TIM11->SR &= ~TIM_SR_UIF; //���������� ���� UIF
 
 //F_push(Run_GUI);
// F_push(Show_GUI);
 UpdateScreen = 1;
 return;
}