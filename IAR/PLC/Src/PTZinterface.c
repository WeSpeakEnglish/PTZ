#include "PTZinterface.h"
#include "video.h"
#include "calculations.h"
#include "keyboard.h"
#include "visual_kbd.h"
#include "fonts.h"


#include "initial.h"
// (c) RA3TVD code, 2016

struct{
  uint8_t PressTransmiss; // pressure in the transmission
  uint8_t PressPneumosys; // pressuse in the pneumo system
  uint8_t PressEngineOil; // the pressure of oil
  uint8_t Speed;       //  the speed
  uint8_t RateEngine; // the RPM
  float Square;     // the square, which have been prepared
  float TimeOfRun;  // the total time of pass
  uint16_t Passes;  // number of passes
  float SquarePerHour;   // (100m)^2 per hour
  float PetrolPerHour;   // litres per hour
  float PetrolPerSquare; // litres per (100m)^2
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
} 
 PTZ;

static uint8_t StrTransmiss[] =           "     0.0";
static uint8_t StrPneumosys[] =           "     0.0";
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
static uint8_t StrCalibration[]  =        "0      "; //in the LEFT mode

static GUI_Object* Images[90]; 
static GUI_Object* Text[80];
//static GUI_Object* Lines[20];
static GUI_Object* Polygons[6];
//static GUI_Object* Triangles[8];

static uint8_t StrDate[11]="25.04.2016";
static uint8_t StrTime[9]="20:00:00";
static uint8_t StrDATA[8][16];



const uint8_t String_1[] = "Гружу изображения в память:";
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

volatile date_time_t dt;  

volatile Disp DISP;

struct{
  ImageInfo ImgArray[300];
  uint16_t Number;
}
IMAGES;


const Zone ZonesTS_0[]={
  {{13,405},{94,477},           ob11111111},  //0 BIG BOTTOM BTN#0  valid on  all the pages
  {{112,405},{193,477},         ob11111111},  //1 BIG BOTTOM BTN#1  
  {{211,405},{292,477},         ob11111111},  //2 BIG BOTTOM BTN#2 
  {{310,405},{391,477},         ob11111111},  //3 BIG BOTTOM BTN#3  
  {{409,405},{490,477},         ob11111111},  //4 BIG BOTTOM BTN#4 
  {{508,405},{589,477},         ob11111111},  //5 BIG BOTTOM BTN#5 
  {{607,405},{688,477},         ob11111111},  //6 BIG BOTTOM BTN#6  
  {{706,405},{787,477},         ob11111111},  //7 BIG BOTTOM BTN#7 
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
  {{76,211}, {140,271},         ob00010000},  //19 // alternative arm button 
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

static void actions(uint8_t deal);
void PenetrationRising(uint8_t Parm, uint8_t set); //we can set the parameters of this control

void CAM_ON_OFF(){
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

  Text[4] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 120, 100, StrTransmiss, RIGHT_MODE, 1, &RIAD_16pt,0);   // StrTransmiss
  Text[5] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 0, 7, 360, 205, StrSquare, RIGHT_MODE, 1, &RIAD_20pt,0); //square
  Text[6] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 120, 155, StrPneumosys, RIGHT_MODE, 1, &RIAD_16pt,0);
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
    PCF8563_read_datetime(&dt);
    RESmutex_1 = 0;

    GetDateToStr(StrDate, &dt);
    GetTimeToStr(StrTime, &dt);
    TimeIsReady = 0;
  }
  if(DISP.Event){ 
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
    switch(DISP.Screen){
    case 0:
      switch(DISP.TS_ZoneNumber){
      case 10:  ////toggle rectangles

        break;
      case 11:  //toggle rectangles

        break;   
      }
      break;
    case 1:
      DISP.SelectedField = 0; 
      switch(DISP.TS_ZoneNumber){
      case 16:  ////toggle rectangles
        break;
      case 17:  //toggle rectangles
        break; 
      case 22:  //toggle rectangles LEFT PRESSED
        break;
      case 20:  // SW pressed
        break;
      case 18:  // NE
        break;
      case 21:  //toggle rectangles
        break;   
      case 19:  //toggle rectangles
        break; 
      case 23:  //toggle rectangles
        break;   
      }
      break;
    case 2:  
      switch(DISP.TS_ZoneNumber){ 
      case 16: 
        break;  
      case 17:                   
        break;  
      case 22:
        break; 
      case 20:
        break;  
      case 18: //NE
        break;
      case 21:  //SE
        break;    
      case 19:  //toggle rectangles
        break; 
      case 23:  //RIGHT
        break;        
      }
      break;
    case 3:
      switch(DISP.TS_ZoneNumber){ 
      case 24: //TOP BRUSH
        break;
      case 25: //BOTTOM BRUSH
        break;  
      case 26: //TOP RATE
        break;
      case 27: //BOTTOM RATE
        break;      
      }
      break; 
    }


    DISP.Event = 0;
  }

  if(KB_Status.ENTER)KBD_Handle(KB_Status.code);

  if(DISP.ReleaseFlag){
    if((Touch_Data.status != TOUCH_PRESSED) && (!KB_Status.PRESSED))       ReleaseFunction();
    DISP.ReleaseFlag = 0;
  }

}

