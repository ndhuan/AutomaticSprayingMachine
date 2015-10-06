/*
 * MYUART.c
 *
 *  Created on: Jun 14, 2015
 *      Author: AntoniTran
 */

#include "include.h"

unsigned char ConfigUART(UARTType *UART)
{
	unsigned long ulSet = 0;
	//Configure GPIO for UART Mode
	switch ((*UART).PortName)
	{
		case UART0_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
			GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			GPIOPinConfigure(GPIO_PA0_U0RX);
			GPIOPinConfigure(GPIO_PA1_U0TX);
			break;
		case UART1_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
			GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			GPIOPinConfigure(GPIO_PB0_U1RX);
			GPIOPinConfigure(GPIO_PB1_U1TX);
			break;
		case UART2_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
			GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinConfigure(GPIO_PD6_U2RX);
			GPIOPinConfigure(GPIO_PD7_U2TX);
			break;
		case UART3_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
			GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinConfigure(GPIO_PC6_U3RX);
			GPIOPinConfigure(GPIO_PC7_U3TX);
			break;
		case UART4_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
			GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
			GPIOPinConfigure(GPIO_PC4_U4RX);
			GPIOPinConfigure(GPIO_PC5_U4TX);
			break;
		case UART5_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
			GPIOPinConfigure(GPIO_PE4_U5RX);
			GPIOPinConfigure(GPIO_PE5_U5TX);
			break;
		case UART6_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
			GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
			GPIOPinConfigure(GPIO_PD4_U6RX);
			GPIOPinConfigure(GPIO_PD5_U6TX);
			break;
		case UART7_BASE:
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
			GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			GPIOPinConfigure(GPIO_PE0_U7RX);
			GPIOPinConfigure(GPIO_PE1_U7TX);
			break;
		default:
			return(INVALID_UART_NAME);
	}

	switch ((*UART).DataBits)
	{
		case 5:
			ulSet |= UART_CONFIG_WLEN_5;
			break;
		case 6:
			ulSet |= UART_CONFIG_WLEN_6;
			break;
		case 7:
			ulSet |= UART_CONFIG_WLEN_7;
			break;
		case 8:
			ulSet |= UART_CONFIG_WLEN_8;
			break;
		default:
			return(INVALID_DATA_BITS);
	}

	switch ((*UART).Parity)
	{
		case None:
			ulSet |= UART_CONFIG_PAR_NONE;
			break;
		case Odd:
			ulSet |= UART_CONFIG_PAR_ODD;
			break;
		case Even:
			ulSet |= UART_CONFIG_PAR_EVEN;
			break;
		case One:
			ulSet |= UART_CONFIG_PAR_ONE;
			break;
		case Zero:
			ulSet |= UART_CONFIG_PAR_ZERO;
			break;
		default:
			return(INVALID_PARITY_BIT);
	}

	switch ((*UART).StopBits)
	{
		case 1:
			ulSet |= UART_CONFIG_STOP_ONE;
			break;
		case 2:
			ulSet |= UART_CONFIG_STOP_TWO;
			break;
		default:
			return(INVALID_STOP_BIT);
	}
	//Configure UART
	UARTConfigSetExpClk((*UART).PortName, ROM_SysCtlClockGet(), (*UART).BaudRate, ulSet);
	if ((*UART).ISR)
	{
		UARTIntRegister((*UART).PortName, (*UART).ISR);
		UARTIntEnable((*UART).PortName, UART_INT_RX | UART_INT_RT);
	}
	UARTEnable((*UART).PortName);
	if ((*UART).ISR)
	{
		IntMasterEnable();
	}
  	return(ERROR_NONE);
}

void UARTPuts(uint32_t UART_Base, const char *s)
{
	while(*s)
	{
		ROM_UARTCharPutNonBlocking(UART_Base, *s++);
#ifdef NETWORK_ENABLE
		if (UART_Base == UART0_BASE)
		{
			CheckNetworkFrame(1);
		}
#endif
	}
#ifdef NETWORK_ENABLE
	CheckNetworkFrame(1);
#endif
}


void UARTPutn(uint32_t UART_Base, long Num)
{
	unsigned long temp = 1;
	long NumTemp;
	NumTemp = Num;
	if (Num == 0)
	{
		ROM_UARTCharPutNonBlocking(UART_Base, 48);
#ifdef NETWORK_ENABLE
		if (UART_Base == UART0_BASE)
		{
			CheckNetworkFrame(0);
		}
#endif
	}
	else
	{
		if (Num < 0)
		{
			ROM_UARTCharPutNonBlocking(UART_Base, '-');
#ifdef NETWORK_ENABLE
			if (UART_Base == UART0_BASE)
			{
				CheckNetworkFrame(0);
			}
#endif
			Num *= -1;
		}
		while (NumTemp)
		{
			NumTemp /= 10;
			temp *= 10;
		}
		temp /= 10;
		while (temp)
		{
			ROM_UARTCharPutNonBlocking(UART_Base,(Num / temp) % 10 + 48);
#ifdef NETWORK_ENABLE
			if (UART_Base == UART0_BASE)
			{
				CheckNetworkFrame(0);
			}
#endif
			temp /= 10;
		}
	}
#ifdef NETWORK_ENABLE
	CheckNetworkFrame(1);
#endif
}

void CheckNetworkFrame(uint8_t reset)
{
	static uint8_t ByteCount = 0;	//Used for split Network frame
	ByteCount++;
	if (reset)
	{
		ByteCount = 8;
	}
	if (ByteCount == 8)
	{
		ROM_UARTCharPutNonBlocking(UART0_BASE, '\r');
		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 300);	//Delay 10ms
		ByteCount = 0;
	}
}

void UARTPut_uint32(uint32_t UART_Base, uint32_t num2send)
{
	int i;
	uint32_t temp=num2send;
	for(i=4;i>0;i--)
	{
		ROM_UARTCharPutNonBlocking (UART_Base, (char)((temp&0xff000000)>>24));
		temp=temp<<8;
	}

}



