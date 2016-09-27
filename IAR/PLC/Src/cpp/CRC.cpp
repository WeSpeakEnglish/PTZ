#include "CRC.h"

namespace RGsystem
{
	bool getCRCbusy()
	{
		return (bool)CRC->IDR;
	}

	void setCRCbusy(bool val)
	{
		CRC->IDR=val;
	}

	void CRCnext(uint data)
	{
		CRC->DR=data;
	}

	uint getCRC()
	{
		return CRC->DR;
	}

	void setCRC(uint value)
	{
		static const uint CrcTable[16] = {	0x00000000,0xB2B4BCB6,0x61A864DB,0xD31CD86D,
											0xC350C9B6,0x71E47500,0xA2F8AD6D,0x104C11DB,
											0x82608EDB,0x30D4326D,0xE3C8EA00,0x517C56B6,
											0x4130476D,0xF384FBDB,0x209823B6,0x922C9F00 };
		
		for(byte i=0;i<8;++i)value = (value >> 4) ^ CrcTable[value & 0x0F];
		CRCnext(value ^ getCRC());
	}

	void CRCreset()
	{
		CRC->CR|=CRC_CR_RESET;
	}
	
	uint CRCcalculateBuf(byte *buf, ushort bufLen)
	{
		uint *ptr=(uint*)buf;
		CRCreset();
		while(bufLen>3)
		{
			CRCnext(*ptr);
			ptr++;
			bufLen-=4;
		}
		if(bufLen>0)
		{
			uint last=*ptr;
			for(byte i=3;i>=bufLen;--i)
			{
				last&=~((uint)(0xFF<<(i*8)));
				last |= (uint)(0xBA<<(i*8));
			}
			CRCnext(last);
		}
		return getCRC();
	}

	void CRCinit()
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
		CRCreset();
		CRC->IDR=false;
	}
}
