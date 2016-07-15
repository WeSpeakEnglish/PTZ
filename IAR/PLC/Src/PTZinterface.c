#include "PTZinterface.h"
#include "video.h"
#include "calculations.h"
#include "core.h"
#include "keyboard.h"
#include "sound.h"
#include "fonts.h"
#include "timer14.h"
#include "timer13.h"
#include "ltdc.h"
#include "memory.h"
#include "lm75.h"
#include "spi_mem.h"
#include "ff.h"


struct{
 uint8_t PressTransmiss;
 uint8_t PressPneumosys;
 uint8_t PressEngineOil;
 uint8_t Speed;
 uint8_t RateEngine;
 float Square;
 float TimeOfRun;
 uint16_t Passes;
 float SquarePerHour;
 float PetrolPerHour;
 float PetrolPerSquare;
 
}PTZ;
/// CONST STRs
//const uint8_t str1[] = "атм"; //1
//const uint8_t str2[] = "км";  //2
//const uint8_t str3[] = "rpm";  //3
//const uint8_t str4[] = "ч";  //3

static uint8_t StrTransmiss[] =           "     0.0";
static uint8_t StrPneumosys[] =           "     0.0";
static uint8_t StrPressEngineOil[] =      "     0.0";
static uint8_t StrSpeed[] =               "       0";
static uint8_t StrRPM[] =                 "     0.0";
static uint8_t StrTIME[] =                "     0.0";
static uint8_t StrSquare[] =              "     0.0";
static uint8_t StrPasses[] =              "       0";
static uint8_t StrSquarePerHour[] =       "     0.0";
static uint8_t PetrolPerHour[] =          "     0.0";
static uint8_t PetrolPerSquare[] =        "     0.0";

static GUI_Object* Images[90]; 

static GUI_Object* Text[40];
//static GUI_Object* Lines[20];
static GUI_Object* Polygons[10];
//static GUI_Object* Triangles[8];

static uint8_t StrDate[11]="25.04.2016";
static uint8_t StrTime[9]="20:00:00";
static uint8_t StrDATA[8][16];



const uint8_t String_1[] = "Загрузка изображений в память:";
//const uint8_t String_2[] = "атм";

const Point Poly1_points[4]={{227,311},{363,317},{355,311},{363,304}};
//const Point Poly1_points[4]={{227,311},{363,317},{363,304}};
const Point Poly2_points[4]={{411,198},{477,202},{472,198},{477,194}};
//const Point Poly2_points[4]={{411,198},{477,202},{477,194}};
//const Point Poly3_points[4]={{662,123},{704,120},{701,123},{704,126}};
const Point Poly3_points[4]={{662,123},{704,120},{704,126}};
//const Point Poly4_points[4]={{662,311},{704,308},{701,311},{704,314}};
const Point Poly4_points[4]={{662,311},{704,308},{704,314}};
//const Point Poly5_points[4]={{662,311},{704,308},{704,314}};

const Point TurnCenter1 = {399,303};
const Point TurnCenter2 = {520,200};
const Point TurnCenter3 = {710,123};
const Point TurnCenter4 = {710,311};

volatile uint8_t UpdateScreen = 0;


uint8_t Temp8;
uint8_t StartTestFlag = 1;

uint8_t RateChange = 0;

volatile date_time_t dt;  

volatile Disp DISP;

struct{
ImageInfo ImgArray[300];
uint16_t Number;
}IMAGES;




  const Zone ZonesTS_0[]={
    {{13,405},{94,477}},  // BIG BOTTOM BTN#0  
   {{112,405},{193,477}}, // BIG BOTTOM BTN#1  
   {{211,405},{292,477}},  // BIG BOTTOM BTN#2 
   {{310,405},{391,477}},  // BIG BOTTOM BTN#3  
   {{409,405},{490,477}},  // BIG BOTTOM BTN#4 
   {{508,405},{589,477}},  // BIG BOTTOM BTN#5 
   {{607,405},{688,477}},  // BIG BOTTOM BTN#6  
   {{706,405},{787,477}},  // BIG BOTTOM BTN#7 
   {{900,900},{900,900}},  //  
   {{900,900},{900,900}},  //9  
   {{900,900},{900,900}},  //10  
   {{900,900},{900,900}},  //11  
   {{900,900},{900,900}},   //12  
   {{900,900},{900,900}},  //13  
   {{900,900},{900,900}},  //14  
   {{900,900},{900,900}},  //15  
   
   {{900,900},{900,900}},   //16  
   {{900,900},{900,900}},   //17  
   {{900,900},{900,900}},   //18  
   
   {{900,900},{900,900}},   //19  
   {{900,900},{900,900}},   //20  
   {{900,900},{900,900}},   //21  
   
   {{900,900},{900,900}},   //22  
   {{900,900},{900,900}},   //23 
   
   {{900,900},{900,900}},    //24 
   {{900,900},{900,900}},   //25  
   
   {{900,900},{900,900}},    //26  
   {{900,900},{900,900}},   //27 
   {{900,900},{900,900}},   //28  CAM
 };   
 
