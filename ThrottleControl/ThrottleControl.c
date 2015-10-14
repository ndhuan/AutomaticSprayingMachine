/*
 * ThrottleControl.c
 *
 *  Created on: Oct 13, 2015
 *      Author: Huan
 */
#include "include.h"
#define START_BYTE 0XAA

//PIDType pidThrottle;

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
