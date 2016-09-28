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

typedef enum {UINT8_T = 0,  INT8_T = 1, 
              UINT16_T = 2, INT16_T = 3,   
              UINT32_T = 4, INT32_T = 5,
              UINT64_T = 6, INT64_T = 7, 
              FLOAT = 8,    DOUBLE = 9,
              }PointerType;

#define SIZE_CAN_DATA  100
typedef struct {
  PointerType           dType[SIZE_CAN_DATA];                   // type of pointer to the data 
  uint32_t              dData[SIZE_CAN_DATA];                   // data 
  uint16_t              dID[SIZE_CAN_DATA];                     // indicates received channel and Type
  uint16_t              dLength;                                // how much values of data we have?
}CAN_Data_struct;



extern CAN_Data_struct CAN_data;
#endif