#ifndef CAN_EXCHANGE_H
#define CAN_EXCHANGE_H

#include "variables.h"
#define CAN1_ID_EXT (CAN1->sFIFOMailBox[0].RIR>>3)
#define CAN1_ID_STD (CAN1->sFIFOMailBox[0].RIR>>21)
#define CAN1_RTR ((CAN1->sFIFOMailBox[0].RIR>>1)&1)
#define CAN1_DLC (CAN1->sFIFOMailBox[0].RDTR&0x0F)
#define CAN1_Data ((byte*)0x400065B8)
#define CAN1_Data_L (CAN1->sFIFOMailBox[0].RDLR)
#define CAN1_Data_H (CAN1->sFIFOMailBox[0].RDHR)
#define CAN1_MASK 0x7F0
#define UC_ID 0x7F2
#define BC_ID 0x7F1

extern uint8_t CAN1_MSG_Received;
extern uint8_t CAN2_MSG_Received;
void pasreCAN(void);

#endif