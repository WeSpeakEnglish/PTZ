#include "PTZinterface.h"
#include "video.h"
#include "calculations.h"
#include "keyboard.h"
#include "visual_kbd.h"
#include "fonts.h"


#include "initial.h"
// (c) RA3TVD code, 2016

struct{
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
}SaveParams;

struct{
  uint32_t Machinist; // a motorman
  float Width;
}PreparatingSquare;

struct{
 uint8_t Screen           : 3;      // we show the calculation square screen
                                    // or we show the addition parameters screen
                                    // or show current ones
 uint8_t AddActiveStr      : 4; // activated string
 uint8_t GoesFromVirtualKB : 1; // if we go from the KB the behaviour is different
}UserParamsCond={0}; // conditions of the user control of showing parameters

struct{
  uint8_t PressTransmiss; // pressure in the transmission
  uint8_t PressPneumosys; // pressuse in the pneumo system
  uint8_t PressEngineOil; // the pressure of oil
  uint8_t Speed;          //  the speed
  uint8_t RateEngine;     // the RPM
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
  float PressAir;         // the temperature of air in the ...
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
} 
 PTZ;
                                                         // in the RIGHT text implementation mode
static uint8_t StrPressTransmiss[] =      "     0.0";
static uint8_t StrPressPneumosys[] =      "     0.0";
static uint8_t StrPressEngineOil[] =      "     0.0";
static uint8_t StrSpeed[] =               "       0";
static uint8_t StrRPM[] =                 "       0";
static uint8_t StrTIME[] =                "     0.0";
static uint8_t StrSquare[] =              "     0.0";
static uint8_t StrPasses[] =              "       0";
static uint8_t StrSquarePerHour[] =       "     0.0";
static uint8_t PetrolPerHour[] =          "     0.0";
static uint8_t PetrolPerSquare[] =        "     0.0";
static uint8_t StrSleep[]        =        "      0%";
static uint8_t StrRising[]       =        "      0";
//-----------------------------
static uint8_t StrTempEngine[]   =        "     0.0"; // ∞C
static uint8_t StrVolumeFuel[]   =        "     0.0"; // in liters
static uint8_t StrVoltage[]      =        "     0.0"; // in Volts
static uint8_t StrTempTransmiss[]=        "     0.0"; //
static uint8_t StrTotalPatch[]=           "     400"; // the total patch 
//-----------------------------

static uint8_t StrCalibration[]  =        "0      "; //in the LEFT mode

static GUI_Object* Images[90]; 
static GUI_Object* Text[80];
//static GUI_Object* Lines[20];
static GUI_Object* Polygons[6];
//static GUI_Object* Triangles[8];

static uint8_t StrDate[11]="25.04.2016";
static uint8_t StrTime[9]="20:00:00";
static uint8_t StrDATA[8][16];



const uint8_t String_1[] = "√ружу изображени€ в пам€ть:";
//const uint8_t String_2[] = "???";

Point Poly1_points[4]={
  {  227,311  }
  ,{ 363,317  }
  ,{ 355,311  }
  ,{ 363,304  }
};
Point Poly2_points[4]={
  {  411,198  }
  ,{ 477,202  }
  ,{ 472,198  }
  ,{ 477,194  }
};
Point Poly3_points[4]={
  {  662,123  }
  ,{ 704,120  }
  ,{ 704,126  }
};
Point Poly4_points[4]={
  {  662,311  }
  ,{ 704,308  }
  ,{ 704,314  }
};

const Point Origin_1 = {399,303};
const Point Origin_2 = {520,200};
const Point Origin_3 = {710,123};
const Point Origin_4 = {710,311};

volatile uint8_t UpdateScreen = 0;


uint8_t Temp8;
uint8_t StartTestFlag = 1;

uint8_t RateChange = 0;

volatile Disp DISP;

struct{
  ImageInfo ImgArray[300];
  uint16_t Number;
}
IMAGES;


const Zone ZonesTS_0[]={
  {{13,405},{94,477},           ob10111111},  //0 BIG BOTTOM BTN#0  valid on  all the pages
  {{112,405},{193,477},         ob10111111},  //1 BIG BOTTOM BTN#1  
  {{211,405},{292,477},         ob10111111},  //2 BIG BOTTOM BTN#2 
  {{310,405},{391,477},         ob10111111},  //3 BIG BOTTOM BTN#3  
  {{409,405},{490,477},         ob10111111},  //4 BIG BOTTOM BTN#4 
  {{508,405},{589,477},         ob10111111},  //5 BIG BOTTOM BTN#5 
  {{607,405},{688,477},         ob10111111},  //6 BIG BOTTOM BTN#6  
  {{706,405},{787,477},         ob10111111},  //7 BIG BOTTOM BTN#7 
  {{688,141},{756,202},         ob00000100},  //8 ---| these three buttons is active only on the second page and if the Condition.activity is setted up
  {{688,218},{756,280},         ob00000100},  //9    |
  {{688,296},{756,358},         ob00000100},  //10---| 
  {{310,172},{367,227},         ob00000100},  //11 //alternative penetration button 
  {{420,280},{480,340},         ob00000100},  //12 //alternative STOP button  
  {{530,172},{586,227},         ob00000100},  //13 // alternative arm button 
  {{18,77},  {171,384},         ob00001000},  //14 // alternative arm button 
  {{171,77}, {324,384},         ob00001000},  //15 // alternative arm button 
  {{324,77}, {477,384},         ob00001000},  //16 // alternative arm button 
  {{477,77}, {630,384},         ob00001000},  //17 // alternative arm button 
  {{630,77}, {783,384},         ob00001000},  //18 // alternative arm button 
  {{15,205}, {777,459},         ob01000000},  //19 // zone on KBD
};   

struct{ // this is a (penetration/rising) condition
  uint8_t label;  //which label to show
  uint8_t bigImage;
  uint8_t BlinkCounter; 
  uint8_t penetration         :   1;
  uint8_t set_penetration     :   1;
  uint8_t stop                :   1;
  uint8_t set_stop            :   1;
  uint8_t rising              :   1;
  uint8_t set_rising          :   1;
  uint8_t erase_flag          :   1;
  uint8_t activity            :   1; // to show the bottom and the right side buttons
  uint8_t butt_selected       :   2;
  uint8_t blink_button        :   2;
}
Condition={
  5,1,0,0,0,1,0,0,0,0}; 


#define MAX_PSEUDO_TIME_VAL     1000
struct{
  uint8_t EntireSelect    :       3; // select THIS complex exit
  uint8_t SelectedSide    :       1; // A or B side
  uint8_t EditShow        :       2; //edit or just show condition...which of them is edit
  uint8_t TimeEdit        :       2; // are we modefying the Time, right? 
  uint8_t LockToken       :       1; // we go to change lock field of the "hydroexit" instance
  uint8_t TimeEraseToken  :       1; // this token to erase time field to 0.0 in time edit mode
  uint8_t TempA;                     // we should store temporarily variable value of Extits intensity of the flow
  uint8_t TempB;
  uint16_t PseudoTime;               // multiplied by 10 for easily ++ and -- operations implements
  
 } PoolOfExits = {0,0,0,0,0,0,0}; 

void actions(uint8_t deal);
void PenetrationRising(uint8_t Parm, uint8_t set); //we can set the parameters of this control

void CAM_ON_OFF(void){
  if(!CAM_flag) {
    CAM_flag = 1;
    VideoCAMOnOff(4, 1); //number four on
  }
  else {
    VideoCAMOnOff(4, 0); //number four off
    CAM_flag = 0;
  }
  return;
}

void Load_GUI_0(void){ 
  //

  DISP.Screen = 10; // out of range to prevent any activities


  GUI_Free();
  Text[0] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 140, 240, (uint32_t)String_1, LEFT_MODE, 1, &GOST_B_23_var,0);   // watch
  Itoa(StrDATA[0],0);
  Text[1] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 400, 240, (uint32_t)StrDATA[0], LEFT_MODE, 1, &GOST_B_23_var,0);   // watch

  PreLoadImages();
  DISP.Screen = 0; 
  
  Text[2] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 40, 10, StrTime, LEFT_MODE, 1, &GOST_B_23_var,0);   // watch
  Text[3] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 700, 10, StrDate, LEFT_MODE, 1, &GOST_B_23_var,0);   // date

  Text[4] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 120, 100, StrPressTransmiss, RIGHT_MODE, 1, &RIAD_16pt,0);   // StrTransmiss
  Text[5] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 360, 205, StrSquare, RIGHT_MODE, 1, &RIAD_20pt,0); //square
  Text[6] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 120, 155, StrPressPneumosys, RIGHT_MODE, 1, &RIAD_16pt,0);
  Text[7] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 360, 240, StrTIME, RIGHT_MODE, 1, &RIAD_20pt,0); // time
  Text[8] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 120, 210, StrPressEngineOil, RIGHT_MODE, 1, &RIAD_16pt,0);
  Text[9] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 360, 275, StrPasses, RIGHT_MODE, 1, &RIAD_20pt,0);
  Text[10] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 755, 210, StrTIME, RIGHT_MODE, 1, &RIAD_16pt,0);
  Text[11] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 530, 205, StrSquarePerHour, RIGHT_MODE, 1, &RIAD_20pt,0);
  Text[12] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 405, 232, StrSpeed, RIGHT_MODE, 1, &RIAD_16pt,0);
  Text[13] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 530, 240, PetrolPerHour, RIGHT_MODE, 1, &RIAD_20pt,0);
  Text[14] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 576, 170, StrRPM, RIGHT_MODE, 1, &RIAD_16pt,0);
  Text[15] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 530, 275, PetrolPerSquare, RIGHT_MODE, 1, &RIAD_20pt,0);
  Text[16] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 312, 141, StrSpeed, RIGHT_MODE, 1, &RIAD_30pt,0);
  Text[17] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 456, 141, StrRPM, RIGHT_MODE, 1, &RIAD_30pt,0); // last parameter is bg_color 0 is transparent
  Text[18] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 708, 335, StrSleep, RIGHT_MODE, 1, &RIAD_20pt,0);
  Text[19] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 100, 322, StrRising, RIGHT_MODE, 1, &RIAD_20pt,0);
  Text[20] = GUI_SetObject(TEXT_STRING ,0xFF000000, 0, 7, 300, 97, StrCalibration, LEFT_MODE, 1, &RIAD_20pt,0);

  Images[0] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 1, 3, &IMAGES.ImgArray[287], 3   , 394); //HOME+ 99*i
  Images[4] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF333333, 0, 3, &IMAGES.ImgArray[290], 549 , 95); //circle for indicator // 2-nd screen (from 0...)
  // Images[2] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 201 , 394); //turn up/dowm
  // Images[3] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 300 , 394); //hydrocilinder
  //  Images[4] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 399 , 394); //microchip
  Images[5] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[197], 497 , 394); //piece of... with red
  Images[6] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF0A0C0B, 0, 3, &IMAGES.ImgArray[288], 41, 185);   //the triangle nice pointer green
  Images[7] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF0A0C0B, 0, 3, &IMAGES.ImgArray[289], 126, 185); // the triangle nice pointer red

  Images[8] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[175], 126 , 0); // the signal red sign
  Images[9] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[176], 126 + 77 , 0); // the sattellite red sign
  Images[10] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[177], 126 + 77*2 , 0); // the termometer red sign
  Images[11] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[187], 126 + 77*3 , 0); // the pressure red sign
  Images[12] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[188], 126 + 77*4 , 0); // the valve red sign
  Images[13] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[189], 126 + 77*5 , 0); // the filter red sign
  Images[14] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[181], 126 + 77*6 , 0); // the hydro red sign
  Images[15] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[10], 46 , 89); // the gypo and drop
  Images[16] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[26], 46 , 89+55); // the gypo and integrals
  Images[17] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[15], 46 , 89+110); // the Engine Oil

  Images[18] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[25], 36 , 320); // the counterclockwise gear and cap
  Images[19] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[23], 86 , 320); // the tractor with clockwise arrow

  Images[20] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[46], 230 , 325); // the parking lights sign
  Images[21] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[51], 276 , 325); // the near light sign
  Images[22] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[56], 316 , 325); // the far light sign
  Images[23] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[111], 359 , 320); // the FIRST big letter
  Images[24] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[111], 400 , 320); // the SECOND big letter
  Images[25] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[61], 440 , 324); // the T letter in the ring letter
  Images[26] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[64], 474 , 324); // the Oil mark 
  Images[27] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[67], 534 , 324); // the breaker

  Images[28] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[70], 314 , 267);            // the accumulator
  Images[29] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[6], 370 , 267);            // the coil
  Images[30] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[11], 431 , 267);            // the ((P)) sign

  Images[31] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[186], 696 , 135);            // the red fuel sign
  Images[32] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[185], 696 , 323);            // the red temp sign

  Polygons[0] = GUI_SetObject(ROTATING_FILLED_POLY_TYPE_FAST, 0xFFFF0000, 2, 4, Poly1_points,4, &Origin_1, 4000); // BIG ARROW with speed Point* pToPoints, uint8_t NumbOfPoints, const pPoint Origin, uint32_t angle_deg (*0.001 degrees)
  Polygons[1] = GUI_SetObject(ROTATING_FILLED_POLY_TYPE_FAST, 0xFFFF0000, 2, 4, Poly2_points,4, &Origin_2, 53500); //the middle arrow at 53.5 degrees
  Polygons[2] = GUI_SetObject(ROTATING_FILLED_TRIANGLE_FAST, 0xFFFF0000, 2, 3, Poly3_points, &Origin_3, 0); //the small arrow 
  Polygons[3] = GUI_SetObject(ROTATING_FILLED_TRIANGLE_FAST, 0xFFFF0000, 2, 3, Poly4_points, &Origin_4, 0); //the second small arrow 

}