static void actions(uint8_t deal);

void Load_GUI_0(void){ 
//
 
 DISP.Screen = 0; 

 
  GUI_Free();
  Text[0] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 140, 240, (uint32_t)String_1, LEFT_MODE, 1, &GOST_B_23_var,0);   // watch
  Itoa(StrDATA[0],0);
  Text[1] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 400, 240, (uint32_t)StrDATA[0], LEFT_MODE, 1, &GOST_B_23_var,0);   // watch
 
  PreLoadImages();
 
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
  
  Images[0] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 1, 3, &IMAGES.ImgArray[287], 3   , 394); //HOME+ 99*i
 // Images[1] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 102 , 394); //tractor in the gear
 // Images[2] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 201 , 394); //turn up/dowm
 // Images[3] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 300 , 394); //hydrocilinder
//  Images[4] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 399 , 394); //microchip
   Images[5] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[197], 497 , 394); //piece of... with red
//  Images[6] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 597 , 394); //tractor and wrench
 // Images[7] = GUI_SetObject(IMAGE_WITH_TRANSP,0xFF121211, 0, 3, &IMAGES.ImgArray[287], 696 , 394); // videocam
  
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

  Polygons[0] = GUI_SetObject(ROTATING_FILLED_POLY_TYPE_FAST, 0xFFFF0000, 2, 4, Poly1_points,4, &TurnCenter1, 4000); // BIG ARROW with speed Point* pToPoints, uint8_t NumbOfPoints, const pPoint Origin, uint32_t angle_deg (*0.001 degrees)
  Polygons[1] = GUI_SetObject(ROTATING_FILLED_POLY_TYPE_FAST, 0xFFFF0000, 2, 4, Poly2_points,4, &TurnCenter2, 53500); //the middle arrow at 53.5 degrees
  Polygons[2] = GUI_SetObject(ROTATING_FILLED_TRIANGLE_FAST, 0xFFFF0000, 2, 3, Poly3_points, &TurnCenter3, 0); //the small arrow 
  Polygons[3] = GUI_SetObject(ROTATING_FILLED_TRIANGLE_FAST, 0xFFFF0000, 2, 3, Poly4_points, &TurnCenter4, 0); //the second small arrow 
  
//  Triangles[0] =  GUI_SetObject(ROTATING_FILLED_TRIANGLE_FAST, 0xFFFF0000, 2, 3, Poly5_points,&TurnCenter4,20000); //the second small arrow
  
  

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
          break;
        case 9:  //toggle index of button
          break;  
        case 12: //pressed topping
          break; 
        case 13: //pressed blade front
          break;   
        case 14: //pressed blade front
          break;  
        case 15: //pressed blade side
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
  
  DISP.TS_ZoneNumber = -1; 
  DISP.Event = 0;
  }
 
 if(KB_Status.ENTER)KBD_Handle(KB_Status.code);
 
    if(DISP.ReleaseFlag){
     if((Touch_Data.status != TOUCH_PRESSED) && (!KB_Status.PRESSED))
       ReleaseFunction();
     DISP.ReleaseFlag = 0;
   }
   
}

void PreLoadImages(void){


  IMAGES.Number = 0;
  // just simply load images into the memory 
  FillStructIMG(SDRAM_BANK_ADDR + IMAGE_1_OFFSET, 0,   287);
  
   //image 006.bmp like base  
   FillImageSoft(IMAGES.ImgArray[161].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, IMAGES.ImgArray[161].xsize, IMAGES.ImgArray[161].ysize); 
 //  FillImageSoft(IMAGES.ImgArray[164].address, SDRAM_BANK_ADDR + LAYER_BACK1_OFFSET, IMAGES.ImgArray[164].xsize, IMAGES.ImgArray[164].ysize);

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
          case 0x38:
           actions(code - 0x31);
           break;
          }
          break;
        case 1:
          switch(KB_Status.code){
               case 0x34:

                 break;
                case 0x35:  

                break; 
               case 0x36:

                 break;
               case 0x37:  

                 break;                   
          }
          break;
          case 2: 
  switch(KB_Status.code){
               case 0x34:

                 break;
               case 0x35:  

                 break; 
               case 0x36:

                 break;
               case 0x37:  

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
      if(DISP.Screen == 1 || DISP.Screen == 2)
      {

      }
         
      
    if(DISP.Screen == 3) {

       }   
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
  //UpdateScreen = 1;

      } 

 return;
}

