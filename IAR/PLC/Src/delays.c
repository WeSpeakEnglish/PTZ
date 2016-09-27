#include "delays.h"
#include "initial.h"
void HAL_Delay(__IO uint32_t Delay){

  static uint32_t OldTicksGlobal_mS   = 0x00000000;
  uint32_t Difference     = 0x00000000;

  TicksGlobal_mS = HAL_GetTick();
  OldTicksGlobal_mS = TicksGlobal_mS; 
  
  while(Difference < Delay)  {
    TicksGlobal_mS = HAL_GetTick();
  if(OldTicksGlobal_mS > TicksGlobal_mS ){
    Difference = 0xFFFFFFFF - OldTicksGlobal_mS + TicksGlobal_mS + 1;
  }
  else Difference = TicksGlobal_mS - OldTicksGlobal_mS;

  WaitWhileSysTick(MAXDELAY_SYSTICK);      

      F_pull()(); // pull medium
      if(!WaitWhileSysTick(0)) return;
    }
}

void delay_100ms(void){
 HAL_Delay(100);

}