void Run_GUI(void){

  ViewScreen();

  if(TimeIsReady){
    while (RESmutex_1) ;
    RESmutex_1 = 1;

    if(VisualKBD.Parsed && (VisualKBD.Type == KEYB_DATE)) 
    {
      M_push(PCF8583_set_by_Q);
      M_push(ResetParsedVisualKBD);
    }
     else
      M_push(PCF8583_read_by_Q);
    //PCF8563_read_datetime(&dt);
    RESmutex_1 = 0;

    GetDateToStr(StrDate, &dt);
    GetTimeToStr(StrTime, &dt);
    TimeIsReady = 0;
  }
  if(DISP.Event && Touch_Data.status == TOUCH_PRESSED){ 
    switch(DISP.TS_ZoneNumber){
    case 0:  //toggle index of button
    case 1:  //toggle index of button
    case 2:  //toggle index of button
    case 3:  //toggle index of button
    case 4:
    case 5:
    case 6:
    case 7:  //toggle index of button    
      actions(DISP.TS_ZoneNumber);
      break;
    case 8:  //toggle index of button  
      if (Condition.activity) actions(0);
      break;
    case 9:  //toggle index of button
      if (Condition.activity) actions(1);
      break;  
    case 10:
      if (Condition.activity) actions(2);
      break;
    case 11:
       if (!Condition.activity)actions(2);
      break;  
    case 12: 
       if (!Condition.activity)actions(4);
      break; 
    case 13: 
       if (!Condition.activity)actions(6);
      break;   
    case 14: 
      if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit){
      PoolOfExits.EntireSelect = 0;
      }
      break;  
    case 15: 
      if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit){
      PoolOfExits.EntireSelect = 1;
      }
      break; 
    case 16:
      if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit){
      PoolOfExits.EntireSelect = 2;
      }
      break;  
    case 17: 
      if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit){
      PoolOfExits.EntireSelect = 3;
      }
      break;      
    case 18: 
      if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit){
      PoolOfExits.EntireSelect = 4;
      }
    //------------- test
    case 19:
      V_KBD_Fill_button = DISP.TS_ZoneNumber;
      break;

   }

    DISP.Event = 0;
  }

  if(KB_Status.ENTER)KBD_Handle(KB_Status.code);
 
  if(DISP.Screen == 6){
       ShowVisualKbdString(); 
  }
  
  if(DISP.ReleaseFlag){
    if((Touch_Data.status != TOUCH_PRESSED) && (!KB_Status.PRESSED))       ReleaseFunction();
    DISP.ReleaseFlag = 0;
  }

}

void PreLoadImages(void){

  IMAGES.Number = 0;
  // just simply load images into the memory 
  FillStructIMG(SDRAM_BANK_ADDR + IMAGE_1_OFFSET, 0,   295);

  //image 006.bmp like base  
  FillImageSoft(IMAGES.ImgArray[161].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, IMAGES.ImgArray[161].xsize, IMAGES.ImgArray[161].ysize); 
  return;
}

void KBD_Handle(uint8_t code){ //the handle of the keyboard
  KB_Status.ENTER = 0;
  //up flags
  if(KB_Status.PRESSED){
    DISP.Event = 1;
    DISP.KbdCode = KB_Status.code;

    switch(DISP.SelectedField){
    case 1:
      break;
    case 2:
      break;
    }

    switch(DISP.Screen){
    case 0:
      switch(code){
      case 0x31:
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x37:

        actions(code - 0x31);
        break; 
      case 0x38:
        M_push(CAM_ON_OFF);
        break;
      }
      break;
    case 1:
      switch(code){
      case 0x31:
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x37:

        actions(code - 0x31); 
        break;
      case 0x38:
        M_push(CAM_ON_OFF);
        break;                   
      }
      break;
    case 2: 
      switch(code){
 
      case 0x31:
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x37:
      case 0x38:
        actions(code - 0x31); 
        break;                   
      }
      break;
    case 3: 
      switch(code){
 
      case 0x31:
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x37:
      case 0x38:
        actions(code - 0x31); 
        break;                   
      }
      break;      
    }


  }
  else
      {
    DISP.Event = 1;
    switch(DISP.Screen){
    case 1:
      DISP.ReleaseTask = 1;
      DISP.ReleaseFlag = 1;
      break;
    case 2:
      DISP.ReleaseTask = 2;
      DISP.ReleaseFlag = 1;
      break;
    case 3:
      DISP.ReleaseTask = 3;
      DISP.ReleaseFlag = 1;
      break;  
    case 4:
      DISP.ReleaseTask = 4;
      DISP.ReleaseFlag = 1;
      break;  
    }

  }
  return;
}


void TouchScreen_Handle(void){ //the handle of Touch Screen
  uint8_t Index;
  uint16_t x = Touch_Data.xp;
  uint16_t y = Touch_Data.yp;
  if(Touch_Data.status == TOUCH_PRESSED){
    DISP.TS_ZoneNumber = -1;

    for(Index = 0; Index < sizeof(ZonesTS_0)/8; Index++){
      if(ZonesTS_0[Index].PagesActivities & (1<<DISP.Screen)) // are we allowed here?
      if((x > ZonesTS_0[Index].LeftTop.X  && x < ZonesTS_0[Index].RightBottom.X)&&
            (y > ZonesTS_0[Index].LeftTop.Y  && y < ZonesTS_0[Index].RightBottom.Y)){
                DISP.TS_ZoneNumber = Index;
        if((DISP.TS_ZoneNumber != 7)&& CAM_flag)     
                DISP.TS_ZoneNumber = 100;
      }
    } 

    SOUND.CounterSound= 0, SOUND.SoundPeriod = 50; 
  }
  else{
    Timer14_Init_Deal(200, 3);

  }
  if(DISP.TS_ZoneNumber != -1){    
    DISP.Event = 1;
  } 

  return;
}

#define SENSORS_CONDITIONS_IMG_Y 44
#define TRACTOR_PARAMETERS_IMG_Y 44

void ViewScreen(void){
  uint16_t i;
  static uint8_t OldScreen = 0;
  Tests();
  if(OldScreen != DISP.Screen){
    for(i = 4; i < sizeof(Text)/4 ; i++){
      Text[i]->z_index = 0;
    } 
    for(i = 4; i < sizeof(Images)/4 ; i++){
      Images[i]->z_index = 0;
    } 
    for(i = 0; i < sizeof(Polygons)/4 ; i++){
      Polygons[i]->z_index = 0;
    } 
    switch(DISP.Screen){
    case 0:
      Text[4]->z_index = 3; // StrTransmiss
      Text[6]->z_index = 3; // StrPneumosys
      Text[8]->z_index = 3; // StrPressEngineOil
      Text[10]->z_index = 3; // StrTIME
      Text[12]->z_index = 3; // StrSpeed
      Text[14]->z_index = 3; // StrRPM
      Polygons[0]-> z_index = 1; // BIG ARROW with speed 
      Polygons[1]-> z_index = 1; //the middle arrow 
      Polygons[2]-> z_index = 1; //the small arrow 
      Polygons[3]-> z_index = 1; //the second small arrow 
      StartTestFlag = 1;

      Images[0]->params[1] = 3;
      Images[0]->z_index = 1;
      Images[23] -> z_index = 1;
      Images[24] -> z_index = 1;
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[161].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET);
      break; 
    case 1:
      Text[4]->z_index = 3; // StrTransmiss
      Text[5]->z_index = 3; //Square
      Text[6]->z_index = 3; // StrPneumosys
      Text[7]->z_index = 3; // Time
      Text[8]->z_index = 3; // StrPressEngineOil
      Text[9]->z_index = 3; // Passes
      Text[11]->z_index = 3; //square per hour 
      Text[13]->z_index = 3; //petrol per hour
      Text[15]->z_index = 3; //petrol per square
      Text[16]->z_index = 3; // SPEED
      Text[17]->z_index = 3; //RPM
      Text[18]->z_index = 3; //Sleep
      StartTestFlag = 0;
      Images[0]->params[1] = 102; // coord of frame
      Images[0]->z_index = 1;
      Images[23] -> z_index = 1;
      Images[24] -> z_index = 1;
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[164].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      break;  
    case 2:
      Text[19]->z_index = 3;
      Text[20]->z_index = 3;
      Images[0]->z_index = 0;
      Images[6]->z_index = 1;
      Images[7]->z_index = 1;
      Images[4]->z_index = 1;
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[272].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET);  //change the background
      break;
    case 3:
      Images[0]->z_index = 0;
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[239].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET);  //change the background
      break;   
    
    case 4:
      Images[0]->z_index = 0;
      
      // API fill IMAGE into background
      _HW_Fill_Image(IMAGES.ImgArray[251].address, 4 * TRACTOR_PARAMETERS_IMG_Y * DisplayWIDTH + SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, IMAGES.ImgArray[251].xsize, IMAGES.ImgArray[251].ysize);
      //_HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[251].address, 4 * SENSORS_CONDITIONS_IMG_Y * DisplayWIDTH + SDRAM_BANK_ADDR + LAYER_BACK_OFFSET);  //modify the background by IMAGE
      break;
    case 5: 
      Images[0]->z_index = 0;
      //prepare the screen for display on this properties position    
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[272].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      
      if (!UserParamsCond.GoesFromVirtualKB){
        UserParamsCond.Screen = 0;
        UserParamsCond.GoesFromVirtualKB = 0;
      }
      
        switch(UserParamsCond.Screen){
          case 0: // form the page of params
            UserParamsPreparingScreens(0);
           break; 
          case 1:
            UserParamsPreparingScreens(1);
           break; 
          case 2:
            UserParamsPreparingScreens(2);
           break;       

           
      }
     // SaveParams.TractorModel = StrToArray("Kirovec"
      break;
    case 6:
      Images[0]->z_index = 0;
          switch(VisualKBD.Type){
      case KEYB_FULL:
          _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[291].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
          break;
      case KEYB_DATE:
          ExchangeScreensVisualKBD(2);
         _HW_Fill_Region(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET + 39 * 4 + 269 * 4* DisplayWIDTH, 142, 65, 0xFF333333); // gray rectangles overheaded backgroung image
         _HW_Fill_Region(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET + 248 * 4 + 269 * 4* DisplayWIDTH, 415, 65, 0xFF333333);
         _HW_Fill_Region(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET + 16 * 4 + 333 * 4* DisplayWIDTH, 203, 65, 0xFF333333);
         _HW_Fill_Region(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET + 354 * 4 + 333 * 4* DisplayWIDTH, 427, 65, 0xFF333333);
         _HW_Fill_Region(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET + 16 * 4 + 397 * 4* DisplayWIDTH, 694, 65, 0xFF333333);
          break;
      }

          
      break; 
    }
    OldScreen = DISP.Screen;
  }
}

