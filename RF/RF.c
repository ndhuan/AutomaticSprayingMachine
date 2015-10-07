/*
 * RF.c

 *
 *  Created on: Sep 11, 2015
 *      Author: Huan
 */
#include "include.h"

#define TIMER_RF TIMER1_BASE
#define TIMER_RF_INTERVAL_MS 5


#define UART_TX_BUFFER_SIZE 256
#define TX_BUFFER_USED          (GetBufferCount(&g_ui32UARTTxReadIndex,  \
		&g_ui32UARTTxWriteIndex, \
		UART_TX_BUFFER_SIZE))
#define TX_BUFFER_FREE          (UART_TX_BUFFER_SIZE - TX_BUFFER_USED)
#define TX_BUFFER_EMPTY         (IsBufferEmpty(&g_ui32UARTTxReadIndex,   \
		&g_ui32UARTTxWriteIndex))
#define TX_BUFFER_FULL          (IsBufferFull(&g_ui32UARTTxReadIndex,  \
		&g_ui32UARTTxWriteIndex, \
		UART_TX_BUFFER_SIZE))
#define ADVANCE_TX_BUFFER_INDEX(Index) \
		(Index) = ((Index) + 1) % UART_TX_BUFFER_SIZE
static unsigned char g_pcUARTTxBuffer[UART_TX_BUFFER_SIZE];
static volatile uint32_t g_ui32UARTTxWriteIndex = 0;
static volatile uint32_t g_ui32UARTTxReadIndex = 0;

static uint32_t
GetBufferCount(volatile uint32_t *pui32Read,
		volatile uint32_t *pui32Write, uint32_t ui32Size)
{
	uint32_t ui32Write;
	uint32_t ui32Read;

	ui32Write = *pui32Write;
	ui32Read = *pui32Read;

	return((ui32Write >= ui32Read) ? (ui32Write - ui32Read) :
			(ui32Size - (ui32Read - ui32Write)));
}

static bool
IsBufferEmpty(volatile uint32_t *pui32Read,
		volatile uint32_t *pui32Write)
{
	uint32_t ui32Write;
	uint32_t ui32Read;

	ui32Write = *pui32Write;
	ui32Read = *pui32Read;

	return((ui32Write == ui32Read) ? true : false);
}
static bool
IsBufferFull(volatile uint32_t *pui32Read,
		volatile uint32_t *pui32Write, uint32_t ui32Size)
{
	uint32_t ui32Write;
	uint32_t ui32Read;

	ui32Write = *pui32Write;
	ui32Read = *pui32Read;

	return((((ui32Write + 1) % ui32Size) == ui32Read) ? true : false);
}
static void
UARTPrimeTransmit(uint32_t ui32Base)
{
    //
    // Do we have any data to transmit?
    //
    if(!TX_BUFFER_EMPTY)
    {
        //
        // Disable the UART interrupt.  If we don't do this there is a race
        // condition which can cause the read index to be corrupted.
        //
        MAP_TimerIntDisable(TIMER_RF, TIMER_TIMA_TIMEOUT);

        //
        // Yes - take some characters out of the transmit buffer and feed
        // them to the UART transmit FIFO.
        //
        while(MAP_UARTSpaceAvail(UART_RF) && !TX_BUFFER_EMPTY)
        {
            MAP_UARTCharPutNonBlocking(UART_RF,
                                      g_pcUARTTxBuffer[g_ui32UARTTxReadIndex]);
            ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxReadIndex);
        }

        //
        // Reenable the UART interrupt.
        //
        MAP_TimerIntEnable(TIMER_RF, TIMER_TIMA_TIMEOUT);
    }
}
extern int RFsend(uint8_t* pcBuf,uint32_t ui32Len)
{
	uint32_t uIdx;
	for (uIdx=0 ; uIdx<ui32Len ; uIdx++)
	{
		if(!TX_BUFFER_FULL)
		{
			g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = pcBuf[uIdx];
			ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
		}
		else
		{
			//
			// Buffer is full - discard remaining characters and return.
					//
			break;
		}
	}
	//
	// If we have anything in the buffer, make sure that the UART is set
	// up to transmit it.
	//
	if(!TX_BUFFER_EMPTY)
	{
		UARTPrimeTransmit(UART_RF);
		MAP_TimerIntEnable(TIMER_RF,TIMER_TIMA_TIMEOUT);
	}

	//
	// Return the number of characters written.
	//
	return(uIdx);
}
static void TimerRFIntHandler()
{
	uint32_t ui32Ints = MAP_TimerIntStatus(TIMER_RF, true);
	MAP_TimerIntClear(TIMER_RF, ui32Ints);
    UARTPrimeTransmit(UART_RF);

     //
     // If the output buffer is empty, turn off the transmit interrupt.
     //
     if(TX_BUFFER_EMPTY)
     {
         MAP_TimerIntDisable(TIMER_RF, TIMER_TIMA_TIMEOUT);
     }
}
extern void RFInit()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	ROM_TimerConfigure(TIMER_RF, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER_RF, TIMER_A, (ROM_SysCtlClockGet() / 1000) * TIMER_RF_INTERVAL_MS - 1);

	ROM_IntMasterEnable();
	TimerIntRegister(TIMER_RF, TIMER_A, &TimerRFIntHandler);
	ROM_IntEnable(INT_TIMER1A);
	//ROM_TimerIntEnable(TIMER_RF, TIMER_TIMA_TIMEOUT);
	ROM_TimerIntClear(TIMER_RF, TIMER_TIMA_TIMEOUT);
	ROM_TimerEnable(TIMER_RF, TIMER_A);

}
