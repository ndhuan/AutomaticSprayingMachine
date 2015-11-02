/*
 * InterruptHandler.c
 *
 *  Created on: May 9, 2015
 *      Author: AntoniTran
 */

#include "include.h"

//#define DEBUG_DELTAT_STEERING
//#define DEBUG_THROTTLE_SETPOINT

#if defined(DEBUG_DELTAT_STEERING)
|| defined(DEBUG_DELTAT_SETPOINT)
|| defined(DEBUG_DELTAT_THROTTLE)
|| defined(DEBUG_THROTTLE_SETPOINT)
static int printStep=0;
#endif
#define PID_SCALE 100000

#define STEERING_MID 126000//thay doi theo pin ;D
#define DEADBAND_STEERING 4000
#define T2VEL 130//50000/6s/50Hz
#define DEADBAND_THROTTLE 4000
#define PULSE2VEL 100

#define THROTTLE_SCALING 1000
#define THROTTLE_OFFSET 0

#define START_BYTE (char)0XAA


extern float x,y,pre_x,pre_y,headingAngle;

bool flagNewGPSMsg=false,flagNewPos=false;
bool isAuto=false;
bool isRunning=true;

static bool hasFixed=false;

#ifndef USE_QEI
static int32_t Position = 0;
static int32_t Speed = 0;
static int32_t Speedtemp = 0;
#endif

uint8_t MsgBuf[MAX_GPS_MSG_BYTE];
uint32_t msgLen=0;

//int32_t i32Pulse_Throttle;
//int32_t i32Pulse_SStop;
//int32_t i32Pulse_Mode;

static uint32_t ui32T_Edgeup_Steering, ui32T_Edgedown_Steering;
static int32_t i32DeltaT_Steering,i32Pulse_Steering,i32SteeringMid=0;

static uint32_t ui32T_Edgeup_Throttle, ui32T_Edgedown_Throttle;
static int32_t i32DeltaT_Throttle,i32Pulse_Throttle;

static uint32_t ui32T_Edgeup_SStop, ui32T_Edgedown_SStop;
static int32_t i32DeltaT_SStop;

static uint32_t ui32T_Edgeup_Mode, ui32T_Edgedown_Mode;
static int32_t i32DeltaT_Mode;

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
	if (ROM_GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1)&GPIO_PIN_1)
		ui32T_Edgeup_Throttle = ROM_TimerValueGet(WTIMER2_BASE, TIMER_B);
	else
	{
		static int32_t sp=0,pre_sp=0;
		if (firstEdgeThrottle)
		{
			firstEdgeThrottle=0;
			return;
		}

		ui32T_Edgedown_Throttle = ROM_TimerValueGet(WTIMER2_BASE, TIMER_B);

		i32DeltaT_Throttle = (int32_t)(ui32T_Edgedown_Throttle - ui32T_Edgeup_Throttle);

		if ((i32DeltaT_Throttle<70000) || (i32DeltaT_Throttle>170000))
			return;

#ifdef DEBUG_DELTAT_THROTTLE
		printStep++;
		if (printStep==5)
		{
			RFprint("Throttle sp:%d\r\n", i32DeltaT_Throttle);
			printStep = 0;
		}
#endif


		sp = (i32DeltaT_Throttle-80000)/PULSE2VEL;
		if (sp<0)
			sp=0;
		else if (sp>1000)
			sp=1000;
		if ((sp - pre_sp > 2)||(sp - pre_sp < -2))
			throttleSet(sp);
		pre_sp = sp;

#ifdef DEBUG_THROTTLE_SETPOINT
		printStep++;
		if (printStep==5)
		{
			RFprint("Throttle sp:%d\r\n", sp);
			printStep = 0;
		}
#endif
	}
}

