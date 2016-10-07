#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H
#include "gui.h"
#include "stmpe811.h"
#include "rtc.h"
#include "lcd.h"
#include "visual_kbd.h"

typedef struct{
  uint8_t       Screen; //0 =base 1= lateral blade 2 = frontal blade 3 = topping 4 = brush 
  uint8_t       Event;
  uint8_t       KbdCode;
  int8_t        TS_ZoneNumber;
  int8_t        SelectedField;
  int8_t        ReleaseTask; // task for release button or touch screen
  int8_t        ReleaseFlag; // task for release button or touch screen
}Disp;

typedef struct{
  uint8_t TractorModel[MAXSTRINGLENGTH];
  uint8_t Language[MAXSTRINGLENGTH];        // the language of UI
  uint8_t TractorNumb[MAXSTRINGLENGTH];     // the number of Tractor
  uint8_t Version[MAXSTRINGLENGTH];         // Software version
  int16_t SpecialParam;                    // special parameter
  uint8_t ManufactureDate[MAXSTRINGLENGTH]; // like "12.09.2016"
  float Generator;                          // the current of generator
  uint8_t FuelTank[MAXSTRINGLENGTH];        // the tank of gas
  float WorkHours;                          // the time of work
  uint16_t SpeeedSensor;                    // the speed sensor here
  float TotalPatch;                         // the total patch of proceed
  float Square;                         // the total patch of proceed
  float EquipmentWide;                      // in meters how wide is our patch
  int16_t Motorman;                        // in meters how wide is our patch
} SAVE_Params;

typedef struct{
  uint8_t PressTransmiss; // pressure in the transmission
 // uint8_t PressPneumosys; 
  uint8_t PressEngineOil; // the pressure of oil
  float Speed;          //  the speed
  float RateEngine;     // the RPM
  float Square;           // the square, which have been prepared
  float TimeOfRun;        // the total time of pass
  uint16_t Passes;        // number of passes
  float SquarePerHour;    // (100m)^2 per hour
  float PetrolPerHour;    // litres per hour
  float PetrolPerSquare;  // litres per (100m)^2
  float TempEngine;       // the temperature of engine
  float PressOilEngine;   // the pressure in engine oil tubes
  float VolumeFuel;       // volume of gas in liters 
  float Voltage;          // voltage in the system (volts)
  float PressOilGearbox;  // the pressure in the gearbox 
  float TempOilGearbox;   // the temperature in the gearbox (transmission)
  float PressAir;         // pressuse in the pneumo system
  uint16_t Mileage;       // mileage in km
  
  uint8_t Slip; // slipping
  int8_t Rising; // rasing/penetration(-)
  int8_t Calibration; //in centimeters
  struct{
    uint8_t A;
    uint8_t B;
    uint8_t Lock : 1; // is it LOCK?
    float Time;
  }
  Hydroexits[5];
  //sensors: P means pressure and T means temperature
  
  struct{
   uint8_t T_CoolingLiquid             : 1;
   uint8_t P_Engine                    : 1;
   uint8_t ImpurityPressureFilter      : 1;
   uint8_t T_HydroSystem               : 1;
   uint8_t Engine_Malfunction          : 1;
   uint8_t ImpurityAirFilter           : 1;
   uint8_t T_Engine                    : 1;
   uint8_t ImpurityAttachments         : 1;
   uint8_t P_First_Contour             : 1;
   uint8_t EngineOilLevel              : 1;
   uint8_t CoolingLiquidLevel          : 1;
   uint8_t ImpurityEngineFilter        : 1;
   uint8_t ImpurityDrainFilter         : 1;
   uint8_t P_Second_Contour            : 1;
   uint8_t ImpurityCoolingLiquid       : 1;
   uint8_t ImpurityRudderFilter        : 1;
   uint8_t HydrotankLevel              : 1;
   uint8_t AccumulatorDischarge        : 1;
  }Errors; 
  struct{
    uint8_t ParkingLights              : 1;
    uint8_t LocalLight                 : 1;    
    uint8_t FarLight                   : 1;  // 
    uint8_t PTO                        : 1; //'ÌÎÌ'
    uint8_t RearAxle                   : 1;
    uint8_t ParkingBrake               : 1;
    uint8_t Coil                       : 1;
  }Signals;
}PTZ_params ;


extern SAVE_Params SaveParams;
extern PTZ_params PTZ;

extern volatile uint8_t UpdateScreen;

extern volatile Disp DISP;

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

uint32_t ShowAlertWindow(void);
void FastStrCpy(const uint8_t * Src, uint8_t * Dst, uint8_t SizeOfDst, Text_AlignModeTypdef Direction);
void UserParamsPreparingScreens(uint8_t Screen);
void BIG_Arrow(uint16_t SetValue);
void RPM_Arrow(uint16_t SetValue);
void FUEL_Arrow(uint16_t SetValue);
void TEMP_Arrow(uint16_t SetValue);
void LittleHidroExitsShow(void);
void UserControlsShow(void);
void UserParamsInit(void);
void BigHidroExitsShow(uint8_t Number, uint8_t Parm);
void UserParamsExchangeScreens(uint8_t ControlVar);
void UserParamsShow(void);
int32_t exAddition(int32_t inValue, int16_t minValue, int16_t maxVal, int16_t Step, uint16_t padPeriod);
void Tests(void);
void Test1(void);
void Test2(void);
void RunVisualKBD(void);
void ShowVisualKbdString(void); 
uint8_t ParseVisualKBD_Str(void);
void ExchangeScreensVisualKBD(uint8_t cmd);
void CAM_ON_OFF(void);
#endif



