#include "stm32f7xx_hal.h"
#include "spi_mem.h"
#include "spi.h"
#include "delays.h"
#define sEE_SPI SPI2
#define SPI_handle hspi2;


uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t* NumByteToWrite)
{
  sEE_WriteEnable();
  sEE_CS_LOW();
  sEE_SendByte(sEE_CMD_WRITE);
  sEE_SendByte((WriteAddr & 0xFF00) >> 8);
  sEE_SendByte(WriteAddr & 0xFF);
  while ((*NumByteToWrite)--)
  {
    sEE_SendByte(*pBuffer);
    pBuffer++;
  }
  sEE_CS_HIGH();
  sEE_WaitEepromStandbyState();
  sEE_WriteDisable();
  return 0;
}

/**
  * @brief  Writes block of data to the EEPROM. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0, sEE_DataNum = 0;
  
  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;

  if (Addr == 0) 
  {
    if (NumOfPage == 0) 
    {
      sEE_DataNum = NumByteToWrite;      
      sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
    }
    else
    {
      while (NumOfPage--)
      {
        sEE_DataNum = sEE_PAGESIZE;
        sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }
      
      sEE_DataNum = NumOfSingle;
      sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
    }
  }
  else 
  {
    if (NumOfPage == 0) 
    {
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
        sEE_DataNum = count;
        sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
        WriteAddr +=  count;
        pBuffer += count;
        
        sEE_DataNum = temp;
        sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
      }
      else
      {
        sEE_DataNum = NumByteToWrite; 
        sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
      }
    }
    else 
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;

      sEE_DataNum = count;
        
      sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sEE_DataNum = sEE_PAGESIZE;
        
        sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sEE_DataNum = NumOfSingle;
        
        sEE_WritePage(pBuffer, WriteAddr, &sEE_DataNum);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer: pointer to the buffer that receives the data read from the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the EEPROM.
  * @retval None
  */
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{
  sEE_CS_LOW();
  sEE_SendByte(sEE_CMD_READ);
  sEE_SendByte((ReadAddr & 0xFF00) >> 8);
  sEE_SendByte(ReadAddr & 0xFF);
  while ((*NumByteToRead)--) 
  {
    *pBuffer = sEE_SendByte(sEE_DUMMY_BYTE);
    pBuffer++;
  }
  sEE_CS_HIGH();
  return 0;
}


/**
  * @brief  Reads a byte from the SPI EEPROM.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI EEPROM.
  */
uint8_t sEE_ReadByte(void)
{
  return (sEE_SendByte(sEE_DUMMY_BYTE));
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sEE_SendByte(uint8_t byte)
{
  uint8_t buff;
  SPI_transmitted = SPI_received = 0;
  HAL_SPI_TransmitReceive_DMA(&hspi2,&byte,&buff,1);
  WaitWhileSPI(MAXDELAY_SPI); //set the period
  while((!SPI_transmitted )||(!SPI_received)){
  if(!WaitWhileSPI(0)) return 0xFF;
  }

return buff;

}
/**
  * @brief  Enables the write access to the EEPROM.
  * @param  None
  * @retval None
  */
void sEE_WriteEnable(void)
{
  sEE_CS_LOW();
  sEE_SendByte(sEE_CMD_WREN);
  sEE_CS_HIGH();
}

/**
  * @brief  Disables the write access to the EEPROM.
  * @param  None
  * @retval None
  */
void sEE_WriteDisable(void)
{
  sEE_CS_LOW();
  sEE_SendByte(sEE_CMD_WRDI);
  sEE_CS_HIGH();
}

/**
  * @brief  Write new value in EEPROM Status Register.
  * @param  regval : new value of register
  * @retval None
  */
void sEE_WriteStatusRegister(uint8_t regval)
{
  sEE_CS_LOW();
  sEE_WriteEnable();
  sEE_SendByte(sEE_CMD_WRSR);
  sEE_SendByte(regval);  
  sEE_CS_HIGH();
}

/**
  * @brief  Read EEPROM Status Register.
  * @param  None
  * @retval The value of the Status register.
  */
uint8_t sEE_ReadStatusRegister(void)
{
  uint8_t sEEstatus = 0;
  sEE_CS_LOW();
  sEE_SendByte(sEE_CMD_RDSR);
  sEEstatus = sEE_SendByte(sEE_DUMMY_BYTE);
  sEE_CS_HIGH();
  
  return sEEstatus;
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the EEPROM's
  *         status register and loop until write operation has completed.
  * @param  None
  * @retval None
  */
uint32_t sEE_WaitEepromStandbyState(void)      
{
  uint8_t sEEstatus = 0;
  sEE_CS_LOW();
  sEE_SendByte(sEE_CMD_RDSR);
  do
  {
    sEEstatus = sEE_SendByte(sEE_DUMMY_BYTE);
  }
  while ((sEEstatus & sEE_WIP_FLAG) == SET); 
  sEE_CS_HIGH();
  return 0;
}