void ReleaseFunction(void){
  switch(DISP.Screen){
  case 1 :
    break;  
  case 2 :
    break;        
  case 3 : 
    exAddition(0,0,0,0,0); // reset exAddition Function
    break;
   
  case 6:
    V_KBD_Fill_button = 0;

    break;
    
  } 

  DISP.ReleaseTask = 0;
}


void KBD_Repeat_Handle(void){

  return;
}

void actions(uint8_t deal){
  static  struct {
   uint8_t WriteGo      : 1; 
   uint8_t EnterScreen  : 4;
  }
  Flags={0};

  switch(deal) {
  case 0: 
    if(DISP.Screen == 5) {
    //  if(!UserParamsCond.Screen) 
        UserParamsExchangeScreens(1);
      
    }
    else
    if(!((DISP.Screen == 2)&&(Condition.activity)))DISP.Screen = 0;
    if(DISP.Screen == 2 && Condition.activity)Condition.butt_selected = 1;
    break;
  case 1: 
    if(DISP.Screen == 5) {
   //  if(!UserParamsCond.Screen) 
       UserParamsExchangeScreens(2);
     
    }
    if(DISP.Screen == 0) DISP.Screen = 1;
    if(DISP.Screen == 2){
      if(Condition.activity)Condition.butt_selected = 2;
      else {PenetrationRising(4, 1); }//set activity to the control      
    }
    if(DISP.Screen == 3){
      if(PoolOfExits.EditShow){
        if(!PoolOfExits.SelectedSide)PoolOfExits.SelectedSide = 1;
        else PoolOfExits.SelectedSide = 0;
      }
      else{
       if(PoolOfExits.EntireSelect)PoolOfExits.EntireSelect--;
      }
    }
    if(DISP.Screen == 4) DISP.Screen = 1;
    break;
  case 2: 
    if(DISP.Screen == 3){
      if(PoolOfExits.EditShow){
        if(!PoolOfExits.SelectedSide)PoolOfExits.SelectedSide = 1;
        else PoolOfExits.SelectedSide = 0;
      }
      else{
       if(PoolOfExits.EntireSelect < 4)PoolOfExits.EntireSelect++;
      }
    }
    if(DISP.Screen == 2){
      if(Condition.activity)Condition.butt_selected = 3;
      else PenetrationRising(1, 1);
    }

    if(DISP.Screen < 2){
      DISP.Screen = deal;
      PenetrationRising(4, 0);
    }
   if(DISP.Screen == 4) DISP.Screen = 2;
    break;
  case 3: 
    if(DISP.Screen == 3){
      if(!PTZ.Hydroexits[PoolOfExits.EntireSelect].Lock)
      if(!PoolOfExits.EditShow){
        
        if(PoolOfExits.TimeEdit){
           if(PoolOfExits.PseudoTime < MAX_PSEUDO_TIME_VAL) PoolOfExits.PseudoTime++;
           }
        else
          PoolOfExits.EditShow = 1;
      }
      else{
        if(!PoolOfExits.SelectedSide){
         PoolOfExits.TempA = exAddition(PoolOfExits.TempA, 0, 100, 1, 10);///PoolOfExits.TempA++;
        }
        else{
         PoolOfExits.TempB = exAddition(PoolOfExits.TempB, 0, 100, 1, 10);
        } 
      }
    }
   if((DISP.Screen < 2))DISP.Screen = 3;
   if(DISP.Screen == 4) DISP.Screen = 3;
   
   if(DISP.Screen == 2){
      if(Condition.activity){
        Condition.blink_button = Condition.butt_selected;
        Condition.BlinkCounter = 0;
        Condition.erase_flag = 1; // set the erase token
      }
   }
 
  //plus button
     
    break;
  case 4:
   if(DISP.Screen < 2){
     DISP.Screen = 4;
    } 
   switch(DISP.Screen){
     case 2:
        if(Condition.activity){
        Condition.blink_button = Condition.butt_selected;
        Condition.BlinkCounter = 0;
        Condition.erase_flag = 0; // reset the erase token
        }
        else PenetrationRising(2, 1);
      break;
     case 3:
       if(!PTZ.Hydroexits[PoolOfExits.EntireSelect].Lock)
       if(!PoolOfExits.TimeEdit)PoolOfExits.TimeEdit = 1;
        else{
         if(PoolOfExits.EditShow){ 
          if(!PoolOfExits.SelectedSide){
           PoolOfExits.TempA = exAddition(PoolOfExits.TempA, 0, 100, -1, 10);
           }
           else{
            PoolOfExits.TempB = exAddition(PoolOfExits.TempB, 0, 100, -1, 10);
          } 
        }
        else{
           if(PoolOfExits.PseudoTime > 0) PoolOfExits.PseudoTime--;
         }
       }
      break; 
     case 5:
       if( UserParamsCond.Screen == 2){ // we edit the params and switch to the visual kbd
         VisualKBD.EnteredFromDISP_Screen = DISP.Screen;
         switch(UserParamsCond.AddActiveStr)
           case 1:
             VisualKBD.Type = KEYB_DATE;
             DISP.Screen = 6; 
            break; 
       }
     break;
   }
    break;  
  case 5:
    if(DISP.Screen == 0 || DISP.Screen == 1){
      if(Flags.WriteGo){ 
        PTZ.Passes++;
        Itoa_R(StrPasses, sizeof(StrPasses), PTZ.Passes);
      }
      Flags.WriteGo = !Flags.WriteGo;
      Images[5] ->z_index = Flags.WriteGo;
    }
    if(DISP.Screen == 3){
      if(PoolOfExits.TimeEdit) PoolOfExits.TimeEraseToken = 1;
    }
    break;
  case 6:      
    if(DISP.Screen == 2){
      PenetrationRising(3, 1);
    }
    else{
      if(DISP.Screen < 2){
      Flags.EnterScreen = DISP.Screen;
      DISP.Screen = 5;
      UserParamsInit();
    }
    else
      if(DISP.Screen == 3){
        if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit){
         if(!PoolOfExits.LockToken)
           PoolOfExits.LockToken = 1;

        }
        else
        if(PoolOfExits.EditShow == 2)
               PoolOfExits.EditShow = 3; 
        if(PoolOfExits.TimeEdit == 2)
               PoolOfExits.TimeEdit = 3; 
      }
    if(DISP.Screen == 4) DISP.Screen = 5;
    }
    break;
  case 7:
    switch(DISP.Screen){
      case 2:  
        if(Condition.activity != 0)PenetrationRising(4,0);
        else CAM_ON_OFF();
          break;
      case 3:
        if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit) DISP.Screen = 0; //if we in the edit mode
        else{
         PoolOfExits.EditShow = 0; //else go from Edit mode and discard changes
         PoolOfExits.TimeEdit = 0; //and time edit mode too...
           } 
        break;
      case 5:
        if(UserParamsCond.Screen){
           UserParamsExchangeScreens(0);
         }
        else{
          DISP.Screen = Flags.EnterScreen;

         }
        break;
      default:
          CAM_ON_OFF();
    }
    break;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// hanlders for test controls and others
void Test2(void){  
  static uint16_t Counter1; 
  if(Counter1 > 600){
    BIG_Arrow(1200 - Counter1);
    RPM_Arrow( (Counter1-600)*2/3);
    FUEL_Arrow((1200-Counter1)/6);
    TEMP_Arrow(40+ (12000- Counter1*10)/75);
  }
  else{ 
    BIG_Arrow(Counter1);
    FUEL_Arrow(Counter1/6);
    TEMP_Arrow(40+ (Counter1*10)/75);
    RPM_Arrow((600 - Counter1)*2/3);
  }
  Counter1+=2; 
  if(Counter1 == 1200) Counter1 = 0;
}