void Steering_WTimer3AISR(void){
	ROM_TimerIntClear(WTIMER3_BASE, TIMER_CAPA_EVENT);
	// Doc trang thai canh ngat
	if (isAuto)
		return;
	if (ROM_GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_2)&GPIO_PIN_2)
		ui32T_Edgeup_Steering = ROM_TimerValueGet(WTIMER3_BASE, TIMER_A);
	else
	{
		static int32_t sp = 0, i=0;
		if (firstEdgeSteering)
		{
			firstEdgeSteering=0;
			return;
		}

		ui32T_Edgedown_Steering = ROM_TimerValueGet(WTIMER3_BASE, TIMER_A);

		i32DeltaT_Steering = (int32_t)(ui32T_Edgedown_Steering - ui32T_Edgeup_Steering);

		if (i<10)
		{
			i32SteeringMid += i32DeltaT_Steering;
			i++;
			if (i==10)
			{
				i32SteeringMid = i32SteeringMid/10;
			}
			return;
		}

		i32Pulse_Steering =  i32DeltaT_Steering-i32SteeringMid;
		if ((i32Pulse_Steering<-50000) || (i32Pulse_Steering>50000))
			return;

		//		UARTPutn(UART0_BASE, i32Pulse_Steering);
		//		ROM_UARTCharPut(UART0_BASE, '\n');

#ifdef DEBUG_DELTAT_STEERING
		printStep++;
		if (printStep==5)
		{
			RFprint("Steering:%d\r\n",i32DeltaT_Steering);
			printStep = 0;
		}
#endif

		if (i32Pulse_Steering < -DEADBAND_STEERING)
		{
			i32Pulse_Steering = sp + (i32Pulse_Steering+DEADBAND_STEERING)/T2VEL;

			if (i32Pulse_Steering<-MAX_STEERING_SETPOINT)
				sp = -MAX_STEERING_SETPOINT;
			else
				sp = i32Pulse_Steering;
			steeringSet(sp,-1);

		}
		else if (i32Pulse_Steering > DEADBAND_STEERING)
		{
			i32Pulse_Steering = sp + (i32Pulse_Steering-DEADBAND_STEERING)/T2VEL;

			if (i32Pulse_Steering>MAX_STEERING_SETPOINT)
				sp = MAX_STEERING_SETPOINT;
			else
				sp = i32Pulse_Steering;
			steeringSet(sp,-1);
		}
#ifdef DEBUG_STEERING_SETPOINT
		printStep++;
		if (printStep==5)
		{
			RFprint("Steering sp:%d\r\n", sp);
			printStep = 0;
		}
#endif


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
		else if ((i32DeltaT_SStop>135000) && (i32DeltaT_SStop<145000) && isRunning)
			//both RF receiver and PC gui must start
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

void Control_Timer5ISR(void)
{
	ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
#ifdef USE_QEI
	//Get Velocity
	//	Speed = ROM_QEIVelocityGet(QEI0_BASE) * ROM_QEIDirectionGet(QEI0_BASE);
	//Get Position
	//	Position = ((int32_t)ROM_QEIPositionGet(QEI0_BASE));
#else
	Speed = Speedtemp;
#endif

	steeringControl();
	LED_RED_TOGGLE;
}

void BattSense_Timer3ISR(void)
{
	ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	ROM_ADCProcessorTrigger(ADC0_BASE, 3);
}

void BattSenseISR(void)
{
	uint32_t ADCResult, BatteryVoltage;
	ROM_ADCIntClear(ADC0_BASE, 3);
	ROM_ADCSequenceDataGet(ADC0_BASE, 3, (uint32_t *)&ADCResult);
	BatteryVoltage = ((float)ADCResult) / 4096 * 3.3 * (220 + 680) / 220;
#warning CHECK BATT SENSE
#if 0
	if (BatteryVoltage < (float)11.0)
	{
		HBridgeEnable();
		throttleStop();
		while(1)
		{
			LED3_TOGGLE;
			ROM_SysCtlDelay(ROM_SysCtlClockGet()/3);
		}
	}
#endif
}

void Button1_ISR()
{
	isAuto = !isAuto;
	if (isAuto)
	{
		hasFixed = true;
		LED4_ON;
	}
	else
	{
		hasFixed = false;
		LED4_OFF;
	}
}

void Button2_ISR()
{
	isAuto = !isAuto;
	if (isAuto)
	{
		hasFixed = true;
		LED4_ON;
	}
	else
	{
		hasFixed = false;
		LED4_OFF;
	}
}

void HandleGPSMsg(uint8_t* Msg)
{
	int i,sign=1,x_temp=0,y_temp=0;
	static int fixCnt=0,floatCnt=0;

	//check time
	if ((Msg[0]!='2') || (Msg[1]!='0') ||
			(Msg[2]<'0') || (Msg[2]>'9') ||
			(Msg[3]<'0') || (Msg[3]>'9') ||
			(Msg[4]!='/'))
		return;

//	hasFixed = true;

	if (Msg[71] == '1')//fix sol
	{
		if (!hasFixed)
		{
			if (fixCnt<25)
			{
				fixCnt++;
				return;
			}
			else//25 continuous fix solutions
			{
				hasFixed=true;
			}
		}
	}

	if (hasFixed)
	{
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
		sign=1;
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
			HandleGPSMsg(MsgBuf);
			flagNewGPSMsg=true;
			msgLen=numBytes;
			numBytes = 0;
		}
		if (numBytes==MAX_GPS_MSG_BYTE)
			numBytes=0;

	}
}

