#include <stdarg.h>
#include "calculations.h"
#include "initial.h"


static GUI_Object GUI_Objects[MAX_OBJECTS_Q];//@(DATA_IN_SDRAM+SDRAM_BANK_ADDR);


void GUI_Free(void){
  int i, j;
  for(i = 0; i < MAX_OBJECTS_Q;  i++) {
    GUI_Objects[i].existance = 0;
    GUI_Objects[i].type = 0;
    GUI_Objects[i].color = 0;
    GUI_Objects[i].z_index = 0;
    for (j = 0; j < MAX_PARAMS_Q; j++)
      GUI_Objects[i].params[j] = 0;
  }
  return;
}


GUI_Object*  GUI_SetObject(uint32_t typeObj, uint32_t colorObj, uint32_t z_Index, uint32_t NumbOfParms,...){
  va_list arg_ptr;
  static int i, j;      //indexes
  uint8_t takedFlag = 0;     // Take
  static GUI_Object NullObj;

  va_start(arg_ptr, NumbOfParms);

  NullObj.existance =0;

  for(i = 0; i < MAX_OBJECTS_Q; i++)       // find the nearest vacansion
   if (GUI_Objects[i].existance == 0x00){ 
    takedFlag = 1; 
    break;
  }
  if (takedFlag){ 
    GUI_Objects[i].existance = 1; // set the status of existance
    GUI_Objects[i].type = typeObj;
    GUI_Objects[i].color = colorObj;
    GUI_Objects[i].z_index = z_Index;

    for(j = 0; j < NumbOfParms; j++){
      GUI_Objects[i].params[j] = va_arg(arg_ptr, uint32_t);
    }
    va_end(arg_ptr);
    return &GUI_Objects[i]; 
  }

  return &NullObj;
}



