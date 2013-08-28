/*****************************************************************************/
/**
* @file robot_spi.c
*
*
* This file contains a driver which get register value from fpga
* via spi interface using DMA.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_spi.h"

/************************** Constant Definitions *****************************/

#define SPIx                       SPI2
#define SPIx_CLK                   RCC_APB1Periph_SPI2

#define SPIx_SCK_PIN               GPIO_Pin_13                  /* PB.13 */
#define SPIx_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define SPIx_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE            GPIO_PinSource13
#define SPIx_SCK_AF                GPIO_AF_SPI2

#define SPIx_MISO_PIN              GPIO_Pin_14                  /* PB.14 */
#define SPIx_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define SPIx_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define SPIx_MISO_SOURCE           GPIO_PinSource14
#define SPIx_MISO_AF               GPIO_AF_SPI1

#define SPIx_MOSI_PIN              GPIO_Pin_15                  /* PB.15 */
#define SPIx_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define SPIx_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define SPIx_MOSI_SOURCE           GPIO_PinSource15
#define SPIx_MOSI_AF               GPIO_AF_SPI1

#define SPIx_CS_PIN                GPIO_Pin_12                  /* PB.12 */
#define SPIx_CS_GPIO_PORT          GPIOB                       /* GPIOB */
#define SPIx_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

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
* Get Spi Buffer Address
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void GetSpiBuffer(u8 *pBuffer)
{
	pBuffer = Spi_RxBuffer;
}

/*****************************************************************************/
/**
*
* Refersh RAM value via spi from fpga.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SpiRefershTask( void *pvParameters )
{
	portTickType xLastWakeTime;
    portBASE_TYPE xStatus;
    
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		SPIx_CS_LOW();

		/* Enable DMA SPI RX Stream */
		DMA_Cmd(SPIx_RX_DMA_STREAM, ENABLE);
		/* Enable DMA SPI TX Stream */
		DMA_Cmd(SPIx_TX_DMA_STREAM, ENABLE);

    	do{
    		xStatus = xSemaphoreTake(xSPIDMASemaphore, 10/portTICK_RATE_MS);
    	}while(xStatus==pdTRUE);
			
		SPIx_CS_HIGH();

		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
	}
}

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
    portBASE_TYPE xStatus;
	
	SPIx_CS_LOW();
  
	/* Enable DMA SPI RX Stream */
	DMA_Cmd(SPIx_RX_DMA_STREAM, ENABLE);
	/* Enable DMA SPI TX Stream */
	DMA_Cmd(SPIx_TX_DMA_STREAM, ENABLE);
  
	do{
		xStatus = xSemaphoreTake(xSPIDMASemaphore, 10/portTICK_RATE_MS);
	}while(xStatus==pdTRUE);
	
	SPIx_CS_HIGH();

	i=0x14;
	if ((Spi_RxBuffer[(i*3+1)]!=0x5A)&&(Spi_RxBuffer[(i*3+2)]!=0xA5)){
		DebugPrintf("SPI test failed, FPGA board may have some error!\n");
		while(1);
	}
	DebugPrintf("SPI test pass!\n");
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
void SpiInitialise(void)
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

	/* Enable SPI DMA TX Requsts */
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	/* Enable SPI DMA RX Requsts */
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
	
	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(SPIx_RX_DMA_STREAM, DMA_IT_TC, ENABLE);
	
	/* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA_STREAM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	vSemaphoreCreateBinary(xSPIDMASemaphore);
}