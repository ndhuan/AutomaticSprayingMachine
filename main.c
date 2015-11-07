/*
 * main.c
 */

#include "include.h"

#define DEBUG_LOCAL_POS

extern bool flagNewGPSMsg,flagNewPos,isAuto;
extern uint8_t MsgBuf[MAX_GPS_MSG_BYTE];
extern uint32_t msgLen;

bool flagFirstPos=true;
float x=0.0,y=0.0,pre_x=0.0,pre_y=0.0,headingAngle=0.0;

void main(void) {
	//Enable FPU
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	// Configure clock 80 MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	ROM_SysCtlDelay(ROM_SysCtlClockGet()/3);

	Config_Button_LimitSw();
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

	ConfigHomeTimeoutTimer();
	throttleHome();

	ConfigBattSense();

	while(1)
	{
		if (flagNewPos)
		{
			static int i=0, x_calib=0, y_calib=0;
			flagNewPos=0;
			LED_GREEN_TOGGLE;
#ifdef DEBUG_LOCAL_POS
			if (!flagFirstPos)
				RFprint("pos %d,%d\r\n",(int)(x-x_calib),(int)(y-y_calib));
//			{
//				UARTPuts(UART_RF,"pos");
//				UARTPutn(UART_RF,x-x_calib);
//				ROM_UARTCharPut(UART_RF,',');
//				UARTPutn(UART_RF,y-y_calib);
//				ROM_UARTCharPut(UART_RF,'\r');
//				ROM_UARTCharPut(UART_RF,'\n');
//			}
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
				static bool flagFirstAngle=false;
				if (isAuto)
				{
					if ((y-pre_y)*(y-pre_y)+(x-pre_x)*(x-pre_x)>0.0025*D_SCALE*D_SCALE)
					{
						headingAngle = atan2f(y-pre_y,x-pre_x);
						flagFirstAngle = true;
					}
					if (flagFirstAngle)
						PathFollow(x-x_calib,y-y_calib,headingAngle);
				}
			}
		}
		if (flagNewGPSMsg)//send gps frame to PC
		{
			static int printStep=0;
			LED_BLUE_TOGGLE;
			flagNewGPSMsg=0;
			printStep++;
			if (printStep==5)
			{
//				RFsend(MsgBuf,msgLen);
				printStep=0;
			}
		}
	}

}