void Test1(void){
  static uint8_t Toggle1;
  static uint8_t Counter;
  if(Toggle1){
    Toggle1 = 0;
  }
  else{ 
    Toggle1 = 1;
  }
  if(DISP.Screen < 4){
  Images[8]->z_index =  Toggle1;
  Images[9]->z_index =  Toggle1;
  Images[10]->z_index = Toggle1;
  Images[11]->z_index = Toggle1;
  Images[12]->z_index = Toggle1;
  Images[13]->z_index = Toggle1;
  Images[14]->z_index = Toggle1;
  }
  else
  {
  Images[8]->z_index =  0;
  Images[9]->z_index =  0;
  Images[10]->z_index = 0;
  Images[11]->z_index = 0;
  Images[12]->z_index = 0;
  Images[13]->z_index = 0;
  Images[14]->z_index = 0;
  }
  Counter++;
}
void Test3(void){
  static uint8_t TestVar = 0;
  if(DISP.Screen == 4){
    PTZ.Errors.AccumulatorDischarge  = 0;
    PTZ.Errors.CoolingLiquidLevel = 0;
    PTZ.Errors.EngineOilLevel = 0;
    PTZ.Errors.Engine_Malfunction = 0;
    PTZ.Errors.HydrotankLevel = 0;
    PTZ.Errors.ImpurityAirFilter = 0;
    PTZ.Errors.ImpurityAttachments = 0;
    PTZ.Errors.ImpurityCoolingLiquid = 0;
    PTZ.Errors.ImpurityDrainFilter = 0;
    PTZ.Errors.ImpurityEngineFilter = 0;
    PTZ.Errors.ImpurityPressureFilter = 0;
    PTZ.Errors.ImpurityRudderFilter = 0;
    PTZ.Errors.P_Engine = 0;
    PTZ.Errors.P_First_Contour = 0;
    PTZ.Errors.P_Second_Contour = 0;
    PTZ.Errors.T_CoolingLiquid = 0;
    PTZ.Errors.T_Engine = 0;
    PTZ.Errors.T_HydroSystem = 0;
    
    switch(TestVar){
    case 1:   PTZ.Errors.T_CoolingLiquid = 1;
            break;
    case 2:   PTZ.Errors.P_Engine = 1;
            break;
    case 3:   PTZ.Errors.ImpurityPressureFilter = 1;
            break;
    case 4:   PTZ.Errors.T_HydroSystem = 1;
            break;     
    case 5:   PTZ.Errors.Engine_Malfunction = 1;
            break;     
    case 6:   PTZ.Errors.ImpurityAirFilter = 1;
            break;  
    case 7:   PTZ.Errors.T_Engine = 1;
            break;       
    case 8:   PTZ.Errors.ImpurityAttachments = 1;
            break;            
    case 9:   PTZ.Errors.P_First_Contour = 1;
            break;                
    case 10:   PTZ.Errors.EngineOilLevel = 1;
            break;                
    case 11:  PTZ.Errors.CoolingLiquidLevel = 1;
            break;  
    case 12:  PTZ.Errors.ImpurityEngineFilter = 1;
            break;
    case 13:  PTZ.Errors.ImpurityDrainFilter = 1;
            break;     
    case 14:  PTZ.Errors.P_Second_Contour = 1;
            break; 
    case 15:  PTZ.Errors.ImpurityCoolingLiquid = 1;
            break;  
    case 16:  PTZ.Errors.ImpurityRudderFilter = 1;
            break;         
    case 17:  PTZ.Errors.HydrotankLevel = 1;
            break;    
    case 18:  PTZ.Errors.AccumulatorDischarge = 1;
            break;    
    }

  }
  TestVar++;
  TestVar%= 19;
}
void Tests(void ){
  static uint32_t Counter = 0;
  if(StartTestFlag){
    if(Counter%8 == 0) Test1();
    if(! (Counter%16) )Test3();
    Test2();
    Counter++; 
  }

}
///!-- here is the our controls
void BIG_Arrow(uint16_t SetValue) // in the parts of 0.1 kmph
{
  if(DISP.Screen == 0) Polygons[0]->params[3] = 4000 + SetValue * 296;
  return;
}

void RPM_Arrow(uint16_t SetValue) // in the parts of 0.1 kmph
{
  if(DISP.Screen == 0) Polygons[1]->params[3] = 53500 + SetValue * 386;
  return;
}

void FUEL_Arrow(uint16_t SetValue) // in the parts of 0.01 of tank
{
  if(DISP.Screen == 0){
    Polygons[2]->params[2] = SetValue * 1800;
    if(SetValue < 17) Images[31]->z_index = 1;
    else Images[31]->z_index = 0;
  }
  return;
}

void TEMP_Arrow(uint16_t SetValue) // in the parts of 0.1 of degrees kmph 40
{
  if(DISP.Screen == 0){
    Polygons[3]->params[2] = (SetValue - 40) * 2250;
    if(SetValue > 99)     Images[32]->z_index = 1;
    else                  Images[32]->z_index = 0;
  }
  return;
}

//////////////// Virtual KBD Mechanica ---------------------------------------------------


typedef struct {
  enum  {LESS = 0, MORE, EQUAL}Sign; // border of threshold
  uint8_t Value; // comparing value
  uint8_t Index; // index in symbols array to apply the rule
} parseRules;

uint8_t TakeDayFromDate(date_time_t Date){
uint16_t t1, t2, t2r, t3;
const uint16_t m[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
 t1 = Date.year;
 t2 = t1 / 100;
 t2r = t1 % 100;
 t3 = t1 + (t1 >> 2) - t2 + (t2 >> 2) - 1;
 if ( ((t1 & 0x0003 == 0) && ((t2r != 0) || (t2 & 0x0003 == 0))) && (Date.month < 2) ) t3--;
return (t3 + m[Date.month - 1] + Date.day) % 7;
}

uint8_t ParseVisualKBD_Str(void){ //parse to date
register uint8_t i;  
uint8_t Errors = 0;


const parseRules Rules[]={
  {LESS,0x33,0},
  {MORE,0x2A,0},
  {LESS,0x3A,1},
  {MORE,0x2A,1},
  {EQUAL,':',2},
  {MORE,0x2A,3},  
  {LESS,0x36,3},
  {MORE,0x2A,4},  
  {LESS,0x3A,4},
  {EQUAL,',',5},
  {MORE,0x2A,6},  
  {LESS,0x34,6},
  {MORE,0x2A,7},  
  {LESS,0x3A,7},  
  {EQUAL,'.',8}, 
  {MORE,0x2A,9}, 
  {LESS,0x32,9}, 
  {MORE,0x2A,10}, 
  {LESS,0x3A,10},  
  {EQUAL,'.',11}, 
  {MORE,0x2A,12}, 
  {LESS,0x3A,12},
  {MORE,0x2A,13}, 
  {LESS,0x3A,13},
};

const uint8_t DaysRules[]={31,28,31,30,31,30,31,31,30,31,30,31};

  switch(VisualKBD.Type){
    case KEYB_DATE:
      for(i = 0; i < sizeof(Rules)/sizeof(parseRules); i++){
        switch(Rules[i].Sign){
          case LESS:
            if(!(VisualKBD.Symbols[Rules[i].Index] < Rules[i].Value))Errors++;
                 break;
          case MORE:
            if(!(VisualKBD.Symbols[Rules[i].Index] > Rules[i].Value))Errors++;
                 break;                 
          case EQUAL:
            if(!(VisualKBD.Symbols[Rules[i].Index] == Rules[i].Value))Errors++;
                 break;         
        }
       }

 
   DateSet.day =   (VisualKBD.Symbols[6] - 0x30)*10 + (VisualKBD.Symbols[7] - 0x30);
   DateSet.month = (VisualKBD.Symbols[9] - 0x30)*10 + (VisualKBD.Symbols[10] - 0x30);
   DateSet.year =  (VisualKBD.Symbols[12] - 0x30)*10 + (VisualKBD.Symbols[13] - 0x30);
   DateSet.hours = (VisualKBD.Symbols[0] - 0x30)*10 + (VisualKBD.Symbols[1] - 0x30);
   DateSet.minutes = (VisualKBD.Symbols[3] - 0x30)*10 + (VisualKBD.Symbols[4] - 0x30);
  
   if(DateSet.year > 0 && DateSet.year < 99){
    if(DateSet.month > 0 && DateSet.month < 13){    
      if(DateSet.day > 0 && DateSet.day < 32){ 
       if((DateSet.month == 2)&&(!(DateSet.year%4))){
         if (DateSet.day > 29) 
           Errors++;
        }
        else{
         if (DateSet.day > DaysRules[DateSet.month-1]) 
           Errors++;
       }
      } else 
        Errors++; 
     }
    else 
      Errors++;
   }
   else 
     Errors++;
  
   if(DateSet.hours > 23)Errors++;
   if(DateSet.minutes > 59)Errors++;  
   
   if(!Errors) {
     DateSet.weekday = TakeDayFromDate(DateSet);
    
     DateSet.seconds = 0;
     //PCF8563_set_datetime(&dt);
     VisualKBD.Parsed = 1;
     }   
     break;
   case KEYB_FULL:
     break;
  }
  if(!Errors) VisualKBD.Parsed = 1;
 return Errors;
}

void ExchangeScreensVisualKBD(uint8_t cmd){ // cmd is 0 eq SHIFT EXCHANGE; 1 - eq  LANGUAGE EXCANGE; 2 - numberous/literals exchange
   switch(cmd){
   case 0: 
    switch(VisualKBD.Screen){
     case 0: 
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[292].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      VisualKBD.Screen = 1;
         break;
     case 1: 
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[291].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      VisualKBD.Screen = 0;
         break;     
     case 2: 
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[294].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      VisualKBD.Screen = 3;
         break;      
     case 3: 
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[293].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      VisualKBD.Screen = 2;
         break;              
     }
    break;
   case 1:
    switch(VisualKBD.Screen){
     case 0:
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[293].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
       VisualKBD.Screen = 2; 
       VisualKBD.Lang = 1;
       break;
     case 1:
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[294].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
       VisualKBD.Screen = 3; 
       VisualKBD.Lang = 1;
       break;  
     case 2:
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[291].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      VisualKBD.Screen = 0; 
      VisualKBD.Lang = 0;
       break;       
     case 3:
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[292].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
      VisualKBD.Screen = 1; 
      VisualKBD.Lang = 0;
        break;                
     }
    break; 
   case 2:
    switch(VisualKBD.Screen){
      case 0:
      case 1:
      case 2:
      case 3:
       _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[295].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
       VisualKBD.Screen = 4; 
        break;
      case 4:  
        if(VisualKBD.Lang){
         _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[291].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
         if(VisualKBD.Type == KEYB_FULL)VisualKBD.Screen = 0;
        }
        else{
         
         switch(VisualKBD.Type){
            case KEYB_FULL:
              VisualKBD.Screen = 2;
              _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[293].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
              break;
            case KEYB_DATE:
              VisualKBD.Screen = 4;
              _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[295].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET); //change the background
              break;
         }

        } 
        break;
    }
    break;
  }
}

#define KBD_STR_X       39
#define KBD_STR0_Y      112
#define KBD_STR1_Y      164

#define DELAYED_HL_KB           500 // show, that data had approved by parser
//#define EXIT_HL_KB_PERIOD       // waiting in approval mode for X ms
void ShowVisualKbdString(void){
  uint8_t i;
//  uint8_t Str_Length;
  static uint32_t Delayed; // delay highlight
  static uint8_t parsedFlag = 0;
  static uint8_t Counter = 0;
  
  for(i = 0; i < sizeof(VisualKBD.Symbols)-1; i++){
   if(VisualKBD.Symbols[i] == '\0') break;
  }
 //Str_Length = i;
 
  switch(VisualKBD.Type){
    case KEYB_DATE:
      if( i > 14)
        VisualKBD.Symbols[i - 1] = '\0';
         break;   
  }
  
  if(Counter%16 < 8){
    VisualKBD.Symbols[i] = '}' - 1;
     VisualKBD.Symbols[i + 1] = '\0'; 
  }; 
  
  if(VisualKBD.Type == KEYB_DATE){
   LCD_InitParams(0, 0, 0xFFFFB500, &RIAD_20pt);
   LCD_DisplayStringAt(32, 50, "¬ведите врем€ и дату в формате", LEFT_MODE, 3);
   LCD_InitParams(0, 0, 0xFF00FFB5, &RIAD_20pt);
   LCD_DisplayStringAt(445, 50, "08:02,01.09.16", LEFT_MODE, 3);
  }
  if(VisualKBD.Parsed){
    LCD_InitParams(0, 0, 0xFF00FF00, &RIAD_30pt);
    parsedFlag = 1;
    Delayed = timestamp + DELAYED_HL_KB;
  }
  else{
    if(timestamp < Delayed){LCD_InitParams(0, 0, 0xFF00FF00, &RIAD_30pt);}
    else{
      if(!parsedFlag)LCD_InitParams(0, 0, 0xFFFF0000, &RIAD_30pt);
      else {
         UserParamsCond.GoesFromVirtualKB = 1; //we go from VirtualKBD
         DISP.Screen = VisualKBD.EnteredFromDISP_Screen;   //exit
         parsedFlag = 0;
      }
    }
  }

  LCD_DisplayStringAt(KBD_STR_X, KBD_STR0_Y, VisualKBD.Symbols, LEFT_MODE, 3);
  if(Counter%16 < 8)
    VisualKBD.Symbols[i] = '\0';
  Counter++;
}

