/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can_usr.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Networking
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_PRESSED     0x00
#define KEY_NOT_PRESSED 0x01

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
//global CAN transmit/receive buffer
CanTxMsg CANTxMessage[4];
CanRxMsg CANRxMessage;
xQueueHandle xCANRcvQueue, xCANTransQueue;

/* Private function prototypes -----------------------------------------------*/
static void NVIC_Config_CAN(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Interrupt service. Reveived the CAN message and sent to queue.
  * @param  FIFONumber : FIFO0 or FIFO1.
  * @retval None
  */
void CAN_Msg_Rcvr_from_IRQ (uint8_t FIFONumber)
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    CAN_Receive(CANx, FIFONumber, &CANRxMessage);
    xQueueSendToBackFromISR( xCANRcvQueue, &CANRxMessage, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

/**
  * @brief Send the CAN message if queue is not blocked.
  * @param  pvParameters: task parameters.
  * @retval None
  */
void CANMsgSendTask (void *pvParameters)
{
uint32_t Status;
portBASE_TYPE xStatus;
CanTxMsg CANTxMsgFromQueue;

    for(;;){
        xStatus = xQueueReceive(xCANTransQueue, &CANTxMsgFromQueue, portMAX_DELAY);
        if (xStatus==pdPASS){
            do{
                Status = CAN_Transmit(CANx, &CANTxMsgFromQueue);
            }while(Status == CAN_TxStatus_NoMailBox);
        }
    }
}

void SetMotoCurrent ( void ) 
{
    uint32_t i;
    uint32_t DevId=0x7C;
    uint32_t Status;
    portBASE_TYPE xStatus;
    uint8_t RcvrCounter=0;
    uint8_t TransmitStatus[4]={0, 0, 0, 0};
    uint8_t RcvrDone=0;
    /*  TODO
        calculate the needed current here
    */

    /*
        send the message to driver by queue
    */
    for (i=0;i<4;i++){
        CANTxMessage[i].StdId = i+DevId;
        CANTxMessage[i].RTR = CAN_RTR_DATA;
        CANTxMessage[i].IDE = CAN_ID_STD;
        CANTxMessage[i].DLC = 8;
        CANTxMessage[i].Data[0] = 8;       //len
        CANTxMessage[i].Data[1] = i+DevId; //id
        CANTxMessage[i].Data[2] = 0x96;    //func:设定目标电流
        CANTxMessage[i].Data[3] = 0;       //
        CANTxMessage[i].Data[4] = 0;       //data:0
        CANTxMessage[i].Data[5] = 0;       //
        CANTxMessage[i].Data[6] = 0;       //
        CANTxMessage[i].Data[7] = 0;       //
        xStatus = xQueueSendToBack(xCANTransQueue, &CANTxMessage[i], 0);
        if (xStatus!=pdPASS){
        //error, queue full
        }
    }
    
    /*
        wait for return
    */    
    do{
        xStatus = xQueueReceive( xCANRcvQueue, &CANRxMessage, 5/portTICK_RATE_MS );
        if (Status==pdTRUE){
            RcvrCounter++;
        }
        if ((CANRxMessage.StdId >= 0)&&(CANRxMessage.StdId <= 3)){
            if ((CANRxMessage.Data[0]==6)&&(CANRxMessage.Data[2]==0x96)&&(CANRxMessage.Data[3]==0x80)&&(CANRxMessage.Data[4]==0)){
                TransmitStatus[CANRxMessage.StdId] = 1;
            }else{
                //TODO return message Error
            }
                 
        }else{
            //TODO Id Error
        }
        if(RcvrCounter==4){
            if((TransmitStatus[0])&&(TransmitStatus[1])&&(TransmitStatus[2])&&(TransmitStatus[3])){
                RcvrDone=1;
            }else{
                //TODO if receive more 4 times but some driver not got return.
            }
        }
    }while(!RcvrDone);
                 
    /*
        all 4 message sent and got return
    */
}
/**
  * @brief  Main CAN Task, run every 10 ms.
            small tasks
            1. give the wheel motor current
            2. get the wheel motor speed
            3. give the Steering motor position
            4. get the Steering motor position
            5. give the Motorized Faders voltage
            6. get the Motorized Faders position

  * @param  None
  * @retval None
  */
void vCANMainTask( void *pvParameters )
{
    portTickType xLastWakeTime;
    uint8_t TaskCounter = 0;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        switch (TaskCounter){
            case 0:{
                SetMotoCurrent();
                break;
            }
            case 1: {
                break;
            }
            default:{
                break;
            }
        }
        
        if (TaskCounter == 10){
            TaskCounter = 0;
        } else {
            TaskCounter++;
        }
        
    /* Run this task every 10 ms */
    vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    
    }
}


/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void vCANConfigInitialise(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  /* CAN Baudrate = PCLK1/((1+CAN_BS1+CAN_BS2)*CAN_Prescaler) 
                  = 1MBps (CAN clocked at 40 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
  
  NVIC_Config_CAN();
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
static void NVIC_Config_CAN(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY - 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initializes the Rx Message.
  * @param  RxMessage: pointer to the message to initialize
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0x00;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