void GUI_Release(){  // create GUI 
  static int i, j;       //indexes

  //if z-index == 0 eq hide
  for(j = 1; j < MAX_Z_INDEX; j++ ){

    for(i = 0; i < MAX_OBJECTS_Q; i++){
      if(GUI_Objects[i].z_index == j){
        if(GUI_Objects[i].existance){
          LCD_SetColorPixel(GUI_Objects[i].color); // set the font, color of font and the color of line
          switch(GUI_Objects[i].type){
          case LINE_TYPE:
            LCD_DrawLine(GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2], GUI_Objects[i].params[3]);
            break;
          case VERTICAL_LINE_TYPE:
            _HW_LCD_V_Line(GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            break;    
          case HORIZONTAL_LINE_TYPE:
            _HW_LCD_H_Line(GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            break;  
          case POLYGON_TYPE:
            break;
          case TEXT_STRING:
            LCD_InitParams(0, GUI_Objects[i].params[6], GUI_Objects[i].color, (sFONT*) GUI_Objects[i].params[5]);
            LCD_DisplayStringAt(GUI_Objects[i].params[0], GUI_Objects[i].params[1], (uint8_t*)GUI_Objects[i].params[2], (Text_AlignModeTypdef) GUI_Objects[i].params[3], (uint8_t)GUI_Objects[i].params[4]);
            break; 
          case CIRCLE_TYPE:
            LCD_DrawCircle(GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            break;
          case FILLED_CIRCLE_TYPE:
            LCD_DrawFullCircle(GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            break;
          case FILLED_RECT_TYPE:
            LCD_FillRect(GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2], GUI_Objects[i].params[3]);     
            break;
          case RECT_TYPE:
            LCD_DrawRect((uint16_t)GUI_Objects[i].params[0], (uint16_t)GUI_Objects[i].params[1], (uint16_t)GUI_Objects[i].params[2], (uint16_t)GUI_Objects[i].params[3]);     
            break;         
          case IMAGE_FAST_FILL:
            LCD_Fill_Image((ImageInfo *)GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            break;
          case IMAGE_WITH_TRANSP:
            LCD_Fill_ImageTRANSP((ImageInfo *)GUI_Objects[i].params[0], GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            break;
          case FILLED_TRIANGLE:   
            LCD_FillTriangleFAST((pPoint)GUI_Objects[i].params[0]); 
            break;
          case FILLED_POLY: 
            LCD_FillPolygon((pPoint)GUI_Objects[i].params[0], (uint16_t)GUI_Objects[i].params[1],0);
            break;
          case FILLED_POLY_FAST: 
            LCD_FillPolygon((pPoint)GUI_Objects[i].params[0], (uint16_t)GUI_Objects[i].params[1],1);
            break;         
          case POLY_TYPE: 
            LCD_DrawPolygon((pPoint)GUI_Objects[i].params[0], (uint16_t)GUI_Objects[i].params[1]);
            break;  
          case ROTATING_FILLED_POLY_TYPE: 
            StorePoly((pPoint)(GUI_Objects[i].params[0]),(uint16_t)(GUI_Objects[i].params[1])); 
            RotatePoly((pPoint)(GUI_Objects[i].params[0]),  (uint16_t)(GUI_Objects[i].params[1]),(pPoint)GUI_Objects[i].params[2], GUI_Objects[i].params[3]);
            LCD_FillPolygon((pPoint)GUI_Objects[i].params[0], (uint16_t)GUI_Objects[i].params[1],0); 
            RestorePoly((pPoint)(GUI_Objects[i].params[0]),(uint16_t)(GUI_Objects[i].params[1]));
            break;
          case ROTATING_FILLED_POLY_TYPE_FAST: 
           // HAL_Delay(3);
            StorePoly((pPoint)(GUI_Objects[i].params[0]),(uint16_t)(GUI_Objects[i].params[1])); 
          //  HAL_Delay(3);
            RotatePoly((pPoint)(GUI_Objects[i].params[0]),  (uint16_t)(GUI_Objects[i].params[1]),(pPoint)GUI_Objects[i].params[2], GUI_Objects[i].params[3]);
          //  HAL_Delay(3);
            LCD_FillPolygon((pPoint)GUI_Objects[i].params[0], (uint16_t)GUI_Objects[i].params[1],1); 
          //  HAL_Delay(3);
            RestorePoly((pPoint)(GUI_Objects[i].params[0]),(uint16_t)(GUI_Objects[i].params[1]));
          //  HAL_Delay(10);
            break;
          case FILLED_TRIANGLE_FAST :
            LCD_FillTriangleFAST((pPoint)GUI_Objects[i].params[0]);
            break;     
          case ROTATING_FILLED_TRIANGLE_FAST :
            StorePoly((pPoint)(GUI_Objects[i].params[0]),3); 
            RotatePoly((pPoint)(GUI_Objects[i].params[0]),  3,(pPoint)GUI_Objects[i].params[1], GUI_Objects[i].params[2]);
            LCD_FillTriangleFAST((pPoint)GUI_Objects[i].params[0]);
            RestorePoly((pPoint)(GUI_Objects[i].params[0]),3);
            break; 
          }
        } 
      }
    }
  }  
}

uint8_t GUI_Del_Obj(GUI_Object* deleteObj){
  deleteObj->existance = 0;  // delete existance
  return 0;
}

uint8_t GUI_Hide_Obj(GUI_Object* hideObj){
  hideObj->z_index = -1; // hide
  return 0;
}

uint8_t GUI_SetVisibility_Obj(GUI_Object* Obj, uint32_t Value){  // the alpha level of Object
  Obj->color = Value;  // hide object
  return 0;
}

void Show_GUI(void){

  if(!CAM_flag){ 
 //   RCC->PLLSAICFGR =0x44003300; // PLL Adjust
    GUI_Release(); 
    UserControlsShow();




    

    
    if(!LayerOfView){
      _HW_Fill_Display_From_Mem_565(SDRAM_BANK_ADDR + LAYER_1_OFFSET, SDRAM_BANK_ADDR + FB_565_1);
               WaitWhileDMA2D(MAXDELAY_DMA2D); 
               while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) { 
           if(!WaitWhileDMA2D(0))break;
           } 
      HAL_LTDC_SetAddress(&hltdc, SDRAM_BANK_ADDR + FB_565_1, 0); // set the present layer address
    }
    else{
      _HW_Fill_Display_From_Mem_565(SDRAM_BANK_ADDR + LAYER_2_OFFSET, SDRAM_BANK_ADDR + FB_565_2);
         WaitWhileDMA2D(MAXDELAY_DMA2D); 
           
      while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS)) { 
          if(!WaitWhileDMA2D(0))break;
         } 
      HAL_LTDC_SetAddress(&hltdc, SDRAM_BANK_ADDR + FB_565_2, 0); // set the present layer address
    }

    LayerOfView++;
    LayerOfView %= 2;
    _HW_Fill_Display_From_Mem(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, ProjectionLayerAddress[LayerOfView]);
  }          
}  



