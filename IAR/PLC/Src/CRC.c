#include "stm32f7xx.h"
#include "CRC.h"

uint8_t getCRCbusy()
	{
		return (uint8_t)CRC->IDR;
	}

void setCRCbusy(uint8_t val)
	{
		CRC->IDR=val;
	}

void CRCnext(uint32_t data)
	{
		CRC->DR=data;
	}

uint32_t getCRC()
	{
		return CRC->DR;
	}

void setCRC(uint32_t value)
	{
 static const uint32_t CrcTable[16] = {	0x00000000,0xB2B4BCB6,0x61A864DB,0xD31CD86D,
					0xC350C9B6,0x71E47500,0xA2F8AD6D,0x104C11DB,
					0x82608EDB,0x30D4326D,0xE3C8EA00,0x517C56B6,
					0x4130476D,0xF384FBDB,0x209823B6,0x922C9F00 };
		
		for(uint8_t i = 0; i < 8; ++i)
                  value = (value >> 4) ^ CrcTable[value & 0x0F];
		CRCnext(value ^ getCRC());
	}

void CRCreset()
	{
		CRC->CR|=CRC_CR_RESET;
	}
	
uint32_t CRCcalculateBuf(uint8_t *buf, uint16_t bufLen)
	{
uint32_t *ptr=(uint32_t *)buf;
		CRCreset();
		while(bufLen>3)
		{
			CRCnext(*ptr);
			ptr++;
			bufLen-=4;
		}
		if(bufLen>0)
		{
			uint32_t last=*ptr;
			for(uint8_t i=3;i >= bufLen; --i)
			{
				last&=~((uint32_t)(0xFF<<(i*8)));
				last |= (uint32_t)(0xBA<<(i*8));
			}
			CRCnext(last);
		}
		return getCRC();
	}

void CRCinit()
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
		CRCreset();
		CRC->IDR = 0;
	}


