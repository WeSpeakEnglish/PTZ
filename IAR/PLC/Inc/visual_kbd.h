#ifndef _VISUAL_KBD
#define _VISUAL_KBD
#include "lcd.h"
#include "variables.h"

#define MAXSTRINGLENGTH 30
 

typedef struct {
  uint8_t Screen        : 3; // which screen is active
  uint8_t Pressed       : 1; // is button pressed?
  uint8_t Lang          : 1; // here a two languages for switching
//  uint8_t Upper         : 1; // lower or upper case
  uint8_t ReturnZone;
  uint8_t EnteredFromDISP_Screen;
  enum {KEYB_FULL = 0, KEYB_DATE = 1} Type;
  uint8_t Symbols[MAXSTRINGLENGTH+1];
}VKBD;

extern uint8_t V_KBD_Fill_button; // the filling button flag
extern VKBD VisualKBD;
extern const ZoneKBD ZonesKBD[];
//function for visualizing of key pressing
void ChangeColorKBD(void); // just highlite thr button what we need
uint8_t solveReturnCodeVisualKBD(void); // return the number of array position with pressed button 
void EraseStringVisualKBD(void);  // just string erasing
uint8_t CheckAllowedZones(uint8_t IndexToCompare); //impose very filter
#endif