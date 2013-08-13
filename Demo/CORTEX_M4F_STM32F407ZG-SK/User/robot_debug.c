/*****************************************************************************/
/**
* @file robot_debug.c
*
*
* This file contains a driver for DebugPrintf function in FreeRTOS.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "robot_debug.h"

/************************** Constant Definitions *****************************/
#define DEBUG_QUEUE_SIZE			128
#define DEBUG_TASK_PRIORITY			( tskIDLE_PRIORITY + 4UL )

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
static xQueueHandle xDebugQueue;

/*****************************************************************************/
/**
*
* This is a FreeRTOS task.
* This task has very low priority which continues scaning the usart port and
* send data from queue, if any, to usart.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void UsartTask( void *pvParameters ) 
{
	char ch;
	portBASE_TYPE xStatus;
	//uint16_t u16StackSize;

	/* The parameters are not used. */
	( void ) pvParameters;

	for(;;) {
		// As long as there are characters in the queue fifo this code should
		// pop them out and send them as quick as possible out the UART.
		if( USART_GetFlagStatus( USARTx, USART_FLAG_TXE ) ) {
			// We don't want to block forever - need to check on Rx too.
			xStatus = xQueueReceive( xDebugQueue, &ch, portMAX_DELAY );
			if( xStatus == pdPASS ) USART_SendData( USARTx, ch );
		}
//		if ( USART_GetFlagStatus( USARTx, USART_FLAG_RXNE ) ) {
//			ch = USART_ReceiveData( USARTx );
//			// Handle Debug Console Commands Here.
//			switch ( ch ) {
//
//			// Alphabetical list of commands the console debugger responds to.
//
//			case '0':
//				break;
//			default:
//				break;
//			}
//		}
//
//		taskYIELD();
	}
}

/*****************************************************************************/
/**
*
* Configures the USART Peripheral as Debug Output.
* Creat Debug Output Queue.
* Creat Debug Output Task.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void DebugInitialise(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  /* USART GPIOs configuration **************************************************/
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(USART_GPIO_CLK, ENABLE);

  /* Connect UART pins to AF */
  GPIO_PinAFConfig(USART_GPIO_PORT, USART_TX_SOURCE, USART_AF_PORT);
  GPIO_PinAFConfig(USART_GPIO_PORT, USART_RX_SOURCE, USART_AF_PORT);
  
  /* Configure USART RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = USART_TX_PIN | USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);

  /* USART configuration ********************************************************/
  /* Enable UART clock */
  RCC_APB1PeriphClockCmd(USART_CLK, ENABLE);
  
  /* USART register init */
  USART_DeInit(USARTx);
  USART_StructInit(&USART_InitStructure);
  
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* USART configuration init*/
  USART_Init(USARTx, &USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(USARTx, ENABLE);
  
  /* Creat Debug Output Queue. */
  xDebugQueue = xQueueCreate( DEBUG_QUEUE_SIZE, sizeof( char ) );
  
  /* Creat Debug Output Task. */
  xTaskCreate( 	UsartTask, 
				( signed char * ) "Debug", 
				configMINIMAL_STACK_SIZE, 
				NULL, 
				DEBUG_TASK_PRIORITY, 
				NULL ); 
}

/*****************************************************************************/
/**
*
* This function copies the the given string into the OS queue.  If the queue
* is full then the rest of the string is ignored.
* ToDo: Ignoring a full queue is not good.
*
* @param	s
*
* @return	None
*
* @note		None
*
******************************************************************************/
void DebugString( char *s ) 
{
	portBASE_TYPE xStatus;

	// Once we start coping a string into the queue we don't want to get
	// interrupted.  The copy must be done quickly since interrupts are off!
	taskENTER_CRITICAL();
	while ( *s ) {
		xStatus = xQueueSendToBack( xDebugQueue, s++, 0 );
		if ( xStatus == errQUEUE_FULL ) break;
	}
	taskEXIT_CRITICAL();
}

/*****************************************************************************/
/**
*
* Convert a number to a string - used in DebugPrintf.
*
* @param	s
* @param	pPos
* @param	u32Number
* @param	u8Base
*
* @return	None
*
* @note		None
*
******************************************************************************/
void Num2String( char *s, uint8_t *pPos, uint32_t u32Number, uint8_t u8Base) 
{

    char buf[33];
    char *p = buf + 33;
    uint32_t c, n;

    *--p = '\0';
    do {
        n = u32Number / u8Base;
        c = u32Number - (n * u8Base);
        if (c < 10) {
            *--p = '0' + c;
        } else {
            *--p = 'a' + (c - 10);
        }
        u32Number /= u8Base;
    } while (u32Number != 0);

    while (*p){
    	s[ *pPos ] = *p;
    	*pPos += 1;
        p++;
    }
    return;
}

/*****************************************************************************/
/**
*
* DebugPrintf - really trivial implementation, however, it's reentrant!
* ToDo - This needs a rewrite! Add code to check we're not overflowing.
*
* @param	*fmt
*
* @return	None
*
* @note		None
*
******************************************************************************/
void DebugPrintf(const char *fmt, ...)
 {
	char sTmp[80];	// String build area.  String lives on the stack!
	uint8_t pos=0;
	char *bp = (char *)fmt;
    va_list ap;
    char c;
    char *p;
    int i;

    va_start(ap, fmt);

    while ((c = *bp++)) {
        if (c != '%') {
            sTmp[pos++] = c;
            continue;
        }

        switch ((c = *bp++)) {
			// d - decimal value
			case 'd':
				Num2String( sTmp, &pos, va_arg(ap, uint32_t), 10);
				break;

			// %x - value in hex
			case 'x':
				sTmp[pos++] = '0';
				sTmp[pos++] = 'x';
				Num2String( sTmp, &pos, va_arg(ap, uint32_t), 16);
				break;

			// %b - binary
			case 'b':
				sTmp[pos++] = '0';
				sTmp[pos++] = 'b';
				Num2String( sTmp, &pos, va_arg(ap, uint32_t), 2);
				break;

			// %c - character
			case 'c':
				sTmp[pos++] = va_arg(ap, int);
				break;

			// %i - integer
			case 'i':
				i = va_arg(ap, int32_t);
				if(i < 0){
					sTmp[pos++] = '-';
					Num2String( sTmp, &pos, (~i)+1, 10);
				} else {
					Num2String( sTmp, &pos, i, 10);
				}
				break;

			// %s - string
			case 's':
				p = va_arg(ap, char *);
				do {
					sTmp[pos++] = *p++;
				} while (*p);
				break;

			// %% - output % character
			case '%':
				sTmp[pos++] = '%';
				break;

			// Else, must be something else not handled.
			default:
				sTmp[pos++] = '?';
				break;
        }
    }
    sTmp[pos++] = 0;		// Mark the end of the string.
    DebugString( sTmp );	// Copy the string into the OS queue.
    return;
}



