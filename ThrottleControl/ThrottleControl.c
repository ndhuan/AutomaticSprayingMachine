/*
 * ThrottleControl.c
 *
 *  Created on: Oct 13, 2015
 *      Author: Huan
 */
#include "include.h"
#define START_BYTE 0XAA

//PIDType pidThrottle;
static bool flagTimeout=0;

void throttleControlInit()//already init uart
{
	//	initPID(&pidThrottle,1.0,0.1,0.0);
	//	pidSet(&pidThrottle, 0);
}
void throttleSet(int32_t setPoint)
{
	//truyen uart qua
	uint8_t msg[7];
	int i;
	msg[0] = START_BYTE;
	msg[1] = SETPOINT_CMD;
	msg[2] = 4;
	msg[3] = setPoint;
	msg[4] = setPoint>>8;
	msg[5] = setPoint>>16;
	msg[6] = setPoint>>24;
	for (i=0;i<7;i++)
		ROM_UARTCharPut(UART_THROTTLE,msg[i]);
	//SetPWM_Servo_Throttle(PWM_THROTTLE,setPoint);
}
void throttleStop()
{
	//truyen uart qua
	uint8_t msg[3];
	int i;
	msg[0] = START_BYTE;
	msg[1] = STOP_CMD;
	msg[2] = 0;

	for (i=0;i<3;i++)
		ROM_UARTCharPut(UART_THROTTLE,msg[i]);
	//SetPWM_Servo_Throttle(PWM_THROTTLE,setPoint);
}

void throttleReset()
{
	uint8_t msg[3];
	int i;
	msg[0] = START_BYTE;
	msg[1] = RESET_CMD;
	msg[2] = 0;

	for (i=0;i<3;i++)
		ROM_UARTCharPut(UART_THROTTLE,msg[i]);
}
void Home_Timer2ISR()
{
	ROM_TimerIntClear(TIMER2_BASE,TIMER_A);
	flagTimeout = 1;
}
void throttleHome()
{
	uint8_t msg[3];
	int i;
	if (!LIMIT_SW_THROTTLE_DOWN_ON)
	{
		ROM_TimerIntDisable(WTIMER2_BASE, TIMER_CAPB_EVENT);//disable manual mode

		msg[0] = START_BYTE;
		msg[1] = HOME_CMD;
		msg[2] = 0;

		for (i=0;i<3;i++)
			ROM_UARTCharPut(UART_THROTTLE,msg[i]);

		ROM_TimerLoadSet(TIMER2_BASE, TIMER_A,ROM_SysCtlClockGet()*5);
		ROM_TimerEnable(TIMER2_BASE, TIMER_A);

		while ((!LIMIT_SW_THROTTLE_DOWN_ON) && (!flagTimeout));

		ROM_TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);
	}
}
