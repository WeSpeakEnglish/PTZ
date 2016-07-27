#ifndef _INITIAL_H
#define _INITIAL_H

#include "stm32f7xx_hal.h"
#include "adc.h"
#include "can.h"
#include "dac.h"
#include "dma2d.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fmc.h"
#include "tw8819.h"
#include "ltdc.h"
#include "core.h"
#include "lcd.h"
#include "video.h"
#include "gui.h"
#include "variables.h"  
#include "delays.h"
#include "memory.h"
#include "timer13.h"
#include "timer14.h"
#include "timer11.h"
#include "stmpe811.h" 
#include "ff.h"
#include "rtc.h"
#include "leds.h"
#include "OSDFont.h"
#include "OSDBasic.h"
#include "OSDinitTable.h"
#include "DispInfo.h"
#include "sound.h"

#define PTZ_interface 1

#define SDRAM_BANK_ADDR         0xC0000000
#define LAYERS_SIZE             0x00177000
#define LAYER_1_OFFSET          0x00000000
#define LAYER_2_OFFSET          LAYER_1_OFFSET + LAYERS_SIZE            // 800x480x4 layer 1536000 offset
#define FB_565_1                LAYER_2_OFFSET + LAYERS_SIZE           // 800x480x2 layer
#define FB_565_2                FB_565_1 + LAYERS_SIZE/2            // 800x480x2 layer
#define LAYER_BACK_OFFSET       FB_565_2  + LAYERS_SIZE/2            // BACKGROUND
#define IMAGE_1_OFFSET          LAYER_BACK_OFFSET + LAYERS_SIZE         // big image 1   
#define IMAGE_2_OFFSET          IMAGE_1_OFFSET + LAYERS_SIZE            //big image 2
#define DisplayHEIGHT           480 // pixels
#define DisplayWIDTH            800 // pixels
#define PixelWIDTH              4   // bytes

#ifdef PTZ_interface
#include "PTZinterface.h" 
#endif

#ifdef DOR_interface
#include "userinterface.h" 
#endif
void InitPeriph(void);
#endif