
/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "can_exchange.h"
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
CanRxMsgTypeDef RxMessageCAN1;
CanRxMsgTypeDef RxMessageCAN2;
CanTxMsgTypeDef TxMessageCAN1;
CanTxMsgTypeDef TxMessageCAN2;
uint8_t FlagCAN1_Transmit;
uint8_t FlagCAN2_Transmit;
uint8_t TransmitResultMailbox = 0;

void InitCAN1Filter(CAN_HandleTypeDef* hcan);

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 27;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_1TQ;
  hcan1.Init.BS1 = CAN_BS1_3TQ;
  hcan1.Init.BS2 = CAN_BS2_4TQ;
  hcan1.Init.TTCM = DISABLE; // time trigger communication mode 
  hcan1.Init.ABOM = DISABLE; // automatically Bus-off management
  hcan1.Init.AWUM = DISABLE; // automatic wake-up mode
  hcan1.Init.NART = DISABLE; // non-automatic retransmission mode
  hcan1.Init.RFLM = DISABLE; // receive FIFO Locked mode 
  hcan1.Init.TXFP = DISABLE; // transmit FIFO priority
  hcan1.pTxMsg = &TxMessageCAN1;
  hcan1.pRxMsg = &RxMessageCAN1;
  HAL_CAN_Init(&hcan1);


}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 27;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SJW = CAN_SJW_1TQ;
  hcan2.Init.BS1 = CAN_BS1_6TQ;
  hcan2.Init.BS2 = CAN_BS2_8TQ;
  hcan2.Init.TTCM = DISABLE;  // time trigger communication mode 
  hcan2.Init.ABOM = DISABLE;  // automatically Bus-off management
  hcan2.Init.AWUM = DISABLE;  // automatic wake-up mode
  hcan2.Init.NART = DISABLE;  // non-automatic retransmission mode
  hcan2.Init.RFLM = DISABLE;  // receive FIFO Locked mode
  hcan2.Init.TXFP = DISABLE;  // transmit FIFO priority
  hcan2.pTxMsg = &TxMessageCAN2;
  hcan2.pRxMsg = &RxMessageCAN2;
  HAL_CAN_Init(&hcan2);
  InitCANFilters(&hcan1);
}

static int HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* Peripheral clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }
  
    /**CAN1 GPIO Configuration    
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(hcan->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }
  
    /**CAN2 GPIO Configuration    
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN2_MspInit 1 */
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{

  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }
  
    /**CAN1 GPIO Configuration    
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);

    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);

    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(hcan->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }
  
    /**CAN2 GPIO Configuration    
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_6);

  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
} 


/* USER CODE BEGIN 1 */
void InitCANFilters(CAN_HandleTypeDef* hcan){
  CAN_FilterConfTypeDef  sFilterConfig;
  /*##-2- Configure the CAN Filter ###########################################*/
 
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = UC_ID << 5;// ID
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0xFFE0;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.BankNumber = 0;
  
  sFilterConfig.FilterNumber = 0; // CAN1 [ 0..13]
  HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  sFilterConfig.FilterNumber = 14;  // CAN2 [14..27]
  HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);
  
  if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
 //   Error_Handler();
  }

  /*##-3- Configure Transmission process #####################################*/
  hcan1.pTxMsg->StdId = 0x321;
  hcan1.pTxMsg->ExtId = 0x01;
  hcan1.pTxMsg->RTR = CAN_RTR_DATA;
  hcan1.pTxMsg->IDE = CAN_ID_STD;
  hcan1.pTxMsg->DLC = 8;
  /////////////////////////////
  hcan2.pTxMsg->StdId = 0x321;
  hcan2.pTxMsg->ExtId = 0x01;
  hcan2.pTxMsg->RTR = CAN_RTR_DATA;
  hcan2.pTxMsg->IDE = CAN_ID_STD;
  hcan2.pTxMsg->DLC = 8;

}



void CAN1_ON_OFF(uint8_t cmd){
    if(cmd)HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
    
  return;
}

void CAN2_ON_OFF(uint8_t cmd){
    if(cmd)HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
    
  return;
}
/* USER CODE END 1 */

