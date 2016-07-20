#ifndef _DELAYS_H
#define _DELAYS_H

#define MAXDELAY_DMA2D  300000 // runs of delay function to prevent hang on the procedures 

#define MAXDELAY_CORE   200000 // runs of delay function to prevent hang on the procedures 
#define MAXDELAY_SPI    100000 // runs of delay function to prevent hang on the procedures 

#include "variables.h"

inline uint8_t WaitWhileCORE(uint32_t CounterLimit){ // if we need to waiting awhile CORE
  static uint32_t Counter = 1;
  
  if(!CounterLimit){
    if(!Counter) 
      return 0;
     Counter--;
   }
  else{
  Counter = CounterLimit;
  }
 return 1;
}

inline uint8_t WaitWhileSPI(uint32_t CounterLimit){ // if we need to waiting awhile SPI content
  static uint32_t Counter = 1;
  
  if(!CounterLimit){
    if(!Counter) 
      return 0;
     Counter--;
   }
  else{
  Counter = CounterLimit;
  }
 return 1;
}

inline uint8_t WaitWhileDMA2D(uint32_t CounterLimit){ // if we need to waiting awhile DMA2D content
  static uint32_t Counter = 1;
  
  if(!CounterLimit){
    if(!Counter) 
      return 0;
     Counter--;
   }
  else{
  Counter = CounterLimit;
  }
 return 1;
}

#endif