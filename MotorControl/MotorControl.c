/*
 * MotorControl.c
 *
 *  Created on: Oct 6, 2015
 *      Author: Huan
 */
#include "include.h"

#define SOFT_START_PERIOD_POS 20
#define START_DUTY 200

static int32_t setPoint;
static int32_t delta=0;
static uint32_t count=0;
static int32_t pos;
void motorSet(int32_t tmpSetPoint, int32_t tmpDelta)
{
	setPoint = tmpSetPoint;
	if (setPoint > MAX_STEERING_SETPOINT)
		setPoint = MAX_STEERING_SETPOINT;
	else if (setPoint < -MAX_STEERING_SETPOINT)
		setPoint = -MAX_STEERING_SETPOINT;

	delta = tmpDelta;
	if (delta <= 0)
		MRC_pos_U.uc = setPoint;
}

void motorControl()
{
	int i;
	pos = (int)ROM_QEIPositionGet(QEI0_BASE);

	if (delta>0)
	{
		if (MRC_pos_U.uc < setPoint-delta)
			MRC_pos_U.uc += delta;
		else if (MRC_pos_U.uc > setPoint+delta)
			MRC_pos_U.uc -= delta;
		else
			MRC_pos_U.uc = setPoint;
	}

	MRC_pos_U.y = pos;
	MRC_pos_step();

	if ((count<SOFT_START_PERIOD_POS)//handle noise when power on
			||((count<2*SOFT_START_PERIOD_POS) && (setPoint!=0)))//soft start
	{
		count++;
		if (MRC_pos_Y.u>START_DUTY)
			MRC_pos_Y.u=START_DUTY;
		else if (MRC_pos_Y.u<-START_DUTY)
			MRC_pos_Y.u=-START_DUTY;
	}

	SetPWM_Steering_usingTimer(TIMER0_BASE,PWM_STEERING_F,(long)MRC_pos_Y.u );

	LMSEstimate_pos_U.u = MRC_pos_Y.u;
	LMSEstimate_pos_U.y = pos;
	LMSEstimate_pos_step();

	for (i=0;i<6;i++)
	{
		MRC_pos_U.theta[i] = LMSEstimate_pos_Y.theta[i];
	}
}