void RunVisualKBD(void){
  uint8_t i;

  for(i = 0; i < sizeof(VisualKBD.Symbols); i++){
   if(VisualKBD.Symbols[i] == '\0') break;
  }
  switch (VisualKBD.ReturnZone){
      case 10:
        if(i)
         VisualKBD.Symbols[i-1] = '\0';
          break;
      case 20:
        ParseVisualKBD_Str();
          break;
      case 21:
         if(VisualKBD.Screen == 4){
           if(i<sizeof(VisualKBD.Symbols)- 1)
             VisualKBD.Symbols[i] = ZonesKBD[VisualKBD.ReturnZone].kbdCode2;
        }
        else
         ExchangeScreensVisualKBD(0);
          break;
      case 31:
        if(VisualKBD.Screen == 4) 
          VisualKBD.Symbols[i] = ZonesKBD[VisualKBD.ReturnZone].kbdCode2;
        else{
          ExchangeScreensVisualKBD(0);
        }
          break;
      case 32:
          ExchangeScreensVisualKBD(2);
          break;     
      case 33:
          ExchangeScreensVisualKBD(1);
          break;        
      case 35: 
          ExchangeScreensVisualKBD(2);
          break;
      case 36:
         UserParamsCond.GoesFromVirtualKB = 1; //we go from VirtualKBD
         DISP.Screen = VisualKBD.EnteredFromDISP_Screen;
    //     VisualKBD.EnteredFromDISP_Screen = 0;
          break;
      case 48:
         if(i)
          VisualKBD.Symbols[i-1] = '\0';
          break;
      case 60:
        
          break;
      case 61:
        ExchangeScreensVisualKBD(0);
          break;
      case 72:
        ExchangeScreensVisualKBD(0);        
          break;
      default:
        if(VisualKBD.Screen == 4){
           if(i<sizeof(VisualKBD.Symbols )- 1)
             VisualKBD.Symbols[i] = ZonesKBD[VisualKBD.ReturnZone].kbdCode2;
        }
        else
        {
          if(i<sizeof(VisualKBD.Symbols )- 1){ 
            if(!((VisualKBD.Screen == 1 )||(VisualKBD.Screen == 3))) 
             VisualKBD.Symbols[i] = ZonesKBD[VisualKBD.ReturnZone].kbdCode1;
            else
             if (ZonesKBD[VisualKBD.ReturnZone].kbdCode1 != ',' &&
                 ZonesKBD[VisualKBD.ReturnZone].kbdCode1 != '.')
              VisualKBD.Symbols[i] = ZonesKBD[VisualKBD.ReturnZone].kbdCode1 - 0x20;
            else 
              VisualKBD.Symbols[i] = ZonesKBD[VisualKBD.ReturnZone].kbdCode1;
          }
        }
        break;
  }
return;
}


void ErrorsShow(void){
  // the test area
    if(PTZ.Errors.T_CoolingLiquid) 
      LCD_Fill_Image(&IMAGES.ImgArray[121], 30, 93);
    if(PTZ.Errors.P_Engine) 
      LCD_Fill_Image(&IMAGES.ImgArray[134], 30, 149);
    if(PTZ.Errors.ImpurityPressureFilter) 
      LCD_Fill_Image(&IMAGES.ImgArray[138], 30, 207);
    if(PTZ.Errors.T_HydroSystem)
      LCD_Fill_Image(&IMAGES.ImgArray[125], 30, 266);
    if(PTZ.Errors.Engine_Malfunction)
      LCD_Fill_Image(&IMAGES.ImgArray[129], 30, 324);
    if(PTZ.Errors.ImpurityAirFilter)
      LCD_Fill_Image(&IMAGES.ImgArray[131], 217, 91);
    if(PTZ.Errors.T_Engine)
      LCD_Fill_Image(&IMAGES.ImgArray[135], 217, 149);
    if(PTZ.Errors.ImpurityAttachments)
      LCD_Fill_Image(&IMAGES.ImgArray[122], 217, 207);
    if(PTZ.Errors.P_First_Contour)
      LCD_Fill_Image(&IMAGES.ImgArray[126], 217, 266);
    if(PTZ.Errors.EngineOilLevel)
      LCD_Fill_Image(&IMAGES.ImgArray[130], 217, 324);
    if(PTZ.Errors.CoolingLiquidLevel)
      LCD_Fill_Image(&IMAGES.ImgArray[132], 403, 91);
    if(PTZ.Errors.ImpurityEngineFilter)
      LCD_Fill_Image(&IMAGES.ImgArray[136], 403, 149);
    if(PTZ.Errors.ImpurityDrainFilter)
      LCD_Fill_Image(&IMAGES.ImgArray[123], 403, 207);
    if(PTZ.Errors.P_Second_Contour)
      LCD_Fill_Image(&IMAGES.ImgArray[127], 403, 266);
    if(PTZ.Errors.ImpurityCoolingLiquid)
      LCD_Fill_Image(&IMAGES.ImgArray[133], 589, 91);
    if(PTZ.Errors.ImpurityRudderFilter)
      LCD_Fill_Image(&IMAGES.ImgArray[137], 589, 149);
    if(PTZ.Errors.HydrotankLevel)
      LCD_Fill_Image(&IMAGES.ImgArray[124], 589, 207);
    if(PTZ.Errors.AccumulatorDischarge)
      LCD_Fill_Image(&IMAGES.ImgArray[128], 589, 266);
}

