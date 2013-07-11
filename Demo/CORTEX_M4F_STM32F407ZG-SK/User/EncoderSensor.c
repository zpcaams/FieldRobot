/**
  ******************************************************************************
  * @file    EncoderSensor.c
  * @author  ZhouPeng
  * @version V0.1
  * @date    2013-3-22
  * @brief   This file provides a set of functions needed to manage the EncoderSensor
  *          board via SPI interface.
  */

/* Includes ------------------------------------------------------------------*/
#include "EncoderSensor.h"

/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Private_Defines
  * @{
  */
__IO uint32_t  EncoderSensorTimeout = EncoderSensor_FLAG_TIMEOUT;   

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Private_Macros
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Private_Variables
  * @{
  */ 
uint8_t EncoderSensorBuffer[16*2];

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Private_FunctionPrototypes
  * @{
  */
static uint8_t EncoderSensor_SendByte(uint8_t byte);
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Private_Functions
  * @{
  */


/**
  * @brief  Set EncoderSensor Initialization.
  * @param  None
  * @retval None
  */
void vEncoderSensorInitialise(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(EncoderSensor_SPI_CLK, ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHB1PeriphClockCmd(EncoderSensor_SPI_SCK_GPIO_CLK | EncoderSensor_SPI_MISO_GPIO_CLK | EncoderSensor_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable CS  GPIO clock */
  RCC_AHB1PeriphClockCmd(EncoderSensor_SPI_CS_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(EncoderSensor_SPI_SCK_GPIO_PORT, EncoderSensor_SPI_SCK_SOURCE, EncoderSensor_SPI_SCK_AF);
  GPIO_PinAFConfig(EncoderSensor_SPI_MISO_GPIO_PORT, EncoderSensor_SPI_MISO_SOURCE, EncoderSensor_SPI_MISO_AF);
  GPIO_PinAFConfig(EncoderSensor_SPI_MOSI_GPIO_PORT, EncoderSensor_SPI_MOSI_SOURCE, EncoderSensor_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = EncoderSensor_SPI_SCK_PIN;
  GPIO_Init(EncoderSensor_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  EncoderSensor_SPI_MOSI_PIN;
  GPIO_Init(EncoderSensor_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = EncoderSensor_SPI_MISO_PIN;
  GPIO_Init(EncoderSensor_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(EncoderSensor_SPI);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(EncoderSensor_SPI, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(EncoderSensor_SPI, ENABLE);

  /* Configure GPIO PIN for Lis Chip select */
  GPIO_InitStructure.GPIO_Pin = EncoderSensor_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(EncoderSensor_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(EncoderSensor_SPI_CS_GPIO_PORT, EncoderSensor_SPI_CS_PIN);
}

/**
  * @brief Read the abs encoder information and 2.4G channel signal via SPI interface.
  *        This is a freertos task.
  * @param  vEncoderSensorRefersh: task name. 
  *         pvParameters: task parameters.
  * @retval None
  */
void vEncoderSensorRefershTask( void *pvParameters )
{
        portTickType xLastWakeTime;
	uint8_t i = 0;

	/* The parameters are not used. */
	( void ) pvParameters;

	/* We need to initialise xLastWakeTime prior to the first call to 
	vTaskDelayUntil(). */
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{       
            /* Read data from spi interface */
            for(i=0;i<(15+1);i++)
            {
                EncoderSensor_Read(&EncoderSensorBuffer[2*i], i, 2);
            }

            /* Run this task every 50 ms */
            vTaskDelayUntil( &xLastWakeTime, 50 / portTICK_RATE_MS );
	}
} 

/**
  * @brief  Reads a block of data from the EncoderSensor.
  * @param  pBuffer : pointer to the buffer that receives the data read from the EncoderSensor.
  * @param  ReadAddr : EncoderSensor's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EncoderSensor.
  * @retval None
  */
void EncoderSensor_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  /* Set chip select Low at the start of the transmission */
  EncoderSensor_CS_LOW();
  
  /* Send the Address of the indexed register */
  EncoderSensor_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to EncoderSensor (Slave device) */
    *pBuffer = EncoderSensor_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  EncoderSensor_CS_HIGH();
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t EncoderSensor_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  EncoderSensorTimeout = EncoderSensor_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(EncoderSensor_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((EncoderSensorTimeout--) == 0) return EncoderSensor_TIMEOUT_UserCallback();
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(EncoderSensor_SPI, byte);
  
  /* Wait to receive a Byte */
  EncoderSensorTimeout = EncoderSensor_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(EncoderSensor_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((EncoderSensorTimeout--) == 0) return EncoderSensor_TIMEOUT_UserCallback();
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(EncoderSensor_SPI);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t EncoderSensor_TIMEOUT_UserCallback(void)
{
	//printf("SPI interface ERROR!");
  /* Block communication and all processes */
  while (1)
  {   
  }
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */
