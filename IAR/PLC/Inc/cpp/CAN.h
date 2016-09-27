#pragma once

#include "UCsystem.h"

#define CAN1_ID_EXT (CAN1->sFIFOMailBox[0].RIR>>3)
#define CAN1_ID_STD (CAN1->sFIFOMailBox[0].RIR>>21)
#define CAN1_RTR ((CAN1->sFIFOMailBox[0].RIR>>1)&1)
#define CAN1_DLC (CAN1->sFIFOMailBox[0].RDTR&0x0F)
#define CAN1_Data ((byte*)0x400065B8)
#define CAN1_Data_L (CAN1->sFIFOMailBox[0].RDLR)
#define CAN1_Data_H (CAN1->sFIFOMailBox[0].RDHR)

#define CAN2_ID_EXT (CAN2->sFIFOMailBox[0].RIR>>3)
#define CAN2_ID_STD (CAN2->sFIFOMailBox[0].RIR>>21)
#define CAN2_RTR ((CAN2->sFIFOMailBox[0].RIR>>1)&1)
#define CAN2_DLC (CAN2->sFIFOMailBox[0].RDTR&0x0F)
#define CAN2_Data ((byte*)0x400069B8)
#define CAN2_Data_L (CAN2->sFIFOMailBox[0].RDLR)
#define CAN2_Data_H (CAN2->sFIFOMailBox[0].RDHR)

namespace UCsystem
{
	extern "C"
	{
		void CAN1_TX_IRQHandler();
		void CAN1_RX0_IRQHandler();
		void CAN2_TX_IRQHandler();
		void CAN2_RX0_IRQHandler();
	}

	void CAN1TransmitByte(uint16 _ID, uint8 data);
	void CAN1TransmitPack(ushort _ID, byte * data, byte dataLen);
	void CAN2TransmitByte(uint16 _ID, uint8 data);
	void CAN2TransmitPack(ushort _ID, byte * data, byte data_len);
	bool CAN1ready();
	bool CAN2ready();
	void CANinit();
}
