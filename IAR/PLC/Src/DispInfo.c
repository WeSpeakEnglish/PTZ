#include "DispInfo.h"
#include "OSDBasic.h"
#include "OSDFont.h"
#include "OSDinitTable.h"
#include "tw8819.h"
#include "stm32f7xx_hal.h"
#include "InputsTW.h"
#include "OSDinitTable.h" 

uint8_t  DisplayedOSD = 0;
const uint16_t INTERSIL_COLOR_8[8] = {
  0xFFFF,0x0020,0xDEDB,0xA534,0x6B6D,0xC826,0x4A49,0xDCD5
};
const uint16_t TECHWELL_COLOR_8[8] = {
  0xFFFF,0xC806,0xC98C,0xCB31,0xFE79,0xFCD9,0xCCD4,0xE71C
};


#define INPUTINFO      0x10
#define RATINGINFO      0x40
void ClearOSDInfo(void){
  uint8_t dt;
  dt = DisplayedOSD; 
  if( dt & INPUTINFO )          ClearInput();
}

void OSDDisplayInput(uint8_t * StringDisp){
  uint8_t  len1, len2;
  len1 = strlenA(StringDisp);
  len2 = strlenA("EXIT");
  ClearOSDInfo();
  InitOSDWindow((uint8_t*)Init_Osd_DisplayInput);
  ClearDataCnt(INPUTINFO_ADDR, 51); // Total 42 Char.
  DrawAttrCnt(INPUTINFO_ADDR, DEFAULT_COLOR,51 );

  ClearDataCnt(RATINGINFO_ADDR, 51); // Total 42 Char.
  DrawAttrCnt(RATINGINFO_ADDR, DEFAULT_COLOR, 51 );
  WriteStringToAddr(INPUTINFO_ADDR, StringDisp, len1); 
  WriteStringToAddr(RATINGINFO_ADDR, "EXIT", len2);

  FOSDWinEnable(OSD_Win_Num(RATINGINFO_OSDWIN),1);
  FOSDWinEnable(OSD_Win_Num(INPUTINFO_OSDWIN),1);
  FOSDShowWindowAll(1);
}

uint8_t ClearInput(void){

  //FOSDShowWindowAll(0);
  FOSDWinEnable(INPUTINFO_OSDWIN-2, 0);
  FOSDWinEnable(INPUTINFO_OSDWIN-1, 0);

  DisplayedOSD &= (~(INPUTINFO));

  return 1;
  //  ClearBlending();
}