///-------------USER CONTROL -------------------
#define STEP_BIG_HIDRO_CONTROL          153 // means on the X direction
#define ORIGIN_BIG_HIDRO_CONTROL        18
#define Y_BIG_HIDRO_CONTROL             77
#define y_stepHE_BIG                    14
#define ShowStrokePaddingX              56  // pixels padding of show line of squares in the SHOW condition (Number eq 0, Parm eq 0;)
#define ShowStrokePaddingY              70  //  show and editing at the same distantion
#define EditStrokePaddingX_A            25 // A column padding
#define EditStrokePaddingX_B            87 // B column padding
#define BHE_height_element              12 // 10 px of height
#define BHE_width_element               40 // 10 px of height 
#define GAP_elements                    5  //clearance between elements on Y direction
#define GAP_EditPictureX                14
#define GAP_EditPictureY                60
#define BHE_inactive_color              0xFF666666
#define BHE_active_color                0xFFFF3333
#define BHE_NUMB_ORIGIN_X               68 // big numbers at the top GAP position
#define BHE_NUMB_ORIGIN_Y               86
#define BHE_LEFT_ARR_GAP_X              18 // position of the left indicative arrow
#define BHE_ARR_GAP_Y                   222  // position of the left indicative arrow
#define BHE_RIGHT_ARR_GAP_X             104 // position of the left indicative arrow
#define BHE_ARR_MOVE_SCALE              1.65f //the scale factor
#define BHE_STRA_X_GAP                  60 // the string A x gap  
#define BHE_STRB_X_GAP                  122 // the string B x gap  
#define BHE_STRAB_Y_GAP                 254    // the string A x gap    
#define BHR_STRTIME_X_GAP               88 // the x padding of the Time stroke position
//
void BigHidroExitsShow(uint8_t Number, uint8_t Parm){ // we have 5 complex controls based on images, figures, texts 
                                                      // the number and addition parameter inside the demand for drive inside parms and parms of the array of stuctures; if all zero - just show
uint8_t i,j; // indexes for increment, plain usage 
uint8_t StrRightView[]  =        "      0"; //in the LEFT mode
const Point Coords[5] = 
{{ORIGIN_BIG_HIDRO_CONTROL, Y_BIG_HIDRO_CONTROL},
{ORIGIN_BIG_HIDRO_CONTROL + STEP_BIG_HIDRO_CONTROL, Y_BIG_HIDRO_CONTROL},
{ORIGIN_BIG_HIDRO_CONTROL + 2 * STEP_BIG_HIDRO_CONTROL, Y_BIG_HIDRO_CONTROL},
{ORIGIN_BIG_HIDRO_CONTROL + 3 * STEP_BIG_HIDRO_CONTROL, Y_BIG_HIDRO_CONTROL},
{ORIGIN_BIG_HIDRO_CONTROL + 4 * STEP_BIG_HIDRO_CONTROL, Y_BIG_HIDRO_CONTROL}};

const ImageInfo* ImagesNumber[] = { // the pool of images at the top of this big control
    &IMAGES.ImgArray[278],    // 0 // penetration label gray
    &IMAGES.ImgArray[280],    // 1 // penetration label red
    &IMAGES.ImgArray[282],    // 2 // rise and snubber label gray
    &IMAGES.ImgArray[284],    // 3 // rise and snubber label red
    &IMAGES.ImgArray[286]     // 4 // stop label gray
  };


if(!Number && !Parm){ // inside the show level

 for(j = 0; j < 5; j++){   
   
 if(PoolOfExits.EntireSelect != j){
     LCD_Fill_Image(&IMAGES.ImgArray[218], Coords[j].X, Coords[j].Y);
   }
   else {
     LCD_Fill_Image(&IMAGES.ImgArray[219], Coords[j].X, Coords[j].Y);
     if(PoolOfExits.EditShow){ // show the EditMode
     if(!PoolOfExits.SelectedSide)
       LCD_Fill_Image(&IMAGES.ImgArray[234], Coords[j].X + GAP_EditPictureX, Coords[j].Y + GAP_EditPictureY);
      else 
       LCD_Fill_Image(&IMAGES.ImgArray[235], Coords[j].X + GAP_EditPictureX, Coords[j].Y + GAP_EditPictureY);
      
      if(PoolOfExits.EditShow == 1){
        PoolOfExits.EditShow = 2; ///enter to the next stage
        PoolOfExits.TempA = PTZ.Hydroexits[j].A; // and load values to the temp variables
        PoolOfExits.TempB = PTZ.Hydroexits[j].B; // 
      }
      else{
        if(PoolOfExits.EditShow == 3) {
          PTZ.Hydroexits[j].A = PoolOfExits.TempA;
          PTZ.Hydroexits[j].B = PoolOfExits.TempB;
          PoolOfExits.TimeEdit = 0; 
          PoolOfExits.EditShow = 0;
        }
      }
       LCD_Fill_Image(&IMAGES.ImgArray[34], 400, 394); // "-"     //and three addition buttons in the edit mode
       LCD_Fill_Image(&IMAGES.ImgArray[269], 301, 394); // "+" 
       LCD_Fill_Image(&IMAGES.ImgArray[42],  597, 394); // "Enter" 
       LCD_InitParams(0, 0, 0xFF000000, &RIAD_16pt);
       Itoa_R(StrRightView, sizeof(StrRightView), (int16_t)PoolOfExits.TempA);
       LCD_DisplayStringAt(Coords[j].X + BHE_STRA_X_GAP, Coords[j].Y + BHE_STRAB_Y_GAP, StrRightView, RIGHT_MODE, 1);
       Itoa_R(StrRightView, sizeof(StrRightView), (int16_t)PoolOfExits.TempB);
       LCD_DisplayStringAt(Coords[j].X + BHE_STRB_X_GAP, Coords[j].Y + BHE_STRAB_Y_GAP, StrRightView, RIGHT_MODE, 1);
     }
     else {
       if(PoolOfExits.LockToken){
         if(PTZ.Hydroexits[j].Lock)PTZ.Hydroexits[j].Lock = 0;
         else PTZ.Hydroexits[j].Lock = 1;
       }
       if(PoolOfExits.TimeEraseToken){
         PoolOfExits.TimeEraseToken = 0; 
         PoolOfExits.PseudoTime = 0;
       }
     }
   }
   ///////////the Time manipulations/////////////
   if(!PoolOfExits.EditShow){
     if(j == PoolOfExits.EntireSelect && PoolOfExits.TimeEdit){   
       LCD_Fill_Image(&IMAGES.ImgArray[247], Coords[j].X + 20, Coords[j].Y + 250); // laying the time by image
       LCD_Fill_Image(&IMAGES.ImgArray[34], 400, 394);          // "-"     //and three addition buttons in the edit mode
       LCD_Fill_Image(&IMAGES.ImgArray[269], 301, 394);         // "+" 
       LCD_Fill_Image(&IMAGES.ImgArray[42],  597, 394);         // "Enter"
       LCD_Fill_Image(&IMAGES.ImgArray[92],  498, 394);         // the pencil button
       LCD_InitParams(0, 0, 0xFF000000, &RIAD_16pt);            // set font
       Ftoa_R(StrRightView, sizeof(StrRightView), ((float)PoolOfExits.PseudoTime/10.0f)); // calculate the time in modefied mode
       LCD_DisplayStringAt(Coords[j].X + BHR_STRTIME_X_GAP, Coords[j].Y + BHE_STRAB_Y_GAP, StrRightView, RIGHT_MODE, 1);
        // ant add this staff
    if(PoolOfExits.TimeEdit == 1){
        PoolOfExits.TimeEdit = 2; ///enter to the next stage
        PoolOfExits.PseudoTime = (int16_t)(PTZ.Hydroexits[j].Time * 10.0f);
      }
      else{
        if(PoolOfExits.TimeEdit == 3) {
          PTZ.Hydroexits[j].Time = (float)PoolOfExits.PseudoTime / 10.0f;
          PoolOfExits.TimeEdit = 0; 
          PoolOfExits.EditShow = 0;
       }
      }   
     }
    }
 if(!(PoolOfExits.EntireSelect == j &&  (PoolOfExits.EditShow || PoolOfExits.TimeEdit))){
 
         if(!PTZ.Hydroexits[j].Lock){
         LCD_Fill_Image(&IMAGES.ImgArray[248], Coords[j].X + 20, Coords[j].Y + 250); // laying the time by image 
         LCD_InitParams(0, 0, 0xFF000000, &RIAD_16pt);            // set font
         Ftoa_R(StrRightView, sizeof(StrRightView), (float)PTZ.Hydroexits[j].Time); // calculate the time in modyfied mode
         LCD_DisplayStringAt(Coords[j].X + BHR_STRTIME_X_GAP, Coords[j].Y + BHE_STRAB_Y_GAP, StrRightView, RIGHT_MODE, 1); //put in on the screen
       }
       else
       {
         LCD_SetColorPixel(0xFF0A0C0B); //set the transparence colour  
         LCD_Fill_ImageTRANSP(&IMAGES.ImgArray[237], Coords[j].X + 64, Coords[j].Y + 250); // laying the time by image 
       }
 }
   for(i = 0; i < 10; i++ ){
  if(PoolOfExits.EditShow && PoolOfExits.EntireSelect == j) { 
   if ((PoolOfExits.TempA + 5) / (100 - i*10)){ //math.round :) TempA instead of PTZ.Hydroexits[j].A
        LCD_SetColorPixel(BHE_active_color);
      }
      else{
        LCD_SetColorPixel(BHE_inactive_color );
      }
      LCD_FillRectDMA(Coords[j].X + EditStrokePaddingX_A , 
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY, 
                   Coords[j].X + EditStrokePaddingX_A + BHE_width_element,
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY + BHE_height_element);

      if ((PoolOfExits.TempB + 5) / (100 - i*10)){ //math.round :) TempB instead of PTZ.Hydroexits[j].B
        LCD_SetColorPixel(BHE_active_color);
      }
      else{
        LCD_SetColorPixel(BHE_inactive_color );
      }
      LCD_FillRectDMA(Coords[j].X + EditStrokePaddingX_B , 
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY, 
                   Coords[j].X + EditStrokePaddingX_B + BHE_width_element,
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY + BHE_height_element);
   }   
   else{
    LCD_SetColorPixel(BHE_inactive_color);
    LCD_FillRectDMA(Coords[j].X    + ShowStrokePaddingX , 
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY, 
                   Coords[j].X + ShowStrokePaddingX + BHE_width_element,
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY + BHE_height_element);
    }
   }
   //show the numbers at the top 1,2,3,4,5
   LCD_Fill_Image((ImageInfo *)ImagesNumber[j], BHE_NUMB_ORIGIN_X + j * STEP_BIG_HIDRO_CONTROL, BHE_NUMB_ORIGIN_Y);
      // show the arrows------
   LCD_SetColorPixel(0xFF0A0C0B); // choose the color for transparency
   if(!(PoolOfExits.EditShow && j == PoolOfExits.EntireSelect)){
      LCD_Fill_ImageTRANSP(&IMAGES.ImgArray[288], Coords[j].X + BHE_LEFT_ARR_GAP_X, Coords[j].Y + BHE_ARR_GAP_Y-(uint16_t)((float)PTZ.Hydroexits[j].A * BHE_ARR_MOVE_SCALE));
      LCD_Fill_ImageTRANSP(&IMAGES.ImgArray[289], Coords[j].X + BHE_RIGHT_ARR_GAP_X, Coords[j].Y + BHE_ARR_GAP_Y-(uint16_t)((float)PTZ.Hydroexits[j].B * BHE_ARR_MOVE_SCALE));
   }
     //---------put ignored parameters back -------------
 }
 if(PoolOfExits.LockToken)PoolOfExits.LockToken = 0; //erase Tocken
}
return;
}


//----------------

#define pos_x_controlHE         601
#define pos_y_controlHE         117
#define x_stepHE                35
#define x_step_secondHE         16
#define longierSquareW          12
#define shorherSquareW          7
#define y_stepHE                7

void LittleHidroExitsShow(void){
  uint16_t i,j;
  PTZ.Hydroexits[0].A = 100;
  PTZ.Hydroexits[2].B = 0;
  for(i = 0; i < 10; i++ ){ 
    for(j = 0; j < 5; j++){
      if ((PTZ.Hydroexits[j].A + 5) / (100 - i*10)){ //math.round :)
        LCD_SetColorPixel(0xFF3366FF);
      }
      else{
        LCD_SetColorPixel(0xFF999999);
      }
      LCD_FillRectDMA(pos_x_controlHE + j * x_stepHE, pos_y_controlHE + i*10, pos_x_controlHE + longierSquareW + j * x_stepHE, pos_y_controlHE + y_stepHE + i*10);

      if ((PTZ.Hydroexits[j].B + 5) / (100 - i*10)){ //math.round :)
        LCD_SetColorPixel(0xFFFF3333);
      }
      else{
        LCD_SetColorPixel(0xFF999999);
      }
      LCD_FillRectDMA(pos_x_controlHE + j * x_stepHE + x_step_secondHE, pos_y_controlHE + i*10, pos_x_controlHE + shorherSquareW + j * x_stepHE + x_step_secondHE, pos_y_controlHE + y_stepHE + i*10);

    }
  }
  return;
}

#define BLINKPERIOD     8
#define BLINKDUTY       4
#define BLINKTIME       48

