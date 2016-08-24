#include "visual_kbd.h"
#include "video.h"
uint8_t V_KBD_Fill_button = 0;

void ChangeColor(uint32_t InColor, uint32_t OutColor, const Zone * InsideZone){
// register uint32_t StartAddress; 
 volatile uint32_t * pChange;
 int16_t i, j, Offset;
 Offset = DisplayWIDTH - (InsideZone->RightBottom.X - InsideZone->LeftTop.X);

 pChange = (volatile uint32_t *)(ProjectionLayerAddress[LayerOfView] + 4 * InsideZone->LeftTop.X + 4 * DisplayWIDTH * InsideZone->LeftTop.Y);
 
 for(j = InsideZone->RightBottom.Y - InsideZone->LeftTop.Y; j > 0; j--){
   for(i = InsideZone->RightBottom.X - InsideZone->LeftTop.X; i > 0; i-- ){
     if(*pChange == InColor){
      *pChange = OutColor;
     } 
     pChange++;   
   }
   pChange += Offset;
 }
 
 return;
}


