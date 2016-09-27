#include "can_exchange.h"
#include "can.h"
#include "CRC.h"

uint8_t CAN1_MSG_Received;
uint8_t CAN2_MSG_Received;



typedef enum  {
		CoreID,
		InputChannel_0=0x30,
		InputChannel_1,
		InputChannel_2,
		InputChannel_3,
		InputChannel_4,
		InputChannel_5,
		InputChannel_6,
		InputChannel_7,
		InputChannel_8,
		InputChannel_9,
		InputChannel_10,
		InputChannel_11,
		InputChannel_12,
		InputChannel_13,
		InputChannel_14,
		InputChannel_15,
		InputChannel_16,
		InputChannel_17,
		InputChannel_18,
		InputChannel_19,
		InputChannel_20,
		InputChannel_21,
		InputChannel_22,
		InputChannel_23,
		InputChannel_24,
		InputChannel_25,
		InputChannel_26,
		InputChannel_27,
		InputChannel_28,
		InputChannel_29,
		InputChannel_30,
		InputChannel_31,
		InputChannel_32,
		InputChannel_33,
		InputChannel_34,
		InputChannel_35,
		InputChannel_36,
		InputChannel_37,
		InputChannel_38,
		InputChannel_39,
		InputChannel_40,
		InputChannel_41,
		InputChannel_42,
		InputChannel_43,
		InputChannel_44,
		InputChannel_45,
		InputChannel_46,
		InputChannel_47,
                SpecVAL = 0x1000  // just HACK we need to get at least 13 bits 
	}ParameterID;

typedef enum {Get = 0,Set,Single,Double,Quad,Flow,reserved,Error} ParameterType;

typedef struct 
	{
		ParameterType	        type	:       3;
		ParameterID		ID	:       13;
	}ParameterHeader;
 
struct Parameter8
	{
		ParameterType	        type	:      3;
		ParameterID		ID	:     13;
		uint8_t			data;
	};
	
struct Parameter16
	{
		ParameterType	        type	        : 3;
		ParameterID		ID		: 13;
		uint16_t		data;
	};
	
typedef struct 
	{
		uint16_t	counter	: 11;
		uint8_t	        group	: 5;
		uint16_t	messageLen;
		uint32_t	messageCRC;
//		bool	isStart(){return counter == 0;};
	}MessageHeader;

typedef enum 
	{
		BroadCast=0x7F0,
		OnBoardComputer,
		UniversalController,
		AngleSensor=0x7F5,
		Debugger=0x7FF,
		Mask=0x7F0
	}CANdeviceID;
        
typedef struct  
	{
		uint16_t	counter	: 11;
		uint8_t 	group	: 5;
		uint8_t 	data[6];
	}CANpackBody; 



#define SIZE_CAN_BUFFER         200
typedef struct 
	{
		CANdeviceID	CANID;
		uint8_t		group;
		uint32_t	messageCRC;
		uint16_t	messageLength;
   volatile     uint8_t		messageNew;
		uint8_t		overflow;
		uint8_t		buf[SIZE_CAN_BUFFER];
		uint16_t	bufCounter;
	}CANbufT;



struct{
  CANbufT receiverBuf0;  // double bufferization of CAN data receiving
  CANbufT receiverBuf1;
  uint8_t CurrentBuffer;
}
CAN_ReceivedData;

typedef enum {UINT8_T = 0,  INT8_T = 1, 
              UINT16_T = 2, INT16_T = 3,   
              UINT32_T = 4, INT32_T = 5,
              UINT64_T = 6, INT64_T = 7, 
              FLOAT = 8,    DOUBLE = 9,
              }PointerType;
///------------------------------------------------------------------------
//INTRODUCE----------------------------------------------------------------
#define SIZE_CAN_DATA  100
/////////
struct {
  PointerType           dType[SIZE_CAN_DATA];                   // type of pointer to the data 
  uint32_t              dData[SIZE_CAN_DATA];                   // pointers to the data 
  uint16_t              dID[SIZE_CAN_DATA];                     // indicates received channel and Type
 // uint8_t               dDATA[SIZE_CAN_DATA];                   // that data (pure)
}CAN_data;

void pasreCAN(void){
uint16_t i; 
uint16_t Index = 0;

CANbufT * parsingBuf;
ParameterHeader * paramHead; // head of params
uint8_t step = 3;            // step that use for going inside of the buffer (default)

//if (!CAN_ReceivedData.CurrentBuffer) parsingBuf = &(CAN_ReceivedData.receiverBuf1);  // we need to Parse Another BUffer instead of Current
//else parsingBuf = &(CAN_ReceivedData.receiverBuf0);
if(CAN_ReceivedData.receiverBuf0.messageNew){ 
      parsingBuf = &(CAN_ReceivedData.receiverBuf0);
      CAN_ReceivedData.receiverBuf0.messageNew = 0;
}
else{
  if(CAN_ReceivedData.receiverBuf1.messageNew){
      parsingBuf = &(CAN_ReceivedData.receiverBuf1);
      CAN_ReceivedData.receiverBuf1.messageNew = 0;
  }
  else 
    return;
 }

for(i = 0; i < parsingBuf->messageLength; i += step ){
 paramHead = (ParameterHeader *) &(parsingBuf->buf[i]);
 CAN_data.dID[Index] = paramHead->ID;
 
 switch(paramHead->type){
 case (Single):
   CAN_data.dData[Index] = ((struct Parameter8 *)( & (parsingBuf->buf[i])))->data;
   CAN_data.dType[Index] = UINT8_T;
   step = 3;
    break;
 case (Double):
   CAN_data.dData[Index] = ((struct Parameter16 *)( & (parsingBuf->buf[i])))->data;
   CAN_data.dType[Index] = UINT16_T;
   step = 4;
    break;   
  }
 Index++;
 }
return;
}

