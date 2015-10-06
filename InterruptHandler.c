/*
 * InterruptHandler.c
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */

#include "include.h"

#define STEERING_MID 122000

uint8_t MsgBuf[MAX_GPS_MSG_BYTE];
uint32_t msgLen=0;
extern PIDType PIDPosition;
bool flagNewMsg=false,flagNewPos=false;
extern float x,y,pre_x,pre_y,angle;
bool isAuto=false;
int32_t Position = 0;
int32_t Speed = 0;
#ifndef USE_QEI
int32_t Speedtemp = 0;
#endif
int32_t i32Pulse_Steering;
int32_t i32Pulse_Throttle;
int32_t i32Pulse_SStop;
int32_t i32Pulse_Mode;

uint32_t ui32T_Edgeup_Steering, ui32T_Edgedown_Steering;
int32_t i32DeltaT_Steering;

uint32_t ui32T_Edgeup_Throttle, ui32T_Edgedown_Throttle;
int32_t i32DeltaT_Throttle;

uint32_t ui32T_Edgeup_SStop, ui32T_Edgedown_SStop;
int32_t i32DeltaT_SStop;

uint32_t ui32T_Edgeup_Mode, ui32T_Edgedown_Mode;
int32_t i32DeltaT_Mode;

static bool firstEdgeSteering=1;
static bool firstEdgeThrottle=1;
static bool firstEdgeSStop=1;
static bool firstEdgeMode=1;

void HBridgeEnable(void)
{
	ROM_GPIOPinWrite(ENABLE_PORT, ENABLE_PIN, 0xFF);
}

void HBridgeDisable(void)
{
	ROM_GPIOPinWrite(ENABLE_PORT, ENABLE_PIN, 0);
}

void Throttle_WTimer2BISR(void){
	ROM_TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
	// Doc trang thai canh ngat
	if (GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1)&GPIO_PIN_1)
		ui32T_Edgeup_Throttle = ROM_TimerValueGet(WTIMER2_BASE, TIMER_B);
	else
	{
		if (firstEdgeThrottle)
		{
			firstEdgeThrottle=0;
			return;
		}

		ui32T_Edgedown_Throttle = ROM_TimerValueGet(WTIMER2_BASE, TIMER_B);

		i32DeltaT_Throttle = (int32_t)(ui32T_Edgedown_Throttle - ui32T_Edgeup_Throttle - 120000);
		if ((i32DeltaT_Throttle>-40000) && (i32DeltaT_Throttle<40000))
			SetPWM_Servo_Throttle(PWM_THROTTLE,i32DeltaT_Throttle/1000+85);
//		if ((i32DeltaT_Steering>100000)&(i32DeltaT_Steering<140000))
//		{
//			LED2_ON;
//			LED3_OFF;
//			LED4_OFF;
//		}
//		else if ((i32DeltaT_Steering>140000))
//		{
//			LED2_OFF;
//			LED3_ON;
//			LED4_OFF;
//		}
//		else
//		{
//			LED2_OFF;
//			LED3_OFF;
//			LED4_ON;
//		}

	}
}

