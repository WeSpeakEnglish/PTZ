#include "fatfs.h"

uint8_t retSDRAMDISK;    /* Return value for SDRAMDISK */
char SDRAMDISK_Path[4];  /* SDRAMDISK logical drive path */
uint8_t retSD;    /* Return value for SD */
char SD_Path[4];  /* SD logical drive path */
uint8_t retUSBH;    /* Return value for USBH */
char USBH_Path[4];  /* USBH logical drive path */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SDRAMDISK driver ###########################*/
//  retSDRAMDISK = FATFS_LinkDriver(&SDRAMDISK_Driver, SDRAMDISK_Path);
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SD_Path);
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBH_Path);

  /* USER CODE BEGIN Init */
  /* additional user code for init */     
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
