#include "CAN.h"

namespace UCsystem
{	
	void CAN1_TX_IRQHandler()
	{
		CAN1->TSR = CAN1->TSR;
		RGsystem::sendMessageBody();
	}

	void CAN1_RX0_IRQHandler()// Patch
	{
		byte CANdataSaved[8];//CANDataSaved
		volatile ushort ID = CAN1_ID_STD;
		if(RGsystem::projectDevice > 0)
		{
			if(RGsystem::isRGdevice(ID))
			{
				for(byte i=0; i < 8; ++i) CANdataSaved[i] = CAN1_Data[i];
				for(byte i=0; i < RGsystem::deviceCount; ++i) RGsystem::projectDevice[i].device->CANpackReceived(CANdataSaved);
			}
		}
		CAN1->RF0R = CAN_RF0R_RFOM0;
	}
	
	void CAN2_TX_IRQHandler()
	{
		CAN2->TSR = CAN2->TSR;
		RGsystem::sendMessageBody();
	}

	void CAN2_RX0_IRQHandler()// Patch
	{
		byte CANDataSaved[8];
		for(byte i = 0; i < 8; ++i)CANDataSaved[i] = CAN2_Data[i];
		CAN2->RF0R = CAN_RF0R_RFOM0;
	}
	
	void CAN1TransmitByte(uint16 _ID, uint8 data)
	{
		if ((CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0)
		{
			byte index = CAN1->TSR = (CAN1->TSR & CAN_TSR_CODE) >> 24;
			CAN1->sTxMailBox[index].TDTR = 1;// data length
			CAN1->sTxMailBox[index].TDLR = data;// data fill
			CAN1->sTxMailBox[index].TIR = (uint32_t)(_ID << 21 | CAN_TI0R_TXRQ);// Id field (0x7F2) and request
		}
	}
	
	void CAN1TransmitPack(ushort _ID, byte * data, byte dataLen)
	{
		if (dataLen > 8)dataLen = 8;
		if ((CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0)
		{
			byte index = CAN1->TSR = (CAN1->TSR & CAN_TSR_CODE) >> 24;
			CAN1->sTxMailBox[index].TDTR = dataLen;// data length
			CAN1->sTxMailBox[index].TDLR = 0;// data fill
			CAN1->sTxMailBox[index].TDHR = 0;// data fill
			for (int i = 0; i < dataLen; ++i)
			{
				if (i < 4)CAN1->sTxMailBox[index].TDLR += (data[i] << (i * 8));// data fill
				if (i > 3)CAN1->sTxMailBox[index].TDHR += (data[i] << ((i - 4) * 8));// data fill
			}
			CAN1->sTxMailBox[index].TIR = (uint32_t)(_ID << 21 | CAN_TI0R_TXRQ);// Id field (0x7F2) and request
		}
	}
	
	void CAN2TransmitByte(uint16 _ID, uint8 data)
	{
		if ((CAN2->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0)
		{
			byte index = CAN2->TSR = (CAN2->TSR & CAN_TSR_CODE) >> 24;
			CAN2->sTxMailBox[index].TDTR = 1;// data length
			CAN2->sTxMailBox[index].TDLR = data;// data fill
			CAN2->sTxMailBox[index].TIR = (uint32_t)(_ID << 21 | CAN_TI0R_TXRQ);// Id field (0x7F2) and request
		}
	}

	void CAN2TransmitPack(ushort _ID, byte * data, byte dataLen)
	{
		if (dataLen > 8) dataLen = 8;
		if ((CAN2->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0)
		{
			byte index = CAN2->TSR = (CAN2->TSR & CAN_TSR_CODE) >> 24;
			CAN2->sTxMailBox[index].TDTR = dataLen;// data length
			CAN2->sTxMailBox[index].TDLR = 0;// data fill
			CAN2->sTxMailBox[index].TDHR = 0;// data fill
			for (int i = 0; i < dataLen; ++i)
			{
				if (i < 4)CAN2->sTxMailBox[index].TDLR += (data[i] << (i * 8));// data fill
				if (i > 3)CAN2->sTxMailBox[index].TDHR += (data[i] << ((i - 4) * 8));// data fill
			}
			CAN2->sTxMailBox[index].TIR = (uint32_t)(_ID << 21 | CAN_TI0R_TXRQ);// Id field (0x7F2) and request
		}
	}
	
	bool CAN1ready()
	{
		return (CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0;
	}
	
	bool CAN2ready()
	{
		return (CAN2->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0;
	}

	void CANinit()
	{
		RCC->APB1ENR |= RCC_APB1ENR_CAN2EN | RCC_APB1ENR_CAN1EN;// Both bus for clock to transceiver registers
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;
		GPIOA->MODER |= MODERSet(11, ModeAf) | MODERSet(12, ModeAf);// CAN 1
		AFreg(GPIOA) |= AFset(11, 9) | AFset(12, 9);
		GPIOB->MODER |= MODERSet(5,ModeAf)|MODERSet(6,ModeAf);// CAN 2
		AFreg(GPIOB) |= AFset(5,9)|AFset(6,9);
		
		CAN1->MCR = 0x8000;//Software reset
		CAN2->MCR=0x8000;

		CAN1->MCR |= CAN_MCR_INRQ;// init mode on
		CAN2->MCR |= CAN_MCR_INRQ;
		
		CAN1->MCR |= CAN_MCR_TXFP;// transmit FIFO priority to chronological
		CAN2->MCR |= CAN_MCR_TXFP;
		
		CAN1->IER |= CAN_IER_TMEIE;// transmit FIFO priority to chronological
		CAN2->IER |= CAN_IER_TMEIE;
		
		//while(!(CAN1->MSR |= CAN_MSR_INAK));// wait for init mode ready
		while(!(CAN1->MSR |= CAN_MSR_INAK) && !(CAN2->MSR |= CAN_MSR_INAK));// wait for init mode ready

		CAN1->MCR &= ~CAN_MCR_SLEEP;// exit sleep mode
		CAN2->MCR &= ~CAN_MCR_SLEEP;

		CAN1->BTR = CAN2->BTR = 0x001C0005;// mode, speed

		CAN1->MCR &= ~(uint32)CAN_MCR_INRQ;// exit init mode
		CAN2->MCR &= ~(uint32)CAN_MCR_INRQ;
		
		//while(CAN1->MSR & CAN_MSR_INAK);// wait exiting init mode
		while(CAN1->MSR & CAN_MSR_INAK && CAN2->MSR & CAN_MSR_INAK);// wait exiting init mode

		//CAN1->FMR &= ~CAN_FMR_CAN2SB;// CAN2 filters starts with 0
		CAN1->FMR |= 1 << 8;// 0 - CAN1 filter, other 1-27 to CAN2

		CAN1->FMR |= CAN_FMR_FINIT;// filter mode on

		CAN1->sFilterRegister[0].FR1 = (uint)RGsystem::CANdeviceID::Mask << (16 + 5) | (uint)RGsystem::CANdeviceID::Mask << 5;// Mask and ID pool
		//CAN1->sFilterRegister[0].FR2=0xFFFFFFFF;// Mask in 2 register

		CAN1->FA1R = CAN_FA1R_FACT0;// active filter 0
		CAN1->FMR &= ~CAN_FMR_FINIT;// exit filter mode

		CAN1->IER |= CAN_IER_FMPIE0;// interrupt on receive pack to fifo0
		CAN2->IER |= CAN_IER_FMPIE0;

		NVIC_EnableIRQ(CAN1_RX0_IRQn);
		NVIC_EnableIRQ(CAN1_TX_IRQn);
		NVIC_EnableIRQ(CAN2_RX0_IRQn);
		NVIC_EnableIRQ(CAN2_TX_IRQn);
	}
}
