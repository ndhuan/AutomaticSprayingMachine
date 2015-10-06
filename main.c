/*
 * main.c
 */

#include "include.h"

//#define DEBUG_LOCAL_POS

extern bool flagNewMsg,flagNewPos,isAuto;
bool flagFirstPos=true;
extern uint8_t MsgBuf[MAX_GPS_MSG_BYTE];
extern uint32_t msgLen;
float x=0.0,y=0.0,pre_x=0.0,pre_y=0.0,angle=0.0;



void main(void) {
	//Enable FPU
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	// Configure clock 80 MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	ConfigGPIO();
	ConfigMyUART();
	RFInit();
	ConfigEncoder();//PD6,PD7

	ConfigPWM_Steering_usingTimer();//PB6, PB7
	ConfigPWM_SStop_Throttle();//PD1
	ConfigPWM_SprayValve();

	ConfigPulseTimer_Steering_Mode();
	ConfigPulseTimer_SStop_Throttle();

	ConfigPIDTimer(PID_PERIOD_MS);

	ROM_IntMasterEnable();

	InitPID();
	HBridgeEnable();//PE2
	PIDPositionSet(0,0);
	SSTOP_START;


	while(1)
	{
		if (flagNewPos)
		{
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
				flagFirstPos = false;
				continue;
			}
			//angle = atan2f(y-pre_y,x-pre_x);

		}
		if (flagNewMsg)
		{
			static int printStep=0;
			LED_BLUE_TOGGLE;
			flagNewMsg=0;
			//truyen frame gps ve may
			printStep++;
			if (printStep==2)
			{
				RFsend(MsgBuf,msgLen);
				printStep=0;
			}
		}
	}

}
