#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H
#include "gui.h"
#include "stmpe811.h"
#include "rtc.h"
#include "lcd.h"

typedef struct{
  uint8_t       Screen; //0 =base 1= lateral blade 2 = frontal blade 3 = topping 4 = brush 
  uint8_t       Event;
  uint8_t       KbdCode;
  int8_t        TS_ZoneNumber;
  int8_t        SelectedField;
  int8_t        ReleaseTask; // task for release button or touch screen
  int8_t        ReleaseFlag; // task for release button or touch screen
}Disp;



extern volatile uint8_t UpdateScreen;

extern volatile Disp DISP;
extern volatile date_time_t dt;
extern volatile uint8_t CAM_flag;
extern uint8_t RateChange;
void Load_GUI_0(void);
void Load_GUI_2(void);
void ChangeCircle1(uint8_t Consistance);
void Run_GUI(void);
void KBD_Handle(uint8_t code);
void TouchScreen_Handle(void);
void PreLoadImages(void);
void ViewScreen(void);
void ReleaseFunction(void);
uint8_t solveTriangleZones(const Zone * pZone, uint8_t Type, const uint16_t X,  const uint16_t Y);
void KBD_Repeat_Handle(void);
uint32_t FillStructIMG(uint32_t address, uint16_t startIndex, uint16_t stopIndex);

void BIG_Arrow(uint16_t SetValue);
void RPM_Arrow(uint16_t SetValue);
void FUEL_Arrow(uint16_t SetValue);
void TEMP_Arrow(uint16_t SetValue);
void LittleHidroExitsShow(void);
void UserControlsShow(void);
void BigHidroExitsShow(uint8_t Number, uint8_t Parm);
int32_t exAddition(int32_t inValue, int16_t minValue, int16_t maxVal, int16_t Step, uint16_t padPeriod);
void Tests(void);
void Test1(void);
void Test2(void);
void RunVisualKBD(void);
void ShowVisualKbdString(void); 
#endif