static void processRFMsg(uint8_t *msg)
{
	switch (msg[1])
	{
	case STOP_CMD:
		SSTOP_STOP;
		isRunning = false;
		break;
	case START_CMD:
		isRunning = true;
		break;
	case AUTO_MODE_CMD:
		isAuto = true;
		break;
	case MANUAL_MODE_CMD:
		isAuto = false;
		break;
	case SET_PID_DISTANCE_PARAMS:
	{
		int32_t Kp,Ki,Kd;
		msg += 3;
		Kp=(msg[0]|msg[1]<<8|msg[2]<<16|msg[3]<<24);
		Ki=(msg[4]|msg[5]<<8|msg[6]<<16|msg[7]<<24);
		Kd=(msg[8]|msg[9]<<8|msg[10]<<16|msg[11]<<24);
		setParamsDistance((float)Kp/PID_SCALE,(float)Ki/PID_SCALE,(float)Kd/PID_SCALE);
		break;
	}
	case SET_PID_ANGLE_PARAMS:
	{
		int32_t Kp,Ki,Kd;
		msg += 3;
		Kp=(msg[0]|msg[1]<<8|msg[2]<<16|msg[3]<<24);
		Ki=(msg[4]|msg[5]<<8|msg[6]<<16|msg[7]<<24);
		Kd=(msg[8]|msg[9]<<8|msg[10]<<16|msg[11]<<24);
		setParamsAngle((float)Kp/PID_SCALE,(float)Ki/PID_SCALE,(float)Kd/PID_SCALE);
		break;
	}
	}
}
//frame=START_BYTE + CMD_ID + LENGTH_BYTE + payload (LSB first)
void UartRFIntHandler()
{
	static uint8_t msgRF[20];
	static uint32_t msgRFLen=0;
	static uint32_t numBytes=0;
	uint32_t status = ROM_UARTIntStatus(UART_RF,true);
	ROM_UARTIntClear(UART_RF,status);
	while (ROM_UARTCharsAvail(UART_RF))
	{
		char temp=ROM_UARTCharGetNonBlocking(UART_RF);
		if (numBytes==0)
		{
			if (temp==START_BYTE)
			{
				msgRF[numBytes++]=temp;
			}
			continue;
		}
		msgRF[numBytes++]=temp;
		if (numBytes==3)
			msgRFLen = msgRF[2] + 3;
		if ((numBytes==msgRFLen) && (msgRFLen!=0))
		{
			processRFMsg(msgRF);
			numBytes=0;
			msgRFLen=0;
		}
	}
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
