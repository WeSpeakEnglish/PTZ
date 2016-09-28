#include "can_exchange.h"
#include "can.h"
#include "CRC.h"

uint8_t CAN1_MSG_Received;
uint8_t CAN2_MSG_Received;

typedef enum {Get = 0,Set,Single,Double,Quad,Flow,reserved,Error} ParameterType;

typedef struct 
	{
		ParameterType	        type	:       3;
		uint16_t		ID	:       13;
	}ParameterHeader;
 
struct Parameter8
	{
		ParameterType	        type	:      3;
		uint16_t		ID	:     13;
		uint8_t			data;
	};
	
struct Parameter16
	{
		ParameterType	        type	        : 3;
		uint16_t		ID		: 13;
		uint16_t		data;
	};
	
typedef struct 
	{
		uint16_t	counter	: 11;
		uint8_t	        group	: 5;
		uint16_t	messageLen;
		uint32_t	messageCRC;
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


///------------------------------------------------------------------------
//INTRODUCE----------------------------------------------------------------


CAN_Data_struct CAN_data;

void pasreCAN(void){
uint16_t i; 
uint16_t Index = 0;

CANbufT * parsingBuf;
ParameterHeader * paramHead; // head of params
uint8_t step = 3;            // step that use for going inside of the buffer (default)
if(CAN_ReceivedData.receiverBuf0.messageNew){ 
      parsingBuf = &(CAN_ReceivedData.receiverBuf0);
      CAN_ReceivedData.receiverBuf0.messageNew = 0;
      CAN_data.dLength = 0;
}
else{
  if(CAN_ReceivedData.receiverBuf1.messageNew){
      parsingBuf = &(CAN_ReceivedData.receiverBuf1);
      CAN_ReceivedData.receiverBuf1.messageNew = 0;
      CAN_data.dLength = 0;
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
 CAN_data.dLength++;
 }
return;
}

void CheckMessage(void){
//  receiver.messageCRC = newMessage->messageCRC;
//  if(CRCcalculateBuf(receiver.buf, receiver.messageLength) == receiver.messageCRC)messageReceived();
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