void PenetrationRising(uint8_t Parm, uint8_t set){ //if Parm set as Zero (0) it will Run

  uint32_t LineColours[3]={
    0xffffffff,0xffffffff,0xffffffff  };

  //the images, what need to be chosen
  const ImageInfo* ImagesLabel[] = {
    &IMAGES.ImgArray[166],    // 0 // penetration label gray
    &IMAGES.ImgArray[165],    // 1 // penetration label red
    &IMAGES.ImgArray[231],    // 2 // rise and snubber label gray
    &IMAGES.ImgArray[230],    // 3 // rise and snubber label red
    &IMAGES.ImgArray[223],    // 4 // stop label gray
    &IMAGES.ImgArray[225]     // 5 // stop label red
  };
  const ImageInfo* ImagesBig[] = {
    &IMAGES.ImgArray[167],    // 0 // penetration BIG RED IMAGE
    &IMAGES.ImgArray[224],    // 1 // stop RED IMAGE
    &IMAGES.ImgArray[232]     // 2 // rise and snubber BIG RED IMAGE
  };  

  const  struct{
    ImageInfo* Address[3];
    Point Coords[3];
  }
  ButtonImages = {
    {
      &IMAGES.ImgArray[211],&IMAGES.ImgArray[212],&IMAGES.ImgArray[213]}
    ,{{6,395} ,{104,394} ,{203,395}}
  };
  const struct{ 
    ImageInfo* Address[3];  //3 //
    ImageInfo* EAddress[3]; // erase button alternative
    Point Coords[3];
  } 
  ButtonBlinksImages={
    {&IMAGES.ImgArray[2],&IMAGES.ImgArray[31],&IMAGES.ImgArray[39]},
    {&IMAGES.ImgArray[1],&IMAGES.ImgArray[30],&IMAGES.ImgArray[38]},
    {{683,136} ,{683,213} ,{683,292}}
  };

  switch(Parm){ // we just setting the parameter
  case 0: // Run
    if(Condition.penetration || Condition.set_penetration){

      if(((Condition.BlinkCounter % BLINKPERIOD) < BLINKDUTY)||Condition.set_penetration) {
        if(!Condition.activity ) {
          LineColours[0] = 0xFFFF0000;
          LCD_Fill_Image(&IMAGES.ImgArray[9], 200, 394);
        } 
        LCD_Fill_Image(&IMAGES.ImgArray[241], 301, 162);
      }
    }

    if(Condition.stop || Condition.set_stop){
      if(((Condition.BlinkCounter % BLINKPERIOD) < BLINKDUTY) || Condition.set_stop) {
        if(!Condition.activity ) { 
          LineColours[1] = 0xFFFF0000;
          LCD_Fill_Image(&IMAGES.ImgArray[16], 397, 394);
        }

        LCD_Fill_Image(&IMAGES.ImgArray[243], 412, 273);
      }
    }
    if(Condition.rising || Condition.set_rising){
      if(((Condition.BlinkCounter % BLINKPERIOD) < BLINKDUTY) || Condition.set_rising) { 
        if(!Condition.activity ) {  
          LineColours[2] = 0xFFFF0000;
          LCD_Fill_Image(&IMAGES.ImgArray[22], 597 , 394);
        }

        LCD_Fill_Image(&IMAGES.ImgArray[245], 523 , 162); 

      }
    }
    if(!Condition.activity ) {   
      LCD_SetColorPixel(LineColours[0]);
      LCD_DrawLine(251, 253, 304, 200);
      LCD_DrawLine(252, 253, 305, 200);
      _HW_LCD_V_Line(251, 253, 130);
      _HW_LCD_V_Line(252, 253, 130);  
      LCD_SetColorPixel(LineColours[1]);   
      _HW_LCD_V_Line(448, 345, 37);
      _HW_LCD_V_Line(449, 345, 37);
      LCD_SetColorPixel(LineColours[2]);     
      LCD_DrawLine(598, 200, 651, 253);
      LCD_DrawLine(599, 200, 652, 253);
      _HW_LCD_V_Line(651, 253, 130);
      _HW_LCD_V_Line(652, 253, 130);
    } 
    LCD_Fill_Image((ImageInfo *)ImagesLabel[Condition.label], 361, 102); // the label SHOW
    LCD_Fill_Image((ImageInfo *)ImagesBig[Condition.bigImage], 395, 145); 
    /// condition activity gets a new picture
    if(Condition.activity ) {
      LCD_Fill_Image(&IMAGES.ImgArray[214], 0, 394); 

      if(Condition.butt_selected)         
                        LCD_Fill_Image(ButtonImages.Address[Condition.butt_selected-1], 
                        ButtonImages.Coords[Condition.butt_selected-1].X,
                        ButtonImages.Coords[Condition.butt_selected-1].Y);
      if(Condition.blink_button)          
        if((Condition.BlinkCounter % BLINKPERIOD) < BLINKDUTY) {              
         if(Condition.erase_flag) 
          LCD_Fill_Image(ButtonBlinksImages.EAddress[Condition.blink_button-1], 
                          ButtonBlinksImages.Coords[Condition.blink_button-1].X, 
                          ButtonBlinksImages.Coords[Condition.blink_button-1].Y);    
         else
          LCD_Fill_Image(ButtonBlinksImages.Address[Condition.blink_button-1], 
                          ButtonBlinksImages.Coords[Condition.blink_button-1].X, 
                          ButtonBlinksImages.Coords[Condition.blink_button-1].Y); 
        }
    }

    if(Condition.BlinkCounter++ == 48){
      Condition.penetration = 0;
      Condition.stop = 0;
      Condition.rising = 0;
      Condition.blink_button = 0;
      Condition.erase_flag = 0;
      if (Condition.set_penetration)Condition.label = 1;
      if (Condition.set_rising)Condition.label = 3;
      if (Condition.set_stop)Condition.label = 5;
    }
    break;
  case 1:
    if(Condition.activity == 0){
      if(Condition.penetration){ 
        Condition.bigImage = 0;
        Condition.set_penetration = 1;
        Condition.label = 1; //set the  label 
        Condition.set_rising = 0;
        Condition.set_stop = 0;
        Condition.penetration = 0;
        Condition.rising = 0;
        Condition.stop = 0;
      }
      else{
        Condition.penetration = (set) ? 1 : 0;
        Condition.label = 0;
        Condition.rising = 0;
        Condition.stop = 0;
      }
      Condition.BlinkCounter = 0;
    }
    break;
  case 2:
    if(Condition.activity == 0){
      if(Condition.stop){ 
        Condition.bigImage = 1;
        Condition.set_stop = 1;
        Condition.label = 5; //set the  label 
        Condition.set_rising = 0;
        Condition.set_penetration = 0;
        Condition.penetration = 0;
        Condition.rising = 0;
        Condition.stop = 0;
      }
      else{
        Condition.stop = (set) ? 1 : 0;
        Condition.label = 4;
        Condition.penetration = 0;
        Condition.rising = 0;
      }
      Condition.BlinkCounter = 0;
    }

    break;
  case 3:
    if(Condition.activity == 0){
      if(Condition.rising){ 
        Condition.bigImage = 2;
        Condition.set_rising = 1;
        Condition.label = 3; //set the  label 
        Condition.set_stop = 0;
        Condition.set_penetration = 0;
        Condition.penetration = 0;
        Condition.rising = 0;
        Condition.stop = 0;
      }
      else{
        Condition.rising = (set) ? 1 : 0;
        Condition.label = 2;
        Condition.penetration = 0;
        Condition.stop = 0;
      }
      Condition.BlinkCounter = 0;
    }
    break;
  case 4:
    if(set==1){
      Condition.activity = 1;
      Condition.butt_selected = 0;
    }
    if(set==0)Condition.activity = 0;
    break;
  }

}

typedef struct{
  uint16_t N;   // the number
  Point Coords; // the coords
} pointANDcoords;

void UserParamsPreparingScreens(uint8_t Screen){ // preparing the background

uint8_t Index;

const pointANDcoords
ImParamsLoc[]={
  {271,{0,44}},    //0 the number in the image array and coords
  {195,{2,394}},   //1 field button dashed
  {192,{101,394}}, //2 the ham and skrudriver
  {60,{696,394}},  //3 cross           // cross on all the pages, starts from zero ...(optimized)
  {183,{200,394}}, //4 empty button 
  {183,{398,394}}, //5 empty button
  {183,{597,394}}, //6 empty button
  ////----------------------------- screen 1
  {270,{0,44}},    //7 square calculation
  {45,{2,394}},    //8 up button
  {44,{101,394}},  //9 down button
  {193,{200,394}}, //10 a man name edition  
  ////----------------------------- screen 0
  {209,{0,44}},    //11 square calculation
  {45,{2,394}},    //12up button
  {44,{101,394}},  //13 down button
};

switch(Screen){
 case 0:          
   for(Index = 0; Index < 7; Index++)   
           _HW_Fill_Image(IMAGES.ImgArray[ImParamsLoc[Index].N].address, 
                          4 * ImParamsLoc[Index].Coords.Y * DisplayWIDTH + 4 * ImParamsLoc[Index].Coords.X + SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, 
                          IMAGES.ImgArray[ImParamsLoc[Index].N].xsize, 
                          IMAGES.ImgArray[ImParamsLoc[Index].N].ysize);
           UserParamsCond.Screen = 0;
    break;
 case 1:
     for(Index = 7; Index < 11; Index++)   
           _HW_Fill_Image(IMAGES.ImgArray[ImParamsLoc[Index].N].address, 
                          4 * ImParamsLoc[Index].Coords.Y * DisplayWIDTH + 4 * ImParamsLoc[Index].Coords.X + SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, 
                          IMAGES.ImgArray[ImParamsLoc[Index].N].xsize, 
                          IMAGES.ImgArray[ImParamsLoc[Index].N].ysize);
     
           UserParamsCond.Screen = 1;
    break; 
    
  case 2:
     for(Index = 11; Index < 14; Index++)   
           _HW_Fill_Image(IMAGES.ImgArray[ImParamsLoc[Index].N].address, 
                          4 * ImParamsLoc[Index].Coords.Y * DisplayWIDTH + 4 * ImParamsLoc[Index].Coords.X + SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, 
                          IMAGES.ImgArray[ImParamsLoc[Index].N].xsize, 
                          IMAGES.ImgArray[ImParamsLoc[Index].N].ysize);
     
           UserParamsCond.Screen = 2;
    break;    
    
} 
  return;
}

// exchange parameters screens
#define USERCONTROLSTRINGS 11 // how much strings we have here on screen 1 of our control 
void UserParamsExchangeScreens(uint8_t ControlVar){
       switch(UserParamsCond.Screen){
          case 0:  
            switch(ControlVar){
              case 1: 
                          UserParamsPreparingScreens(1);
                          break;
              case 2:
                          UserParamsPreparingScreens(2);
                          break;
            }
            break;  
            
         case 1:
            switch(ControlVar){
              case 0:
                UserParamsPreparingScreens(0);
                break;
              case 1:
                //to do:: put activities for first button on second screen of the user control right here :)
                break;
              case 2:
                //to do:: put activities for first button on second screen of the user control right here :)
                break;  
            }   
            break;  
            
          case 2:
            switch(ControlVar){
              case 0:
                UserParamsPreparingScreens(0);
                break;
              case 2:
                  UserParamsCond.AddActiveStr++; 
                  UserParamsCond.AddActiveStr %= USERCONTROLSTRINGS;
                 // if(!UserParamsCond.AddActiveStr)
                 //                   UserParamsCond.AddActiveStr = 1;
                break;
              case 1:
                  if(UserParamsCond.AddActiveStr) 
                    UserParamsCond.AddActiveStr--; 
                  else 
                    UserParamsCond.AddActiveStr = USERCONTROLSTRINGS - 1;
                break;  
            }
            break;
       
      }
  return;
}

//////////
void FastStrCpy(const uint8_t * Src, uint8_t * Dst, uint8_t SizeOfDst, Text_AlignModeTypdef Direction){ // Direction is 0 = default(LEFT one), 1  = 
// MAXSTRINGLENGTH is compare with max index
uint8_t i = 0;
uint8_t length = 0;

  switch (Direction){
    case LEFT_MODE: //
      for(i = 0; i < SizeOfDst; i++){
        if(*Src != '\0' ) *Dst++ = *Src++;
        else break;
     }
      break;
    case RIGHT_MODE:
     while(Src[i++])length++;
       for(i = 1; i < length+1 ; i++) // the lastest symbol is '\0'
         Dst[SizeOfDst-i-1] = Src[length-i]; //:)
      break; 
       }  
}

void UserParamsInit(void){
 FastStrCpy("Russian", SaveParams.Language, sizeof(SaveParams.Language), LEFT_MODE);
 FastStrCpy("Kirovec", SaveParams.TractorModel, sizeof(SaveParams.TractorModel), LEFT_MODE);
 FastStrCpy("у567ен78rus",SaveParams.TractorNumb, sizeof(SaveParams.TractorNumb), LEFT_MODE);
 FastStrCpy("0.10.1",SaveParams.Version, sizeof(SaveParams.Version), LEFT_MODE);
 SaveParams.SpecialParam = -30000;
 FastStrCpy("19.09.2016",SaveParams.ManufactureDate, sizeof(SaveParams.ManufactureDate), LEFT_MODE); 
 SaveParams.Generator = 0.17f;
 FastStrCpy("V400", SaveParams.FuelTank, sizeof(SaveParams.ManufactureDate), LEFT_MODE); 
 SaveParams.WorkHours = 34.3f; 
 SaveParams.SpeeedSensor = 30000;
 SaveParams.TotalPatch = 400.0; // it crossing with String... StrTotalPatch
}


uint8_t EditableStringsN[]={1,2,4,6,7,9};

inline uint8_t CheckEditable(void){
 uint8_t i; // just index
 register uint8_t Result = 0;
 
 for(i = 0; i < sizeof(EditableStringsN); i++)
   if( UserParamsCond.AddActiveStr == EditableStringsN[i])
     Result = 1;
return Result;
}

