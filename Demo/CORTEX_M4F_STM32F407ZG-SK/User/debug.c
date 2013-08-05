/*
 * debug.c
 *
 *  Created on: Jan. 5, 2012
 *      Author: James Kemp
 */
#include <stdarg.h>
#include <ctype.h>

#include "debug.h"

// Private functions.
void vNum2String( char *s, uint8_t *pPos, uint32_t u32Number, uint8_t u8Base);

// Total buffer size for all debug messages.
#define DEBUG_QUEUE_SIZE	128
xQueueHandle xDebugQueue;

extern xTaskHandle hDebugTask;


/**
 * @brief Definition for Debug port, connected to USART3
 */ 
USART_TypeDef* COM_USART = USART3; 
USART_InitTypeDef USART_InitStructure;

/**
  * @brief  Configures the USART Peripheral as Debug Output.
  * @param  None
  * @retval None
  */
void vDebugInitialise(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
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
}

// ============================================================================
void vDebugInitQueue( void ) 
{
	xDebugQueue = xQueueCreate( DEBUG_QUEUE_SIZE, sizeof( char ) );
}

// ============================================================================
portTASK_FUNCTION( vDebugTask, pvParameters ) 
{
	char ch;
	portBASE_TYPE xStatus;
	//uint16_t u16StackSize;

	/* The parameters are not used. */
	( void ) pvParameters;

	vDebugString( "Debug task started.\r\n");

	for(;;) {
		// As long as there are characters in the queue fifo this code should
		// pop them out and send them as quick as possible out the UART.
		if( USART_GetFlagStatus( COM_USART, USART_FLAG_TXE ) ) {
			// We don't want to block forever - need to check on Rx too.
			xStatus = xQueueReceive( xDebugQueue, &ch, 10 / portTICK_RATE_MS );
			if( xStatus == pdPASS ) USART_SendData( COM_USART, ch );
		}
		if ( USART_GetFlagStatus( COM_USART, USART_FLAG_RXNE ) ) {
			ch = USART_ReceiveData( COM_USART );
			// Handle Debug Console Commands Here.
			switch ( ch ) {

			// Alphabetical list of commands the console debugger responds to.

			case 'm':
				vDebugPrintf( "Mems dump Stopped.\r\n");
				//vSetMemsDump( false );
				break;
			case 'M':
				vDebugPrintf( "Mems dump Started.\r\n");
				//vSetMemsDump( true );
				break;

			case 'a':
				vDebugPrintf( "AtoD dump Stopped.\r\n");
				//vSetAtoDDump( FALSE );
				break;
			case 'A':
				vDebugPrintf( "AtoD dump Started.\r\n");
				//vSetAtoDDump( TRUE );
				break;

			case 'l':
				vDebugPrintf( "Loop Count Stopped.\r\n");
				//vSetCntLoops( FALSE );
				break;
			case 'L':
				vDebugPrintf( "Loop Count Started.\r\n");
				//vSetCntLoops( TRUE );
				break;

			// Print out how much stack space remains on each task stack.
			case 's':
				vDebugPrintf( "Remaining space on Task Stack:\r\n" );
				//u16StackSize = uxTaskGetStackHighWaterMark( hDebugTask );
				//vDebugPrintf( "Debug\t%d\r\n", u16StackSize);
				//u16StackSize = uxTaskGetStackHighWaterMark( hTimeTask );
				//vDebugPrintf( "Time\t%d\r\n", u16StackSize);
				//u16StackSize = uxTaskGetStackHighWaterMark( hLCDTask );
				//vDebugPrintf( "LCD\t%d\r\n", u16StackSize);
				break;

			// Add general test code here...
			case 't':
				break;

			default:
				break;
			}
		}

		taskYIELD();
	}
}



// This function copies the the given string into the OS queue.  If the queue
// is full then the rest of the string is ignored.
// ToDo: Ignoring a full queue is not good.
// ============================================================================
void vDebugString( char *s ) 
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

// Simply print to the debug console a string based on the type of reset.
// ============================================================================
void vDebugPrintResetType( void ) 
{

	if ( PWR_GetFlagStatus( PWR_FLAG_WU ) )
		vDebugPrintf( "PWR: Wake Up flag\r\n" );
	if ( PWR_GetFlagStatus( PWR_FLAG_SB ) )
		vDebugPrintf( "PWR: StandBy flag.\r\n" );
	if ( PWR_GetFlagStatus( PWR_FLAG_PVDO ) )
		vDebugPrintf( "PWR: PVD Output.\r\n" );
	if ( PWR_GetFlagStatus( PWR_FLAG_BRR ) )
		vDebugPrintf( "PWR: Backup regulator ready flag.\r\n" );
	if ( PWR_GetFlagStatus( PWR_FLAG_REGRDY ) )
		vDebugPrintf( "PWR: Main regulator ready flag.\r\n" );

	if ( RCC_GetFlagStatus( RCC_FLAG_BORRST ) )
		vDebugPrintf( "RCC: POR/PDR or BOR reset\r\n" );
	if ( RCC_GetFlagStatus( RCC_FLAG_PINRST ) )
		vDebugPrintf( "RCC: Pin reset.\r\n" );
	if ( RCC_GetFlagStatus( RCC_FLAG_PORRST ) )
		vDebugPrintf( "RCC: POR/PDR reset.\r\n" );
	if ( RCC_GetFlagStatus( RCC_FLAG_SFTRST ) )
		vDebugPrintf( "RCC: Software reset.\r\n" );
	if ( RCC_GetFlagStatus( RCC_FLAG_IWDGRST ) )
		vDebugPrintf( "RCC: Independent Watchdog reset.\r\n" );
	if ( RCC_GetFlagStatus( RCC_FLAG_WWDGRST ) )
		vDebugPrintf( "RCC: Window Watchdog reset.\r\n" );
	if ( RCC_GetFlagStatus( RCC_FLAG_LPWRRST ) )
		vDebugPrintf( "RCC: Low Power reset.\r\n" );
}



// DebugPrintf - really trivial implementation, however, it's reentrant!
// ToDo - This needs a rewrite! Add code to check we're not overflowing.
// ============================================================================
void vDebugPrintf(const char *fmt, ...)
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
				vNum2String( sTmp, &pos, va_arg(ap, uint32_t), 10);
				break;

			// %x - value in hex
			case 'x':
				sTmp[pos++] = '0';
				sTmp[pos++] = 'x';
				vNum2String( sTmp, &pos, va_arg(ap, uint32_t), 16);
				break;

			// %b - binary
			case 'b':
				sTmp[pos++] = '0';
				sTmp[pos++] = 'b';
				vNum2String( sTmp, &pos, va_arg(ap, uint32_t), 2);
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
					vNum2String( sTmp, &pos, (~i)+1, 10);
				} else {
					vNum2String( sTmp, &pos, i, 10);
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
    vDebugString( sTmp );	// Copy the string into the OS queue.
    return;
}


// Convert a number to a string - used in vDebugPrintf.
// ============================================================================
void vNum2String( char *s, uint8_t *pPos, uint32_t u32Number, uint8_t u8Base) 
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



