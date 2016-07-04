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

#include "memory.h"
#include "timer13.h"
#include "timer14.h"
#include "stmpe811.h" 
#include "ff.h"
#include "rtc.h"
#include "leds.h"
#define PTZ_interface 1

#ifdef PTZ_interface
#include "PTZinterface.h" 
#endif

#ifdef DOR_interface
#include "userinterface.h" 
#endif
void InitPeriph(void);
#endif