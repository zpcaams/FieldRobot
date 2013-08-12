/*****************************************************************************/
/**
* @file robot_encoder.c
*
*
* This file contains a driver which get absolute encoder value and 
* 2.4G wireless remote control information via spi interface.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_encoder.h"

/************************** Constant Definitions *****************************/

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

__IO uint32_t  SPIxTimeout = SPIx_FLAG_TIMEOUT;   

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/*****************************************************************************/
/**
*
* SPI interface TIMEOUT UserCallback.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
uint32_t SPIx_TIMEOUT_UserCallback(void)
{
	DebugPrintf("SPI interface TIMEOUT ERROR!");
	/* Block communication and all processes */
	while (1)
	{   
	}
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/*****************************************************************************/
/**
*
* Sends a Byte through the SPI interface and return the Byte received 
*         	from the SPI bus.
*
* @param	Byte : Byte send.
*
* @return	The received byte value
*
* @note		None
*
******************************************************************************/
static uint8_t SPIx_SendByte(uint8_t byte)
{
	/* Loop while DR register in not emplty */
	SPIxTimeout = SPIx_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPIxTimeout--) == 0) return SPIx_TIMEOUT_UserCallback();
	}
	
	/* Send a Byte through the SPI peripheral */
	SPI_I2S_SendData(SPIx, byte);
	
	/* Wait to receive a Byte */
	SPIxTimeout = SPIx_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPIxTimeout--) == 0) return SPIx_TIMEOUT_UserCallback();
	}
	
	/* Return the Byte read from the SPI bus */
	return (uint8_t)SPI_I2S_ReceiveData(SPIx);
}

/*****************************************************************************/
/**
*
* Reads a block of data from the SPI interface.
*
* @param	pBuffer : pointer to the buffer that receives the data 
* 						read from the SPI interface.
* @param	ReadAddr : Encoder's internal address to read from.
* @param	NumByteToRead : number of bytes to read from the SPI interface.
*
* @return	None
*
* @note		None
*
******************************************************************************/
static void SPIx_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
	/* Set chip select Low at the start of the transmission */
	SPIx_CS_LOW();
  
	/* Send the Address of the indexed register */
	SPIx_SendByte(ReadAddr);
	
	/* Receive the data that will be read from the device (MSB First) */
	while(NumByteToRead > 0x00)
	{
	/* Send dummy byte (0x00) to generate the SPI clock to Encoder (Slave device) */
		*pBuffer = SPIx_SendByte(DUMMY_BYTE);
		NumByteToRead--;
		pBuffer++;
	}
	
	/* Set chip select High at the end of the transmission */ 
	SPIx_CS_HIGH();
}

/*****************************************************************************/
/**
*
* Read the absolute encoder value and 2.4G remote control via SPI interface.
* Each time only one channel data is read.
* For total 16 data takes 160ms to refersh them all.
* 	Channel 0~3 Steering Motor Position Encoder
* 	Channel 4~7 Couplings Position Encoder
* 	Channel 8~15 2.4G remote control
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void EncoderRefershTask( void *pvParameters )
{
	portTickType xLastWakeTime;
	uint8_t i = 0;
	uint16_t SpiBuffer16;
	uint8_t SpiBuffer[2];
	
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{       
	  
		if (i<15) {i++;} 
		else {i = 0;}
		
		SPIx_Read(SpiBuffer, i, 2);
    SpiBuffer16 = (SpiBuffer[0]<<8)+SpiBuffer[1];
		
		if (i < 5){
			SetSteeringMotorPosition((i), SpiBuffer16);
		} else if (i < 8){
			SetCouplingsPosition((i-4), SpiBuffer16);
		} else {
			SetRemoteControl((i-8), SpiBuffer16);
		}
	  
//		vDebugPrintf("i:%i, d:%i\n", i, SpiBuffer16);
		vTaskDelayUntil( &xLastWakeTime, 10 / portTICK_RATE_MS );
	}
} 

/*****************************************************************************/
/**
*
* Initialization the SPI Peripheral.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void EncoderInitialise(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	/* Enable the SPI periph */
	RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);
	
	/* Enable SCK, MOSI and MISO GPIO clocks */
	RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
	
	/* Enable CS  GPIO clock */
	RCC_AHB1PeriphClockCmd(SPIx_CS_GPIO_CLK, ENABLE);
	
	GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
	GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
	GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
	GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
	GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
	
	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
	GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPIx);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPIx, &SPI_InitStructure);
	
	/* Enable SPIx  */
	SPI_Cmd(SPIx, ENABLE);
	
	/* Configure GPIO PIN for Lis Chip select */
	GPIO_InitStructure.GPIO_Pin = SPIx_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);
	
	/* Deselect : Chip Select high */
	GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN);
	
	xTaskCreate( EncoderRefershTask, ( signed char * ) "Encoder", configMINIMAL_STACK_SIZE, NULL, Encoder_TASK_PRIORITY, NULL );

}