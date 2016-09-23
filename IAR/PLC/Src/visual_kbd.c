#include "visual_kbd.h"
#include "initial.h"

uint8_t V_KBD_Fill_button = 0;

VKBD VisualKBD;



//the 
// 0 - lower english
// 1 - uppercase english
// 2 - lower russian
// 3 - upper russian
// 4 - numbers and symbols layout

const ZoneKBD ZonesKBD[]={
    {{18,207},{79,264},             ob00010011, 'q', '1'}, // #0 'q' and 'Q' valid on THIS KBD_layout; num 1
    {{88,207},{149,264},            ob00010011, 'w', '2'}, // #1  'w'   and 'W'; num 2
    {{158,207},{219,264},           ob00010011, 'e', '3'}, // #2  'e'  and 'E' ; num 3
    {{228,207},{289,264},           ob00010011, 'r', '4'}, // #3  'r'   and 'R'; num 4
    {{297,207},{358,264},           ob00010011, 't', '5'}, // #4 't'   and 'T' ; num 5
    {{367,207},{428,264},           ob00010011, 'y', '6'}, // #5 'y'   and 'Y' ; num 6
    {{437,207},{498,264},           ob00010011, 'u', '7'}, // #6 'u'   and 'U' ; num 7
    {{507,207},{568,264},           ob00010011, 'i', '8'}, // #7 'i'   and 'I' ; num 8
    {{577,207},{638,264},           ob00010011, 'o', '9'}, // #8 ; num 9
    {{647,207},{708,264},           ob00010011, 'p', '0'}, // #9 ; num 0
    {{717,207},{778,264},           ob00010011, 8  ,   8},   // #10 BACKSPACE command 
    {{45,272},{106,329},            ob00010011, 'a', '-'}, // #11 ; num '-'
    {{114,272},{176,329},           ob00010011, 's', '/'}, // #12 ; num '/'
    {{183,272},{244,329},           ob00010011, 'd', ':'}, // #13 ; num ':'
    {{252,272},{313,329},           ob00010011, 'f', ';'}, // #14 
    {{321,272},{382,329},           ob00010011, 'g', '('}, // #15  
    {{391,272},{452,329},           ob00010011, 'h', ')'}, // #16     
    {{460,272},{521,329},           ob00010011, 'j', '%'}, // #17      
    {{529,272},{590,329},           ob00010011, 'k', '&'}, // #18 
    {{599,272},{658,329},           ob00010011, 'l', '@'}, // #19
    {{668,272},{776,329},           ob00010011, 13,  '&'},  // #20 ENTER 
    {{18,336},{78,393},             ob00010011, 15,  '*'},  // #21 SHIFT IN / OUT (UP case or Down case)  
    {{87,336},{145,392},            ob00010011, 'z', '+'}, // #22 
    {{155,336},{214,392},           ob00010011, 'x', '_'}, // #23 
    {{222,336},{282,392},           ob00010011, 'c', '.'}, // #24   
    {{290,336},{349,392},           ob00010011, 'v', ','}, // #25  
    {{359,336},{417,392},           ob00010011, 'b', '?'}, // #26 
    {{426,336},{486,392},           ob00010011, 'n', '!'}, // #27    
    {{493,336},{552,392},           ob00010011, 'm', 0x27}, // #28   // '
    {{561,336},{621,392},           ob00010011, ',', 0x22}, // #29   // "
    {{630,336},{688,392},           ob00010011, '.', '='}, // #30 
    {{698,336},{776,392},           ob00011111, 15,  '#'},  // #31 SHIFT IN / OUT (UP case or Down case) , # at the number pallet
    {{18,401},{78,458},             ob00011111, 0x01, 0x03},// #32 command to switch layout to numbers / abc
    {{86,401},{146,458},            ob00011111, 0x02, 0x00},// #33 command to switch layout to another language 
    {{154,401},{621,458},           ob00011111, ' ', ' '}, // #34 SPACE
    {{631,401},{709,458},           ob00011111, 0x01, 0x03},// #35 command to switch layout to numbers  
    {{719,401},{776,458},           ob00011111, 16, 16},  // #36 EXIT 
//-----------------------------------------------------------------------------------
    {{18,207},{71,264},             ob00001100, 'é', 0x00},  // #37
    {{80,207},{133,264},            ob00001100, 'ö', 0x00},  // #38
    {{141,207},{193,264},           ob00001100, 'ó', 0x00},  // #39   
    {{204,207},{256,264},           ob00001100, 'ê', 0x00},  // #40
    {{265,207},{318,264},           ob00001100, 'å', 0x00},  // #41
    {{328,207},{379,264},           ob00001100, 'í', 0x00},  // #42     
    {{389,207},{442,264},           ob00001100, 'ã', 0x00},  // #43        
    {{452,207},{504,264},           ob00001100, 'ø', 0x00},  // #44   
    {{513,207},{565,264},           ob00001100, 'ù', 0x00},  // #45  
    {{575,207},{627,264},           ob00001100, 'ç', 0x00},  // #46 
    {{636,207},{689,264},           ob00001100, 'õ', 0x00},  // #47     
    {{698,207},{775,264},           ob00001100, 8  , 0x00},  // #48     
    {{37,272},{90,329},             ob00001100, 'ô' , 0x00}, // #49
    {{99,272},{152,329},            ob00001100, 'û' , 0x00}, // #50  
    {{160,272},{213,329},           ob00001100, 'â' , 0x00}, // #51  
    {{222,272},{276,329},           ob00001100, 'à' , 0x00}, // #52    
    {{284,272},{337,329},           ob00001100, 'ï' , 0x00}, // #53       
    {{346,272},{399,329},           ob00001100, 'ð' , 0x00}, // #54     
    {{407,272},{460,329},           ob00001100, 'î' , 0x00}, // #55  
    {{470,272},{523,329},           ob00001100, 'ë' , 0x00}, // #56     
    {{531,272},{584,329},           ob00001100, 'ä' , 0x00}, // #57    
    {{594,272},{646,329},           ob00001100, 'æ' , 0x00}, // #58     
    {{655,272},{708,329},           ob00001100, 'ý' , 0x00}, // #59
    {{716,272},{777,329},           ob00001100, 13 , 0x00},  // #60  // ENTER 
    {{18,336},{71,393},             ob00001100, 15 , 0x00},  // #61  // shift set up/down case    
    {{80,336},{132,393},            ob00001100, 'ÿ' , 0x00}, // #62    
    {{142,336},{195,393},           ob00001100, '÷' , 0x00}, // #63     
    {{203,336},{256,393},           ob00001100, 'ñ' , 0x00}, // #64
    {{265,336},{318,393},           ob00001100, 'ì' , 0x00}, // #65  //
    {{327,336},{380,393},           ob00001100, 'è' , 0x00}, // #66
    {{389,336},{442,393},           ob00001100, 'ò' , 0x00}, // #67   
    {{450,336},{503,393},           ob00001100, 'ü' , 0x00}, // #68 
    {{512,336},{565,393},           ob00001100, 'á' , 0x00}, // #69   
    {{574,336},{627,393},           ob00001100, 'þ' , 0x00}, // #70      
    {{636,336},{689,393},           ob00001100, 'ú' , 0x00}, // #71  
    {{697,336},{775,393},           ob00001100, 15 , 0x00},  // #72   
    {{0,0},{0,0},                   ob00011111, 0  , 0},     // #73 //the terminal zone  
};