void CheckMessage(void){
//  receiver.messageCRC = newMessage->messageCRC;
//  if(CRCcalculateBuf(receiver.buf, receiver.messageLength) == receiver.messageCRC)messageReceived();
}

void CAN1TransmitByte(uint16_t _ID, uint8_t data)
 {
   if ((CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0)
    {
	uint8_t index = CAN1->TSR = (CAN1->TSR & CAN_TSR_CODE) >> 24;
			CAN1->sTxMailBox[index].TDTR = 1;// data length
			CAN1->sTxMailBox[index].TDLR = data;// data fill
			CAN1->sTxMailBox[index].TIR = (uint32_t)(_ID << 21 | CAN_TI0R_TXRQ);// Id field (0x7F2) and request
		}
 }

void CAN1TransmitPack(uint16_t _ID, uint8_t * data, uint8_t dataLen)
	{
		if (dataLen > 8)dataLen = 8;
		if ((CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0)
		{
			uint8_t index = CAN1->TSR = (CAN1->TSR & CAN_TSR_CODE) >> 24;
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

uint8_t CAN1ready()
	{
		return (CAN1->TSR & (CAN_TSR_TME0 | CAN_TSR_TME1 | CAN_TSR_TME2)) > 0;
	}


void CAN_PassData(uint8_t Param, uint8_t * pData){  // get data to a buffer //Param: zero - it is a head//pData - is pointer to array with a data
 static  uint16_t countCheck = 1; //(arrived counter)
 uint8_t i;
   
static CANbufT * passingBuf = &(CAN_ReceivedData.receiverBuf0);
 CANpackBody * MsgData;
 MessageHeader * MsgHeader;
 
  switch(Param){
    case 0:  // it is a head
      if(!CAN_ReceivedData.CurrentBuffer)
        passingBuf =  &(CAN_ReceivedData.receiverBuf0);
      else 
        passingBuf =  &(CAN_ReceivedData.receiverBuf1);
      
      MsgHeader = (MessageHeader *) pData;
      passingBuf->group = (MsgHeader)->group;            //take the group   
      passingBuf->messageLength = (MsgHeader)->messageLen; // take the Length
      passingBuf->messageCRC = (MsgHeader)->messageCRC;  //take the CRC
      passingBuf->bufCounter = 0;
      passingBuf->messageNew = 0;
      passingBuf->overflow = 0;
      countCheck = 1;
      break;
    case 1: //it is a data
      MsgData = (CANpackBody *)pData;
      if(MsgData->counter == countCheck++){ //we get the right counter
        for(i=0; i < 6 ; i++){
          if((passingBuf->bufCounter) < (passingBuf->messageLength)){ 
          passingBuf->buf[passingBuf->bufCounter++] = MsgData->data[i];
          if((passingBuf->bufCounter) == (passingBuf->messageLength)){
            passingBuf->messageNew = 1;
           }
          }
         else passingBuf->overflow = 1;   
        }
      }
      break;
  }
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle) {
  if(CanHandle == &hcan1){
    if ((CanHandle->pRxMsg->StdId == UC_ID) && (CanHandle->pRxMsg->IDE == CAN_ID_STD))// && (CanHandle->pRxMsg->DLC == 8))
    {
        if (CanHandle->pRxMsg->Data[0] == 0) {  //HEAD of message
         if(!CAN_ReceivedData.CurrentBuffer)
           CAN_ReceivedData.CurrentBuffer = 1;
         else 
           CAN_ReceivedData.CurrentBuffer = 0;
         
         CAN_PassData(0, &(CanHandle->pRxMsg->Data[0]));
       }
        else{
         CAN_PassData(1, &(CanHandle->pRxMsg->Data[0]));
        }
    }
   if (HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0) != HAL_OK) {
  //       /* Reception Error */
    }
  }
  else{
   if(CanHandle == &hcan2){
    if ((CanHandle->pRxMsg->StdId == UC_ID) && (CanHandle->pRxMsg->IDE == CAN_ID_STD) && (CanHandle->pRxMsg->DLC == 2)) {
    if (CanHandle->pRxMsg->Data[0]) {
              

         CAN2_MSG_Received = 1;
        }
    }
   if (HAL_CAN_Receive_IT(CanHandle, CAN_FIFO1) != HAL_OK) {
        /* Reception Error */
    }
   }
  }
}