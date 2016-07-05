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

#include "lm75.h"
#include "spi_mem.h"


GUI_Object* Images[300]; 
GUI_Object* Text[80];
GUI_Object* Rect1;
GUI_Object* Poly2;
GUI_Object* Poly3;
uint8_t StrDate[11]="25.04.2016";
uint8_t StrTime[9]="20:00:00";
uint8_t StrDATA[16][8];



volatile uint8_t UpdateScreen = 0;
volatile uint8_t CAM_flag = 0;

uint8_t Temp8;

uint8_t RateChange = 0;

volatile date_time_t dt;  

volatile Disp DISP;

struct{
ImageInfo ImgArray[300];
uint16_t Number;
}IMAGES;




  const Zone ZonesTS_0[]={
   {{900,900},{900,900}},  //0  
   {{900,900},{900,900}},   //1  
   {{900,900},{900,900}},   //2  
   {{900,900},{900,900}},   //3  
   {{900,900},{900,900}},   //4  
   {{900,900},{900,900}},  //5 
   {{900,900},{900,900}},  //6  
   {{900,900},{900,900}},   //7  
   {{900,900},{900,900}},  //8  
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
   {{690,415},{784,498}},   //28  CAM
 };   

void Load_GUI_0(void){ 
  
 uint16_t x = 0;
 //uint16_t i = 0;
 
 DISP.Screen = 0; 

 
  GUI_Free();
 
  Text[2] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 40, 10, StrTime, LEFT_MODE, 1, &GOST_B_23_var,0);   // watch
  Text[3] = GUI_SetObject(TEXT_STRING ,0xFFFFFFFF, 3, 7, 700, 10, StrDate, LEFT_MODE, 1, &GOST_B_23_var,0);   // date

  Images[0] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[198], 4   , 394); //HOME+ 99*i
  Images[1] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[190], 103 , 394); //tractor in the gear
  Images[2] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[106], 202 , 394); //turn up/dowm
  Images[3] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[107], 301 , 394); //hydrocilinder
  Images[4] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[108], 400 , 394); //microchip
  Images[5] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[196], 499 , 394); //piece of... with green
  Images[6] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[109], 598 , 394); //tractor and wrench
  Images[7] = GUI_SetObject(IMAGE_FAST_FILL,0, 1, 3, &IMAGES.ImgArray[110], 697 , 394); // videocam
  
  Images[8] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[175], 126 , 0); // the signal red sign
  Images[9] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[176], 126 + 77 , 0); // the sattellite red sign
  Images[10] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[177], 126 + 77*2 , 0); // the termometer red sign
  Images[11] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[187], 126 + 77*3 , 0); // the pressure red sign
  Images[12] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[188], 126 + 77*4 , 0); // the valve red sign
  Images[13] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[189], 126 + 77*5 , 0); // the filter red sign
  Images[14] = GUI_SetObject(IMAGE_FAST_FILL,0, 0, 3, &IMAGES.ImgArray[181], 126 + 77*6 , 0); // the filter red sign
}

void Run_GUI(void){


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
          break;
        case 1:  //toggle index of button
          break;  
        case 2:  //toggle index of button
          break;  
        case 3:  //toggle index of button
          break; 
        case 4:
          break;
        case 5:
          break;  
        case 6:
          break;   
        case 7:  //toggle index of button    
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
        case 28: ///!!!-----------------------------------------------------------------------
          if(!CAM_flag) {
            CAM_flag = 1;
            VideoCAMOnOff(4, 1); //number four on
          }
          else {
            VideoCAMOnOff(4, 0); //number four off
            CAM_flag = 0;
          }
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
    if(DISP.ReleaseFlag){
     if((Touch_Data.status != TOUCH_PRESSED) && (!KB_Status.PRESSED))ReleaseFunction();
     DISP.ReleaseFlag = 0;
   }
}

void PreLoadImages(uint32_t BaseAddr){
  uint32_t address;

  IMAGES.Number = 0;
  // just simply load images into the memory 
  address = BaseAddr + IMAGE_1_OFFSET;
  address = FillStructIMG(address, 0,   286);
  
   //image 006.bmp like base  
   FillImageSoft(IMAGES.ImgArray[161].address, BaseAddr + LAYER_BACK_OFFSET, IMAGES.ImgArray[161].xsize, IMAGES.ImgArray[161].ysize); 
   FillImageSoft(IMAGES.ImgArray[161].address, BaseAddr + LAYER_1_OFFSET, IMAGES.ImgArray[161].xsize, IMAGES.ImgArray[161].ysize);
   FillImageSoft(IMAGES.ImgArray[161].address, BaseAddr + LAYER_2_OFFSET, IMAGES.ImgArray[161].xsize, IMAGES.ImgArray[161].ysize);

 return;
}

void KBD_Handle(uint8_t code){ //the handle of KBD
  
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
          case 3: 
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
  //  DISP.Event = 1;
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
              (y > ZonesTS_0[Index].LeftTop.Y  && y < ZonesTS_0[Index].RightBottom.Y)) DISP.TS_ZoneNumber = Index;
      if(Index != 28 && CAM_flag) DISP.TS_ZoneNumber = 100;
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
 
  for(i = 6; i < sizeof(Images)/4; i++ )  {
  
  }
  for(i = 4; i < sizeof(Text)/4; i++ )  {

  }
  switch(DISP.Screen){
    case 0:
            break; 
    case 1:
            break;  
    case 2:
            break;
    case 3:
            break;          
  }

}

void ReleaseFunction(void){
  switch(DISP.ReleaseTask){
   case 1 :

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


void Test1(void){
static uint8_t Toggle1;

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
}
///!------------------------------------------- Do Not Modify ------------------------------(c)RA3TVD-----
uint32_t FillStructIMG(uint32_t address, uint16_t startIndex, uint16_t stopIndex){
  uint16_t i = 0;
  static uint8_t Name[]="000.bmp";
  ImgSize SizesIMG;
  
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
   IMAGES.Number++;
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