void Steering_WTimer3AISR(void){//3 red: 90000->160000
	ROM_TimerIntClear(WTIMER3_BASE, TIMER_CAPA_EVENT);
	// Doc trang thai canh ngat
	if (isAuto)
		return;
	if (ROM_GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_2)&GPIO_PIN_2)
		ui32T_Edgeup_Steering = ROM_TimerValueGet(WTIMER3_BASE, TIMER_A);
	else
	{
		if (firstEdgeSteering)
		{
			firstEdgeSteering=0;
			return;
		}

		ui32T_Edgedown_Steering = ROM_TimerValueGet(WTIMER3_BASE, TIMER_A);

		i32DeltaT_Steering = (int32_t)(ui32T_Edgedown_Steering - ui32T_Edgeup_Steering);
		i32Pulse_Steering =  i32DeltaT_Steering-STEERING_MID;
		if ((i32Pulse_Steering<-50000) || (i32Pulse_Steering>50000))
			return;

//		UARTPutn(UART0_BASE, i32Pulse_Steering);
//		UARTCharPut(UART0_BASE, '\n');

		//UARTPutn(UART0_BASE,i32DeltaT_Steering);
		//ROM_UARTCharPut(UART0_BASE,'\n');

		if (i32Pulse_Steering < -4000)
		{
			i32Pulse_Steering = PIDPosition.SetPoint + (i32Pulse_Steering+4000)/160;
			//i32Pulse_Steering = PIDPosition.SetPoint + (i32Pulse_Steering+2000)/100;
			PIDPosition.Enable = 0;
			if (i32Pulse_Steering<-25000)//-45 degree
				PIDPosition.SetPoint = -25000;
			else
				PIDPosition.SetPoint = i32Pulse_Steering;
			PIDPosition.Enable = 1;
		}
		else if (i32Pulse_Steering > 4000)
		{
			i32Pulse_Steering = PIDPosition.SetPoint + (i32Pulse_Steering-4000)/160;
			//i32Pulse_Steering = PIDPosition.SetPoint + (i32Pulse_Steering-2000)/100;
			PIDPosition.Enable = 0;
			if (i32Pulse_Steering>25000)
				PIDPosition.SetPoint = 25000;//45 degree
			else
				PIDPosition.SetPoint = i32Pulse_Steering;
			PIDPosition.Enable = 1;
		}

		//UARTPutn(UART0_BASE, PIDPosition.SetPoint);
		//UARTCharPut(UART0_BASE, '\n');

		//PIDPositionSet(i32Pulse_Steering);

		//80000 = 1ms, 1200000 = 1.5ms, 160000=2ms
//		if ((i32DeltaT_Steering>100000)&(i32DeltaT_Steering<140000))
//		{
//			LED2_OFF;
//			LED3_ON;
//			LED4_OFF;
//		}
//		else if ((i32DeltaT_Steering>140000))
//		{
//			LED2_OFF;
//			LED3_OFF;
//			LED4_ON;
//		}
//		else
//		{
//			LED2_ON;
//			LED3_OFF;
//			LED4_OFF;
//		}

	}
}

void SStop_WTimer2AISR(void){
	ROM_TimerIntClear(WTIMER2_BASE, TIMER_CAPA_EVENT);

	// Doc trang thai canh ngat
	if (GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_0)&GPIO_PIN_0)
		ui32T_Edgeup_SStop = ROM_TimerValueGet(WTIMER2_BASE, TIMER_A);
	else
	{

		if (firstEdgeSStop)
		{
			firstEdgeSStop=0;
			return;
		}

		ui32T_Edgedown_SStop = ROM_TimerValueGet(WTIMER2_BASE, TIMER_A);

		i32DeltaT_SStop = (int32_t)(ui32T_Edgedown_SStop - ui32T_Edgeup_SStop);

//				UARTPutn(UART0_BASE,i32DeltaT_SStop);
//				UARTCharPut(UART0_BASE,'\n');

		if ((i32DeltaT_SStop<125000) && (i32DeltaT_SStop>115000))
			SSTOP_STOP;
		else if ((i32DeltaT_SStop>135000) && (i32DeltaT_SStop<145000))
			SSTOP_START;
	}
}

void Mode_WTimer3BISR(void){
	ROM_TimerIntClear(WTIMER3_BASE, TIMER_CAPB_EVENT);
	// Doc trang thai canh ngat
	if (GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_3)&GPIO_PIN_3)
		ui32T_Edgeup_Mode = ROM_TimerValueGet(WTIMER3_BASE, TIMER_B);
	else
	{
		if (firstEdgeMode)
		{
			firstEdgeMode=0;
			return;
		}

		ui32T_Edgedown_Mode = ROM_TimerValueGet(WTIMER3_BASE, TIMER_B);

		i32DeltaT_Mode = (int32_t)(ui32T_Edgedown_Mode - ui32T_Edgeup_Mode);

//				UARTPutn(UART0_BASE,i32DeltaT_SStop);
//				UARTCharPut(UART0_BASE,'\n');

//		if ((i32DeltaT_Mode<125000) && (i32DeltaT_Mode>115000))
//			isAuto=true;
//		else if ((i32DeltaT_Mode>135000) && (i32DeltaT_Mode<145000))
//			isAuto=false;
	}
}

