/*
 * PathFollowing.c
 *
 *  Created on: Sep 11, 2015
 *      Author: Huan
 */
#include "include.h"

#define FOLLOW_ALGORITHM_PID
//#define FOLLOW_ALGORITHM_P

#define LINE_DISTANCE (float)5.0
#define TURN_DISTANCE (float)2.0
#define TURN_ANGLE (float)40.0
#define DELTA_POS_STRAIGHT 400
#define DELTA_POS_TURN 200

typedef enum{
	START,
	LINE_FOLLOWING,
	TURN_LEFT,
	TURN_RIGHT,
	PRE_STOP,
	FINISH
}eSTATE;
typedef enum{
	UP=0,
	RIGHT=1,
	DOWN=2,
	LEFT=3
}eDIRECTION;

static PIDType pidDistance, pidAngle;

static eSTATE eState=LINE_FOLLOWING;
static eDIRECTION eDir=UP;
static uint32_t linePassed=0;

float x_corner[4];
float y_corner[4];
float angle_dest,angle_orig;
static uint32_t totalLine=0;

void setParamsDistance(float Kp,float Ki,float Kd)
{
	pidSetParams(&pidDistance,Kp,Ki,Kd);
}
void setParamsAngle(float Kp,float Ki,float Kd)
{
	pidSetParams(&pidAngle,Kp,Ki,Kd);
}
float SubMod(float angle1,float angle2)//result:-PI->PI
{
	if (angle1-angle2 > PI)
		return angle1-angle2-2*PI;
	else if (angle1-angle2 < -PI)
		return angle1-angle2+2*PI;
	else
		return angle1-angle2;
}
//			   ___
//	-			*|
//			*
//		*
//	*			+
static float signedDistance(float x, float y, float x1, float y1, float x2, float y2)
{
	if ((x1==y1) && (x2==y2))
		return -1.0;
	return ((y2-y1)*x+(x1-x2)*y-x1*y2+x2*y1)/sqrtf((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
}
static float distance(float x, float y, float x1, float y1, float x2, float y2)
{
	if ((x1==y1) && (x2==y2))
		return -1.0;
	return (fabs((y2-y1)*x+(x1-x2)*y-x1*y2+x2*y1))/sqrtf((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1));
}
void PathFollowInit()
{
	// 3	2
	//
	//
	// 0	1

	x_corner[0] = 1*D_SCALE;//1m
	y_corner[0] = 1*D_SCALE;

	x_corner[1] = 16*D_SCALE;
	y_corner[1] = 1*D_SCALE;

	x_corner[2] = 16*D_SCALE;
	y_corner[2] = 11*D_SCALE;

	x_corner[3] = 1*D_SCALE;
	y_corner[3] = 11*D_SCALE;

	angle_orig = atan2f(y_corner[3]-y_corner[0],x_corner[3]-x_corner[0]);
	angle_dest=angle_orig;

	initPID(&pidDistance,1.0,0.01,0.0);
	initPID(&pidAngle,1.0,0.01,0.0);
	pidSet(&pidDistance, 0);
	pidSet(&pidAngle, 0);

	totalLine = (uint32_t)(distance(x_corner[1],y_corner[1],
			x_corner[0],y_corner[0],x_corner[3],y_corner[3])/LINE_DISTANCE)+1;
}
void FollowLine(float x, float y, float angle, float signedDistance, float angle_dest)
{
	//motorSet(SubMod(angle_dest,angle)*RAD2PULSES,DELTA_POS_STRAIGHT);
	//PIDPositionSet(0.5*SubMod(angle_dest,angle)*RAD2PULSES+0.5*signedDistance,DELTA_POS_STRAIGHT);
	pidCalc(&pidAngle, SubMod(angle_dest,angle)*RAD2PULSES, 10000);
	pidCalc(&pidDistance, signedDistance, 10000);
	motorSet(pidAngle.PIDResult+pidDistance.PIDResult,DELTA_POS_STRAIGHT);
}
void TurnRight(float x, float y, float angle)
{
	//giam toc do
	//quay
	if (fabsf(SubMod(angle,angle_dest))>PI/2)
		motorSet((int32_t)(-TURN_ANGLE*DEGREE2PULSES),DELTA_POS_TURN);
	else
		motorSet(0,DELTA_POS_TURN);
}
void TurnLeft(float x, float y, float angle)
{
	//giam toc do
	//quay
	if (fabsf(SubMod(angle,angle_dest))>PI/2)
		motorSet((int32_t)(TURN_ANGLE*DEGREE2PULSES),DELTA_POS_TURN);
	else
		motorSet(0,DELTA_POS_TURN);
}
void PathFollow(float x, float y, float angle)
{
	switch (eState)
	{
	case LINE_FOLLOWING:
	{
		if (eDir==UP)
		{
			if (distance(x,y,x_corner[2],y_corner[2],x_corner[3],y_corner[3])<TURN_DISTANCE*D_SCALE)
			{
				linePassed++;
				if (linePassed==totalLine)
					eState = PRE_STOP;
				else
				{
					eState = TURN_RIGHT;
					RFsend("right\r\n",7);
					angle_dest = -angle_orig;
				}
			}
			else
				FollowLine(x,y,angle,
					signedDistance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])-LINE_DISTANCE*linePassed,
					angle_dest);
		}
		else if (eDir==DOWN)
		{
			if (distance(x,y,x_corner[0],y_corner[0],x_corner[1],y_corner[1])<TURN_DISTANCE*D_SCALE)
			{
				linePassed++;
				if (linePassed==totalLine)
					eState = PRE_STOP;
				else
				{
					eState = TURN_LEFT;
					RFsend("left\r\n",6);
					angle_dest = angle_orig;
				}
			}
			else
				FollowLine(x,y,angle,
					-signedDistance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])+LINE_DISTANCE*linePassed,
					angle_dest);
		}
		break;
	}
	case TURN_LEFT:
	{
		TurnLeft(x,y,angle);
		if (fabsf(SubMod(angle,angle_dest))<PI*0.05)//9 degree
		{
			eState=LINE_FOLLOWING;
			pidReset(&pidAngle);
			pidReset(&pidDistance);
		}
		break;
	}
	case TURN_RIGHT:
	{
		TurnRight(x,y,angle);
		if (fabsf(SubMod(angle,angle_dest))<PI*0.05)
		{
			eState=LINE_FOLLOWING;
			pidReset(&pidAngle);
			pidReset(&pidDistance);
		}
		break;
	}
	}
	if (eState==PRE_STOP)
	{
		SSTOP_STOP;
		HBridgeDisable();
		RFsend("finish\r\n",8);
		eState = FINISH;
	}
}
