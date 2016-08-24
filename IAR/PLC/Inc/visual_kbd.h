#ifndef _VISUAL_KBD
#define _VISUAL_KBD
#include "lcd.h"
#include "variables.h"

extern uint8_t V_KBD_Fill_button; // the filling button flag
//function for visualizing of key pressing
void ChangeColor(uint32_t InColor, uint32_t OutColor, const Zone * InsideZone);



#endif