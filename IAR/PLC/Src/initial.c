#include "initial.h"


void InitPeriph(void){

  SDRAM_Initialization_Sequence(&hsdram1);

  pMediumQueueIni();            // fill the medium queue by Zero functions
  pFastQueueIni();              // fill the fast queue by Zero functions
  pSlowQueueIni();              // fill the slow queue by Zero functions
  NAND_readId();
  Timer14_Init_Deal(1000, 0);   //just init timer
  Timer13_Init();
  Timer11_Init();
  UB_Touch_Init();

  SDRAM_free();
  MX_LTDC_Init();
  LCD_Init();
  LCD_SetLight(10);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  LED_control(1);
}


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

      M_pull()(); // pull medium
      if(!WaitWhileSysTick(0)) return;
    }
}


