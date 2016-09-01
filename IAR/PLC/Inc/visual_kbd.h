#ifndef _VISUAL_KBD
#define _VISUAL_KBD
#include "lcd.h"
#include "variables.h"

extern uint8_t V_KBD_Fill_button; // the filling button flag
//function for visualizing of key pressing
void ChangeColorKBD(void); // just highlite thr button what we need
uint8_t solveReturnCodeVisualKBD(void); // return the number of array position with pressed button 


#endif