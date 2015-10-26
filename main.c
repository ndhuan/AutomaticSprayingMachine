/*
 * main.c
 */

#include "include.h"

//#define DEBUG_LOCAL_POS

extern bool flagNewGPSMsg,flagNewPos,flagControl,isAuto;
extern uint8_t MsgBuf[MAX_GPS_MSG_BYTE];
extern uint32_t msgLen;

bool flagFirstPos=true;
float x=0.0,y=0.0,pre_x=0.0,pre_y=0.0,angle=0.0;
void ButtonsISR()
{
	GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3);
	isAuto = !isAuto;
	if (isAuto)
		LED3_ON;
	else
		LED3_OFF;
}
void Button_init(void)
{
//	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;
//    HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) &= ~0x01;
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
    ROM_GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_FALLING_EDGE);
	GPIOIntRegister(GPIO_PORTA_BASE, &ButtonsISR);
	ROM_IntEnable(INT_GPIOA);
	GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);
}
void main(void) {
	//Enable FPU
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	// Configure clock 80 MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	Button_init();
	ConfigGPIO();
	ConfigMyUART();
	RFinit();
	ConfigEncoder();//PD6,PD7

	ConfigPWM_Steering_usingTimer();//PB6, PB7
	ConfigPWM_SStop_Throttle();//PD1
	ConfigPWM_SprayValve();

	ConfigPulseTimer_Steering_Mode();
	ConfigPulseTimer_SStop_Throttle();

	ConfigSteeringControlTimer(CONTROL_PERIOD_MS);

	steeringControlInit();
	throttleControlInit();
	PathFollowInit();

	HBridgeEnable();//PE2

	SSTOP_START;

	ROM_IntMasterEnable();

	while(1)
	{
		if (flagNewPos)
		{
			static int i=0, x_calib=0, y_calib=0;
			flagNewPos=0;
			LED_GREEN_TOGGLE;
#ifdef DEBUG_LOCAL_POS
			UARTPutn(UART_DEBUG,(int)x);
			ROM_UARTCharPutNonBlocking(UART_DEBUG,',');
			UARTPutn(UART_DEBUG,(int)y);
			ROM_UARTCharPutNonBlocking(UART_DEBUG,'\n');
#endif
			//tinh goc
			if (flagFirstPos)
			{

				x_calib += x;
				y_calib += y;
				i++;
				if (i==10)
				{
					x_calib /= 10;
					y_calib /= 10;
					flagFirstPos = false;
				}

			}
			else
			{
				if (isAuto)
				{
					angle = atan2f(y-pre_y,x-pre_x);
					PathFollow(x-x_calib,y-y_calib,angle);
				}
			}
		}
		if (flagNewGPSMsg)//send gps frame to PC
		{
			static int printStep=0;
			LED_BLUE_TOGGLE;
			flagNewGPSMsg=0;
			printStep++;
			if (printStep==2)
			{
				RFsend(MsgBuf,msgLen);
				printStep=0;
			}
		}
	}

}