void PID_Timer5ISR(void)
{
	ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
#ifdef USE_QEI
	//Get Velocity
	Speed = ROM_QEIVelocityGet(QEI0_BASE) * ROM_QEIDirectionGet(QEI0_BASE);
	//Get Position
	Position = ((int32_t)ROM_QEIPositionGet(QEI0_BASE));
#else
	Speed = Speedtemp;
#endif
	if (PIDPosition.Enable)
	{
#ifdef DEBUG_PID
		static int printStep=0;
#endif
		if (isAuto)
		{
			if (PIDPosition.tempSetPoint>PIDPosition.SetPoint+PIDPosition.deltaPulse)
				PIDPosition.SetPoint = PIDPosition.tempSetPoint-PIDPosition.deltaPulse;
			else if (PIDPosition.tempSetPoint<PIDPosition.SetPoint-PIDPosition.deltaPulse)
				PIDPosition.SetPoint = PIDPosition.tempSetPoint+PIDPosition.deltaPulse;
			else
				PIDPosition.SetPoint = PIDPosition.tempSetPoint;
		}
		PIDCalc(&PIDPosition, Position, 90);
		//			SetPWM_Steering(PWM_STEERING,(long)PIDPosition.PIDResult);
		SetPWM_Steering_usingTimer(TIMER0_BASE,PWM_STEERING,(long)PIDPosition.PIDResult );

#ifdef DEBUG_PID
		printStep++;
		if (printStep==5)
		{
			UARTPutn(UART0_BASE,(long)PIDPosition.PIDResult);
			ROM_UARTCharPut(UART0_BASE,',');
			UARTPutn(UART0_BASE,Position);
			ROM_UARTCharPut(UART0_BASE,'\n');
			printStep=0;
		}
#endif
	}
}

void HandleMsg(uint8_t* Msg)
{
	int i,sign=1,x_temp=0,y_temp=0;
	static int fixCnt=0,floatCnt=0;
	static bool hasFixed=false;
	//check time
	if ((Msg[0]!='2') || (Msg[1]!='0') ||
			(Msg[2]<'0') || (Msg[2]>'9') ||
			(Msg[3]<'0') || (Msg[3]>'9') ||
			(Msg[4]!='/'))
		return;

	if (Msg[71] == '1')//fix sol
	{
		floatCnt=0;
		if (!hasFixed)
		{
			if (fixCnt<25)//25
			{
				fixCnt++;
				return;
			}
			else
			{
				hasFixed=true;
				LED_BLUE_ON;
			}
		}
	}
	else
	{
		fixCnt = 0;
		if (hasFixed)
		{
			if (floatCnt<25)
				floatCnt++;
			else
			{
				hasFixed=false;
				SSTOP_STOP;
				LED_RED_ON;
				return;
			}
		}
		else
			return;
	}

	//get local coordinate
	for (i=24;i<38;i++)
	{
		if (Msg[i] == '-')
			sign = -1;
		else if ((Msg[i]>='0') && (Msg[i]<='9'))
			x_temp = (x_temp<<3) + (x_temp<<1) + Msg[i]-'0';
		else if ((Msg[i]!=' ') && (Msg[i]!='.'))
			return;
	}
	x_temp *= sign;
	for (i=39;i<53;i++)
	{
		if (Msg[i] == '-')
			sign = -1;
		else if ((Msg[i]>='0') && (Msg[i]<='9'))
			y_temp = (y_temp<<3) + (y_temp<<1) + Msg[i]-'0';
		else if ((Msg[i]!=' ') && (Msg[i]!='.'))
			return;
	}
	y_temp *= sign;

	pre_x = x;
	pre_y = y;
	x = x_temp;
	y = y_temp;

	flagNewPos = true;
}
void UartGPSIntHandler()
{
	uint32_t status = ROM_UARTIntStatus(UART_GPS,true);
	static uint32_t numBytes=0;
	ROM_UARTIntClear(UART_GPS,status);
	while (ROM_UARTCharsAvail(UART_GPS))
	{
		if ((MsgBuf[numBytes++]=ROM_UARTCharGetNonBlocking(UART_GPS))=='\n')
		{
			HandleMsg(MsgBuf);
			flagNewMsg=true;
			msgLen=numBytes;
			numBytes = 0;
		}
		if (numBytes==MAX_GPS_MSG_BYTE)
			numBytes=0;

	}
}
void UartRFIntHandler()
{
	uint32_t status = ROM_UARTIntStatus(UART_RF,true);
	ROM_UARTIntClear(UART_RF,status);
}
#ifndef USE_QEI
void EncoderISR(void)
{
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6);
	if (HWREG(GPIO_PORTD_BASE + 0x0200))
	{
		Speedtemp--;
		Position--;
	}
	else
	{
		Speedtemp++;
		Position++;
	}
}
#endif