void PreLoadImages(void){

  IMAGES.Number = 0;
  // just simply load images into the memory 
  FillStructIMG(SDRAM_BANK_ADDR + IMAGE_1_OFFSET, 0,   291);

  //image 006.bmp like base  
  FillImageSoft(IMAGES.ImgArray[161].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, IMAGES.ImgArray[161].xsize, IMAGES.ImgArray[161].ysize); 
  return;
}

void KBD_Handle(uint8_t code){ //the handle of KBD
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
        CAM_ON_OFF();
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
        CAM_ON_OFF();
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
      if(DISP.Screen == 0) {

      }

      if(DISP.Screen == 3) {

      }  
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
      
      _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[291].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET);  //change the background
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
  case 4:
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
uint8_t WriteGo : 
    1; 
  }
  Flags={
    0  };

  switch(deal) {
  case 0: 
    if(!((DISP.Screen == 2)&&(Condition.activity)))DISP.Screen = 0;
    if(DISP.Screen == 2 && Condition.activity)Condition.butt_selected = 1;
    break;
  case 1: 
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
   if((DISP.Screen == 0)||(DISP.Screen == 1))DISP.Screen = 3;
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
    
   if(DISP.Screen == 3){
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
    }
    if(DISP.Screen == 2){
      if(Condition.activity){
        Condition.blink_button = Condition.butt_selected;
        Condition.BlinkCounter = 0;
        Condition.erase_flag = 0; // reset the erase token
      }
      else PenetrationRising(2, 1);
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
    }
    break;
  case 7:
    if(DISP.Screen == 2){
      if(Condition.activity != 0)PenetrationRising(4,0);
      else CAM_ON_OFF();
    }
    else
    if(DISP.Screen == 3){
        if(!PoolOfExits.EditShow && !PoolOfExits.TimeEdit) DISP.Screen = 0; //if we in the edit mode
        else{
         PoolOfExits.EditShow = 0; //else go from Edit mode and discard changes
         PoolOfExits.TimeEdit = 0; //and time edit mode too...
           } 
      }
    else{
      CAM_ON_OFF();
    }
    break;
  case 8:
    break;
  case 9:
    break;
  case 10:
    break;
  case 11: 
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
void Tests(void ){
  static uint32_t Counter = 0;
  if(StartTestFlag){
    if(Counter%8 == 0) Test1();
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
   ///////////////////////
   for(i = 0; i < 10; i++ ){
  if(PoolOfExits.EditShow && PoolOfExits.EntireSelect == j) { 
   if ((PoolOfExits.TempA + 5) / (100 - i*10)){ //math.round :) TempA instead of PTZ.Hydroexits[j].A
        LCD_SetColorPixel(BHE_active_color);
      }
      else{
        LCD_SetColorPixel(BHE_inactive_color );
      }
      LCD_FillRect(Coords[j].X + EditStrokePaddingX_A , 
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY, 
                   Coords[j].X + EditStrokePaddingX_A + BHE_width_element,
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY + BHE_height_element);

      if ((PoolOfExits.TempB + 5) / (100 - i*10)){ //math.round :) TempB instead of PTZ.Hydroexits[j].B
        LCD_SetColorPixel(BHE_active_color);
      }
      else{
        LCD_SetColorPixel(BHE_inactive_color );
      }
      LCD_FillRect(Coords[j].X + EditStrokePaddingX_B , 
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY, 
                   Coords[j].X + EditStrokePaddingX_B + BHE_width_element,
                   Coords[j].Y + i * (BHE_height_element + GAP_elements) + ShowStrokePaddingY + BHE_height_element);
   }   
   else{
    LCD_SetColorPixel(BHE_inactive_color);
    LCD_FillRect(Coords[j].X    + ShowStrokePaddingX , 
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
      LCD_FillRect(pos_x_controlHE + j * x_stepHE, pos_y_controlHE + i*10, pos_x_controlHE + longierSquareW + j * x_stepHE, pos_y_controlHE + y_stepHE + i*10);

      if ((PTZ.Hydroexits[j].B + 5) / (100 - i*10)){ //math.round :)
        LCD_SetColorPixel(0xFFFF3333);
      }
      else{
        LCD_SetColorPixel(0xFF999999);
      }
      LCD_FillRect(pos_x_controlHE + j * x_stepHE + x_step_secondHE, pos_y_controlHE + i*10, pos_x_controlHE + shorherSquareW + j * x_stepHE + x_step_secondHE, pos_y_controlHE + y_stepHE + i*10);

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
          LCD_Fill_Image(&IMAGES.ImgArray[9], 203, 394);
        } 
        LCD_Fill_Image(&IMAGES.ImgArray[241], 301, 162);
      }
    }

    if(Condition.stop || Condition.set_stop){
      if(((Condition.BlinkCounter % BLINKPERIOD) < BLINKDUTY) || Condition.set_stop) {
        if(!Condition.activity ) { 
          LineColours[1] = 0xFFFF0000;
          LCD_Fill_Image(&IMAGES.ImgArray[16], 400, 394);
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
     if(V_KBD_Fill_button)ChangeColor(0xFF595959, 0xFF222222, &ZonesTS_0[V_KBD_Fill_button]);
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
    for(i = startIndex; i < stopIndex+1; i++){
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

