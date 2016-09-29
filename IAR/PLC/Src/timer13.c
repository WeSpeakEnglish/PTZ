#include "rtc.h"
#include "initial.h"
#include "keyboard.h"
#include "can_exchange.h"
#include "protocol_interface.h"

void Timer13_Init(void){
  TIM13->PSC = 10;
  TIM13->ARR = 1759;   //
  TIM13->DIER |= TIM_DIER_UIE; //
  TIM13->CR1 |= TIM_CR1_CEN; // 
  NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn); // TIM6_DAC_IRQn 
  return;
}

void TIM13_IRQHandler(void){
  static uint32_t Counter = 0;
  static uint32_t CounterUPD = 0;

 // static uint8_t Repeat = 0;
  TIM13->SR &= ~TIM_SR_UIF; // UIF

  switch (Counter){
  case 0:
    SetLineKbd(0); 
    break;
  case 10:
    ReadLineKbd(0); 
    break;
  case 20:
    SetLineKbd(1); 
    break;
  case 30: 
    ReadLineKbd(1); 
    break;
  case 40: 
    SetLineKbd(2); 
    break;
  case 50: 
    ReadLineKbd(2); 
    break;
  case 60:     
    SetLineKbd(3); 
    break;
  case 70: 
    ReadLineKbd(3); 
    break;
  case 80:     
    if(SolvePressedKeys()) {
      if(KB_Status.PRESSED){ 
        SOUND.CounterSound= 0; 
        SOUND.SoundPeriod = 200;
        CounterUPD = 0;
      }
      CounterUPD = 0;
      KB_Status.ENTER = 1;
    }
    else
        if(SOUND.CounterSound == SOUND.SoundPeriod){
      if(DISP.ReleaseTask && (Touch_Data.status == TOUCH_RELEASED) && (!DISP.ReleaseFlag))           ReleaseFunction();
    }
    if((KB_Status.EVENT && KB_Status.PRESSED) &&((CounterUPD % 20) == 18))    { 
        KB_Status.ENTER = 1;  
        KB_Status.REPEAT =1;
        CounterUPD = 0;
      }
    if((!KB_Status.PRESSED) && (Touch_Data.status == TOUCH_RELEASED)){
      CounterUPD = 0;
    }
    if(KB_Status.EVENT && !KB_Status.PRESSED && KB_Status.REPEAT)      {
        KB_Status.ENTER = 1;
        KB_Status.REPEAT =0;
      }
    if((CounterUPD % 20) == 18){
      CounterUPD = 0; 
    }
    CounterUPD++;
    break;
  }  
  if (SOUND.CounterSound < SOUND.SoundPeriod) {
    Bip(SOUND.CounterSound%2); 
    SOUND.CounterSound++;
  }
  Counter++;
  if(Counter % 100 == 90){
    
        M_push(CAN_GetValues);
    
  }
  Counter %= 100; 
  return;
}
