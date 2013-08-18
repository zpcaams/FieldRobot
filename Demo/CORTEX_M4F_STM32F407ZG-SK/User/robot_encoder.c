/*****************************************************************************/
/**
* @file robot_encoder.c
*
*
* This file contains a driver which get register value of encoder and 
* 2.4G wireless remote control information via spi interface using DMA.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_encoder.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define SPIx_CS_LOW()       GPIO_ResetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define SPIx_CS_HIGH()      GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

xSemaphoreHandle xSPIDMASemaphore;
static const u8 Spi_TxBuffer[SPIx_DMA_BUFFER_SIZE] = {
		0x00, 0x00, 0x00, 
		0x01, 0x00, 0x00, 
		0x02, 0x00, 0x00, 
		0x03, 0x00, 0x00, 
		0x04, 0x00, 0x00, 
		0x05, 0x00, 0x00, 
		0x06, 0x00, 0x00, 
		0x07, 0x00, 0x00, 
		0x08, 0x00, 0x00, 
		0x09, 0x00, 0x00, 
		0x0A, 0x00, 0x00, 
		0x0B, 0x00, 0x00, 
		0x0C, 0x00, 0x00, 
		0x0D, 0x00, 0x00, 
		0x0E, 0x00, 0x00, 
		0x0F, 0x00, 0x00, 
		0x10, 0x00, 0x00, 
		0x11, 0x00, 0x00, 
		0x12, 0x00, 0x00, 
		0x13, 0x00, 0x00,
		0x14, 0x00, 0x00,
};
static u8 Spi_RxBuffer[SPIx_DMA_BUFFER_SIZE];

/*****************************************************************************/
/**
*
* Run SPI Selftest
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SPISelfTest(void)
{
	u8 i;
	u16 SpiBuffer16;
	
	SPIx_CS_LOW();

	/* Enable SPI DMA TX Requsts */
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	
	/* Enable SPI DMA RX Requsts */
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
	
	if(xSemaphoreTake(xSPIDMASemaphore, portMAX_DELAY)==pdTRUE){
		
		SPIx_CS_HIGH();
	
		i=0x14;
		SpiBuffer16 = *(u16 *)(&Spi_RxBuffer[(i*3+1)]);
		if (SpiBuffer16!=0xA55A){
			DebugPrintf("FPGA board is not power on, SPI test failed!\n");
			while(1){}
		}
	
		i=0x8;
		SpiBuffer16 = *(u16 *)(&Spi_RxBuffer[(i*3+1)]);
		if (SpiBuffer16==60000){
			DebugPrintf("Remote Controller is not power on, SPI test failed!\n");
	//    	while(1){}
		}
		
		DebugPrintf("Load Initial Encoder Sensor Value to Local RAM.\n");
		for(i=0;i<0x14;i++){
			SpiBuffer16 = *(u16 *)(&Spi_RxBuffer[(i*3+1)]);
			DebugPrintf("Channel %i: %i\n", i, SpiBuffer16);
			
			if (i < 5){
				SetSteeringMotorPosition((i), SpiBuffer16);
			} else if (i < 8){
				SetCouplingsPosition((i-4), SpiBuffer16);
			} else if (i<16){
				SetRemoteControl((i-8), SpiBuffer16);
			}else{
				SetAbsEncoderInt((i-16), SpiBuffer16);
			}
		}
	}
} 

/*****************************************************************************/
/**
*
* Read the absolute encoder value and 2.4G remote control via SPI interface.
* 	Channel 0~3 	Steering Motor Position Encoder
* 	Channel 4~7 	Couplings Position Encoder
* 	Channel 8~15 	2.4G remote control
* 	Channel 16~19 	FPGA internal Steering Motor Position Encoder
* 	Channel 20 		SelfTest
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
	u8 i;
	u16 SpiBuffer16;
  
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		SPIx_CS_LOW();

		/* Enable SPI DMA TX Requsts */
		SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
		
		/* Enable SPI DMA RX Requsts */
		SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);

		if(xSemaphoreTake(xSPIDMASemaphore, portMAX_DELAY)==pdTRUE){
			
			SPIx_CS_HIGH();
			
			for(i=0;i<0x14;i++){
				SpiBuffer16 = *(u16 *)(&Spi_RxBuffer[(i*3+1)]);
				
				if (i < 5){
					SetSteeringMotorPosition((i), SpiBuffer16);
				} else if (i < 8){
					SetCouplingsPosition((i-4), SpiBuffer16);
				} else if (i<16){
					SetRemoteControl((i-8), SpiBuffer16);
				}else{
					SetAbsEncoderInt((i-16), SpiBuffer16);
				}			
			}
		}
		
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
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* Enable the SPI periph clock */
	RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);
	
	/* Enable SCK, MOSI, MISO and CS GPIO clocks */
	RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK |
			SPIx_MOSI_GPIO_CLK | SPIx_CS_GPIO_CLK , ENABLE);

	/* Enable DMA clock */
	RCC_AHB1PeriphClockCmd(SPIx_DMA_CLK, ENABLE);

	/* Configure GPIO PIN for Chip select */
	GPIO_InitStructure.GPIO_Pin = SPIx_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);
	SPIx_CS_HIGH();
	
	/* Connect SPI pins to AF5 */
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
	
	/* DMA configuration -------------------------------------------------------*/
	/* Deinitialize DMA Streams */
	DMA_DeInit(SPIx_TX_DMA_STREAM);
	DMA_DeInit(SPIx_RX_DMA_STREAM);
	
	/* Configure DMA Initialization Structure */
	DMA_InitStructure.DMA_BufferSize = SPIx_DMA_BUFFER_SIZE ;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(SPIx->DR)) ;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Configure TX DMA */
	DMA_InitStructure.DMA_Channel = SPIx_TX_DMA_CHANNEL ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)Spi_TxBuffer ;
	DMA_Init(SPIx_TX_DMA_STREAM, &DMA_InitStructure);
	/* Configure RX DMA */
	DMA_InitStructure.DMA_Channel = SPIx_RX_DMA_CHANNEL ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)Spi_RxBuffer ; 
	DMA_Init(SPIx_RX_DMA_STREAM, &DMA_InitStructure);

	/* Enable DMA SPI TX Stream */
	DMA_Cmd(SPIx_TX_DMA_STREAM,ENABLE);
	
	/* Enable DMA SPI RX Stream */
	DMA_Cmd(SPIx_RX_DMA_STREAM,ENABLE);

	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(SPIx_RX_DMA_STREAM, SPIx_RX_DMA_FLAG_TCIF, ENABLE);
	
	/* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA_STREAM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	vSemaphoreCreateBinary(xSPIDMASemaphore);
}