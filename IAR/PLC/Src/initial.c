#include "initial.h"



void InitPeriph(void){
//  date_time_t dt;
SDRAM_Initialization_Sequence(&hsdram1);
  
  pMediumQueueIni();            // fill the medium queue by Zero functions
  pFastQueueIni();              // fill the fast queue by Zero functions
  pSlowQueueIni();              // fill the slow queue by Zero functions
  
  NAND_readId();
  
  Timer14_Init_Deal(1000, 0);   //just init timer
  Timer13_Init();
  Timer11_Init();
  UB_Touch_Init();
//  BD_Init_TW8819();
  
 // dt.weekday = 1;
 // dt.day = 11;
//  dt.month = 07;
//  dt.year = 16;
//  dt.hours = 13;
//  dt.minutes = 8;
 // dt.seconds = 0;
 // PCF8563_set_datetime(&dt);
  
  

  
  SDRAM_free();

   MX_LTDC_Init();
  //LCD_Layers_Init();
 // BD_Init_TW8819(); 
 // Switch_Camera(1);
  
  LCD_Init();
  LCD_SetLight(7);
  
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  LED_control(1);
 

}