typedef struct{
    Point Coords;   // the coords of the displaying string
    uint8_t * pStr; // pointer to the String to display
    Text_AlignModeTypdef  TextAlignment;
} specMark;

void UserParamsShow(void){
  uint8_t i;
const specMark  ParamsCoordsANDStr_0[] = {
    {{289,89}, SaveParams.TractorModel, LEFT_MODE},      //the model of the tractor
    {{327,118}, StrRPM, RIGHT_MODE},                     //the RPM rate
    {{327,146}, StrTempEngine, RIGHT_MODE},              //the temperature of engine    
    {{327,173}, StrPressEngineOil, RIGHT_MODE},             //the pressure of oil in the engine
    {{327,201}, StrVolumeFuel, RIGHT_MODE},              //how much of fuel we are have
    {{327,228}, StrVoltage, RIGHT_MODE},                 // voltage
    {{327,257}, StrPressTransmiss, RIGHT_MODE},          // oil pressure in the transmission 
    {{327,284}, StrTempTransmiss, RIGHT_MODE},          // oil temerature in the transmission 
    {{327,312}, StrPressPneumosys, RIGHT_MODE},          // oil temerature in the transmission 
    {{327,340}, StrTotalPatch, RIGHT_MODE},              // the total patch
  };

const specMark  ParamsCoordsANDStr_2[] = {
    {{285,89}, SaveParams.Language, LEFT_MODE},         //the language of the GUI
    {{285,115}, StrTime, LEFT_MODE},                    //time 
    {{355,115}, StrDate, LEFT_MODE},                    //date
    {{285,140}, SaveParams.TractorNumb, LEFT_MODE},     //Tractor number
    {{285,166}, SaveParams.Version, LEFT_MODE},         //version of Software
    {{285,216}, SaveParams.ManufactureDate, LEFT_MODE}, //DATE OF MADE
    {{285,269}, SaveParams.FuelTank, LEFT_MODE},        //type of the fuel tank
  };
uint8_t AtiveStringParms[]={0,1,1,2,3,5,7};

const pointANDcoords   // the menu coords
    ImParamsLoc[] = {
    {258,{33,91}},  // Language
    {261,{33,116}},  // Time ajust    
    {262,{33,142}},  // Tractor's number   
    {263,{33,167}},  // Program Version
    {264,{33,193}},  // special parameter
    {265,{33,218}},  // manufacturing date
    {266,{33,243}},  // generator
    {267,{33,268}},  // fuel tank
    {268,{33,294}},  // time of work
    {259,{33,319}},  // speed sensor
    {260,{33,345}},  // total patch
    };
  
  switch(UserParamsCond.Screen){
    case 0:
         
       //  Ftoa_R(StrRightView, sizeof(StrRightView), (float)PTZ.Hydroexits[j].Time); // calculate the time in modyfied mode
      for(i = 0; i < sizeof(ParamsCoordsANDStr_0)/sizeof(specMark); i++)
         LCD_DisplayStringAt(ParamsCoordsANDStr_0[i].Coords.X, ParamsCoordsANDStr_0[i].Coords.Y, ParamsCoordsANDStr_0[i].pStr, ParamsCoordsANDStr_0[i].TextAlignment, 1); //put in on the screen
          break;
    case 1:
      
          break;
    case 2:   
      LCD_Fill_Image(&IMAGES.ImgArray[ImParamsLoc[UserParamsCond.AddActiveStr].N], //show selected line
                     ImParamsLoc[UserParamsCond.AddActiveStr].Coords.X, 
                     ImParamsLoc[UserParamsCond.AddActiveStr].Coords.Y);
      
      for(i = 0; i < sizeof(ParamsCoordsANDStr_2)/sizeof(specMark); i++){
        if(UserParamsCond.AddActiveStr == AtiveStringParms[i]) LCD_SetColorPixel(0xFF00FF00);
        else LCD_SetColorPixel(0xFFFFFFFF);
       
        LCD_DisplayStringAt(ParamsCoordsANDStr_2[i].Coords.X, ParamsCoordsANDStr_2[i].Coords.Y, ParamsCoordsANDStr_2[i].pStr, ParamsCoordsANDStr_2[i].TextAlignment, 1); //put in on the screen
       } 
      
      if(UserParamsCond.AddActiveStr == 4) LCD_SetColorPixel(0xFF00FF00);
      else LCD_SetColorPixel(0xFFFFFFFF);
      Itoa(StrDATA[0], SaveParams.SpecialParam);               // special parameter conversion
      LCD_DisplayStringAt(285, 193, StrDATA[0], LEFT_MODE, 1); // 
      
      if(UserParamsCond.AddActiveStr == 6) LCD_SetColorPixel(0xFF00FF00);
      else LCD_SetColorPixel(0xFFFFFFFF); 
      Ftoa_2(StrDATA[0], SaveParams.Generator);                 // Generator ratio
      LCD_DisplayStringAt(285, 244, StrDATA[0], LEFT_MODE, 1); // put it on screen
      
      if(UserParamsCond.AddActiveStr == 8) LCD_SetColorPixel(0xFF00FF00);
      else LCD_SetColorPixel(0xFFFFFFFF);  
      Ftoa_1(StrDATA[0], SaveParams.WorkHours);                 // work hours
      LCD_DisplayStringAt(285, 294, StrDATA[0], LEFT_MODE, 1); // 
 
      if(UserParamsCond.AddActiveStr == 9) LCD_SetColorPixel(0xFF00FF00);
      else LCD_SetColorPixel(0xFFFFFFFF); 
      Utoa(StrDATA[0], SaveParams.SpeeedSensor);
      LCD_DisplayStringAt(285, 320, StrDATA[0], LEFT_MODE, 1); // the speed sensor
      
      if(UserParamsCond.AddActiveStr == 10) LCD_SetColorPixel(0xFF00FF00);
      else LCD_SetColorPixel(0xFFFFFFFF); 
      Utoa(StrDATA[0], (uint16_t)SaveParams.TotalPatch);
      LCD_DisplayStringAt(285, 344, StrDATA[0], LEFT_MODE, 1); // the total patch
      
      LCD_SetColorPixel(0xFFFFFFFF); //all the end !
      
      //show the especial picture with a pencil
      if(CheckEditable()){
                    LCD_Fill_Image(&IMAGES.ImgArray[8],398,394);
      }
          break;          
  }
  return;
}
///!---------------------------------------------------------------------------
void UserControlsShow(void){
  switch(DISP.Screen){
  case 1:
    LittleHidroExitsShow();
    break;
  case 2:
    PenetrationRising(0, 0);
    break;
  case 3:
    BigHidroExitsShow(0, 0);
    break;
  case 4:
    // LCD_Fill_Image(&IMAGES.ImgArray[291], 0, 90); 
    ErrorsShow();
     break;
  case 5:
    UserParamsShow();
     break;
     
  case 6:
    if(V_KBD_Fill_button)
       ChangeColorKBD();
    else{
      if(VisualKBD.Pressed){ 
        VisualKBD.Pressed = 0;
        RunVisualKBD();
      }
    }
     break;
      }
   DISP.TS_ZoneNumber = -1; 
  return;
}
////-----------------------------------------------------------------------------------------------------
uint32_t FillStructIMG(uint32_t address, uint16_t startIndex, uint16_t stopIndex){
  uint16_t i = 0;
  static uint8_t Name[]="000.bmp";
  ImgSize SizesIMG;
  if (SD_mount(1)  == FR_OK){ //mount SD card in 4-bit mode
    for(i = startIndex; i < stopIndex + 1; i++){
      Name[0] = (i/100) + 0x30;
      Name[1] = (i/10 - 10*(i/100))  + 0x30;
      Name[2] = (i%10)  + 0x30;
      SizesIMG = LoadBitmapFromSD(Name, address);

      IMAGES.ImgArray[IMAGES.Number].index   = i;
     
      IMAGES.ImgArray[IMAGES.Number].xsize   = SizesIMG.width;
      IMAGES.ImgArray[IMAGES.Number].ysize   = SizesIMG.height; 
      IMAGES.ImgArray[IMAGES.Number].address = address;
      address += ((uint32_t)SizesIMG.height ) * ((uint32_t)SizesIMG.width) * 3;
     if(i==0)FillImageSoft(IMAGES.ImgArray[0].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, IMAGES.ImgArray[0].xsize, IMAGES.ImgArray[0].ysize); 
     else
      if(UpdateScreen){
        Itoa(StrDATA[0],i);    
        Run_GUI();
        Show_GUI();
        UpdateScreen = 0;
        DISP.ReleaseFlag = 0;
      }
       
      IMAGES.Number++;
    }  
    Text[0]->existance = 0; //delete
    Text[1]->existance = 0; //delete
    SD_mount(0); //unmount SD card in 4-bit mode
  }

  return address;
}
//-- SUBCONTROL functions

uint8_t solveTriangleZones(const Zone * pZone, uint8_t Type, const uint16_t X,  const uint16_t Y) //solve triangle zones [/] and [\] types 
{
  float x1 = (float) pZone->LeftTop.X;
  float x2 = (float) pZone->RightBottom.X;
  float y1 = (float) pZone->LeftTop.Y;
  float y2 = (float) pZone->RightBottom.Y;
  float Ys, k;

  k = (y2 - y1)/(x2 - x1);
  if(Type)  Ys = y1 + k*(X - x1);
  else Ys = y2 - k*(X - x1);
  if((uint16_t)Ys > Y) return 1; 
  return 0;
}

///------- Extended addition
// inValue is an input value
// minStep is a start value of a step
// maxStep is a end value of a step
// Step is an initial step
// predPeriod is treshold period to start of updating the step

// AN ADDITION MODE USAGE:
// ex:: the borders between 0 and 100:
// PoolOfExits.TempA = exAddition(PoolOfExits.TempA, 0, 100, 1, 5 );

//A SUBSTRUCTION mode usage^
// ex:: the borders between 0 and 100:
// PoolOfExits.TempA = exAddition(PoolOfExits.TempA, 0, 100, -1, 5 );

//to reset use like this:  exAddition(0, 0, 0, 0, 0 );

#define STEPS_OF_ADD            40
#define ADD_TRESH               3
int32_t exAddition(int32_t inValue, int16_t minVal, int16_t maxVal, int16_t Step, uint16_t padPeriod){
static uint32_t   OLD_Counter; // we'll calculate the difference
static int16_t    OLD_Step = 0;
register int32_t ReturnValue;
static int16_t   step = 0;
static uint8_t   indexImp = 0;

 if(     (OLD_Step == Step) 
     && (Counter_TIM11 - OLD_Counter < padPeriod )
    ){
  ReturnValue = inValue + step;
  if(!(indexImp++ % ADD_TRESH))
        if(Step > 0)
            step += (maxVal - minVal)/STEPS_OF_ADD;
        else 
            step -= (maxVal - minVal)/STEPS_OF_ADD;
  }
  else{
   step = Step;
   ReturnValue = inValue + Step;
 }

 OLD_Counter =  Counter_TIM11;  
 OLD_Step = Step;
 if(ReturnValue > maxVal) ReturnValue = maxVal;
 if(ReturnValue < minVal) ReturnValue = minVal;
 return ReturnValue;
}