#define ChangeColorMAIN        0xFFCCCCCC
#define ChangeColorADDITION    0xFF676766
#define DestColor              0xFFEEEEEE

void ChangeColorKBD(void){
// register uint32_t StartAddress; 
 volatile uint32_t * pChange;
 int16_t i, j, Offset;
 ZoneKBD * InsideZone;
 uint32_t ColorToChange;
 
 solveReturnCodeVisualKBD();
 InsideZone = (ZoneKBD *) &ZonesKBD[VisualKBD.ReturnZone];
   
 Offset = DisplayWIDTH - (InsideZone->RightBottom.X - InsideZone->LeftTop.X);

 pChange = (volatile uint32_t *)(ProjectionLayerAddress[LayerOfView] + 4 * InsideZone->LeftTop.X + 4 * DisplayWIDTH * InsideZone->LeftTop.Y);
 
 if(VisualKBD.ReturnZone != 10 &&
    VisualKBD.ReturnZone != 20 &&
    VisualKBD.ReturnZone != 21 &&  
    VisualKBD.ReturnZone != 31 &&   
    VisualKBD.ReturnZone != 32 &&    
    VisualKBD.ReturnZone != 33 &&     
    VisualKBD.ReturnZone != 35 &&  
    VisualKBD.ReturnZone != 36 &&
    VisualKBD.ReturnZone != 48 &&     
    VisualKBD.ReturnZone != 60 &&  
    VisualKBD.ReturnZone != 61    
   )
   ColorToChange = ChangeColorMAIN;
 else
   ColorToChange = ChangeColorADDITION;
 
 if(VisualKBD.Screen == 4) 
   if (VisualKBD.ReturnZone == 31 || VisualKBD.ReturnZone == 21)
     ColorToChange = ChangeColorMAIN;
 
 for(j = InsideZone->RightBottom.Y - InsideZone->LeftTop.Y; j > 0; j--){
   for(i = InsideZone->RightBottom.X - InsideZone->LeftTop.X; i > 0; i-- ){
     if(*pChange == ColorToChange){
      *pChange = DestColor;
     } 
     pChange++;   
   }
   pChange += Offset;
 }
 VisualKBD.Pressed = 1;
 return;
}

uint8_t solveReturnCodeVisualKBD(void){ //the handle of Touch Screen
  uint8_t Index;
  uint16_t x = Touch_Data.xp;
  uint16_t y = Touch_Data.yp;

    for(Index = 0; Index < (sizeof(ZonesKBD)/12) - 1; Index++){ //except the terminal zone
      if(ZonesKBD[Index].PagesActivities & (1<<VisualKBD.Screen)) // are we allowed here?
      if((x > ZonesKBD[Index].LeftTop.X  && x < ZonesKBD[Index].RightBottom.X)&&
            (y > ZonesKBD[Index].LeftTop.Y  && y < ZonesKBD[Index].RightBottom.Y)){
                break;
      }
    } 
  if(CheckAllowedZones(Index))VisualKBD.ReturnZone = Index;       //return zone
  else VisualKBD.ReturnZone = (sizeof(ZonesKBD)/12) - 1;   //there NO special zone has been found
  return Index;
}

inline uint8_t CheckAllowedZones(uint8_t IndexToCompare){
uint8_t i; //just variable for cycles
uint8_t Result = 0;
const uint8_t AllowedZonesDate[]={0,1,2,3,4,5,6,7,8,9,10,20,36,13,24,25};

switch(VisualKBD.Type){
  case KEYB_FULL:
        Result = 1;  
        break;
  case KEYB_NAME:
        Result = 1;  
        break;      
  case KEYB_DATE:
          for(i = 0; i < sizeof(AllowedZonesDate); i++){
            if(AllowedZonesDate[i] == IndexToCompare) Result = 1;
          }
          
         break;
         
 }
return Result;
}


void ResetParsedVisualKBD(void){
 if( VisualKBD.Parsed ) VisualKBD.Parsed = 0;
}