void ViewScreen(void){
 uint16_t i;
 static uint8_t OldScreen = 0;

 if(OldScreen != DISP.Screen){
  for(i = 4; i < sizeof(Text)/4 ; i++){
   Text[i]->z_index = 0;
 } 
   for(i = 4; i < sizeof(Images)/4 ; i++){
   Images[i]->z_index = 0;
 } 
  switch(DISP.Screen){
    case 0:
       Text[4]->z_index = 3; // StrTransmiss
       Text[6]->z_index = 3; // StrPneumosys
       Text[8]->z_index = 3; // StrPressEngineOil
       Text[10]->z_index = 3; // StrTIME
       Text[12]->z_index = 3; // StrSpeed
       Text[14]->z_index = 3; // StrRPM
     //  Images[11]->params[0] = (uint32_t)&IMAGES.ImgArray[187]; // the pressure red sign
    //   Images[12]->params[0] = (uint32_t)&IMAGES.ImgArray[188]; // the valve red sign
    //   Images[13]->params[0] = (uint32_t)&IMAGES.ImgArray[189]; // the filter red sign
   //    Images[31]-> z_index = 1;            // the fuel sign
       Polygons[0]-> z_index = 1; // BIG ARROW with speed 
       Polygons[1]-> z_index = 1; //the middle arrow 
       Polygons[2]-> z_index = 1; //the small arrow 
       Polygons[3]-> z_index = 1; //the second small arrow 
       StartTestFlag = 1;
//       bgPointer = SDRAM_BANK_ADDR + LAYER_BACK0_OFFSET;
       Images[0]->params[1] = 3;
       Images[0]->z_index = 1;
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
       Polygons[0]-> z_index = 0; // BIG ARROW with speed 
       Polygons[1]-> z_index = 0; //the middle arrow 
       Polygons[2]-> z_index = 0; //the small arrow 
       Polygons[3]-> z_index = 0; //the second small arrow 
       StartTestFlag = 0;
  //     bgPointer = SDRAM_BANK_ADDR + LAYER_BACK1_OFFSET;
       Images[0]->params[1] = 102;
       Images[0]->z_index = 1;
       _HW_Fill_RGB888_To_ARGB8888(IMAGES.ImgArray[164].address, SDRAM_BANK_ADDR + LAYER_BACK_OFFSET);
            break;  
    case 2:
            break;
    case 3:
            break;          
  }
  OldScreen = DISP.Screen;
 }
}

void ReleaseFunction(void){
  switch(DISP.ReleaseTask){
   case 1 :
//     Images[0]->z_index = 0;
 //    Images[1]->z_index = 0;
 //    Images[2]->z_index = 0;
 //    Images[3]->z_index = 0;
 //    Images[4]->z_index = 0;
 //    Images[5]->z_index = 0;
 //   Images[6]->z_index = 0;
     
         break;  
   case 2 :

         break;        
   case 3 : 


         break;
  } 

  DISP.ReleaseTask = 0;
}


void KBD_Repeat_Handle(void){
      
   return;
}



void actions(uint8_t deal){
static  struct {
    uint8_t WriteGo : 1; 
  
}Flags={0};

  
  
  switch(deal) {
    case 0:
    case 1: 
    case 2:
    case 3:
    case 4: DISP.Screen = deal;
           break;
    case 5:
      if(DISP.Screen == 0){
           Flags.WriteGo = !Flags.WriteGo;
           Images[5] ->z_index = Flags.WriteGo;
      }
           break;
    case 6: 
          DISP.Screen = deal;
//     Images[deal]->z_index = 1; //show frame on the BTN6
//     DISP.ReleaseTask = 1;
      break;
    case 7:
     if(!CAM_flag) {
        CAM_flag = 1;
        VideoCAMOnOff(4, 1); //number four on
      }
       else {
        VideoCAMOnOff(4, 0); //number four off
        CAM_flag = 0;
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
  Images[8]->z_index =  Toggle1;
  Images[9]->z_index =  Toggle1;
  Images[10]->z_index = Toggle1;
  Images[11]->z_index = Toggle1;
  Images[12]->z_index = Toggle1;
  Images[13]->z_index = Toggle1;
  Images[14]->z_index = Toggle1;

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
///!------------------------------------------- Do Not Modify ------------------------------(c)RA3TVD-----

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
  if(SetValue > 99) Images[32]->z_index = 1;
  else Images[32]->z_index = 0;
  }
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
    if(UpdateScreen){
  Itoa(StrDATA[0],i + 1);    
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