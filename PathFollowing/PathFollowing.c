/*
 * PathFollowing.c
 *
 *  Created on: Sep 11, 2015
 *      Author: Huan
 */
#include "include.h"

//#define FOLLOW_ALGORITHM_PID
//#define FOLLOW_ALGORITHM_P
#define FOLLOW_ALGORITHM_PURE_PERSUIT
//#define _DEBUG_PID

#define LINE_DISTANCE (float)5.0
#define TURN_DISTANCE (float)4.0
#define TURN_ANGLE (float)30.0
#define DELTA_POS_STRAIGHT 200
#define DELTA_POS_TURN 100

#define LOOK_AHEAD_D 5*D_SCALE
#define WHEEL_BASE 1.1*D_SCALE
#define D_OFFSET 0.4*D_SCALE//distance form back wheels to robot center

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

static eSTATE eState;
static eDIRECTION eDir;
static uint32_t linePassed=0;

static float x_corner[4];
static float y_corner[4];
static float angle_orig,m_orig,c_orig,delta_c;
static float angle_dest;
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
void calculateLookAheadPoint(float x, float y, float m, float c,float *xg,float *yg);
void PathFollowInit()
{
	// 3	2
	//
	//
	// 0	1

	x_corner[0] = 0.0;//1m
	y_corner[0] = 0.0;

	x_corner[1] = 128471;
	y_corner[1] = -224777;

	x_corner[2] = 360734;
	y_corner[2] = -96816;

	x_corner[3] = 236586;
	y_corner[3] = 130838;

	eState = LINE_FOLLOWING;
	eDir = UP;
#warning check x_corner[3]==x_corner[0]
	m_orig=(y_corner[3]-y_corner[0])/(x_corner[3]-x_corner[0]);
	c_orig=0.0;
	if (m_orig>=0)
		delta_c = -LINE_DISTANCE*D_SCALE*sqrtf(1+m_orig*m_orig);//next line is in the right of current line
	else
		delta_c = LINE_DISTANCE*D_SCALE*sqrtf(1+m_orig*m_orig);
	angle_orig = atan2f(y_corner[3]-y_corner[0],x_corner[3]-x_corner[0]);
	angle_dest = angle_orig;

	initPID(&pidDistance,0.8,0.002,0.05);
	initPID(&pidAngle,1.0,0.005,0.05);
	enablePID(&pidDistance);
	enablePID(&pidAngle);
	pidSet(&pidDistance, 0);
	pidSet(&pidAngle, 0);

	totalLine = (uint32_t)(distance(x_corner[1],y_corner[1],
			x_corner[0],y_corner[0],x_corner[3],y_corner[3])/(LINE_DISTANCE*D_SCALE))+1;
}
void calculateLookAheadPoint(float x, float y, float m, float c,float *xg,float *yg)
{
	float A,B,C,x1,x2;
	A=1+m*m;
	B=-2*x+2*m*c-2*m*y;
	C=x*x+(c-y)*(c-y)-LOOK_AHEAD_D*LOOK_AHEAD_D;
	x1=(-B+sqrtf(B*B-4*A*C))/(2*A);
	x2=(-B/A)-x1;
	if (eDir==UP)
	{
		if ((x2-x1)*(x_corner[3]-x_corner[0])>=0)
		{
			*xg = x2;
			*yg = m*x2+c;
		}
		else
		{
			*xg = x1;
			*yg = m*x1+c;
		}
	}
	else if (eDir==DOWN)
	{
		if ((x2-x1)*(x_corner[3]-x_corner[0])>=0)
		{
			*xg = x1;
			*yg = m*x1+c;
		}
		else
		{
			*xg = x2;
			*yg = m*x2+c;
		}
	}

}
float calculateSteering(float x, float y, float angle, float xg, float yg)
{
	float x_tmp;
	x_tmp = (xg-x)*sinf(angle)-(yg-y)*cosf(angle);//change to robot's coordinate
#if 1
	RFprint("steer %d\r\n",(int)x_tmp);
#endif
	return atanf(-2*WHEEL_BASE*x_tmp/(LOOK_AHEAD_D*LOOK_AHEAD_D));//-pi/2->pi/2
#if 0
	x_tmp = LOOK_AHEAD_D*LOOK_AHEAD_D/(2*x_tmp);
	return atanf(WHEEL_BASE/sqrtf(x_tmp*x_tmp+D_OFFSET*D_OFFSET));
#endif
}
#ifdef	FOLLOW_ALGORITHM_PID
void FollowLine(float x, float y, float angle, float signedDistance)
{
	//pidCalc(&pidAngle, SubMod(angle_dest,angle)*RAD2PULSES, 10000);
	pidCalc(&pidDistance, signedDistance, 15000);
	steeringSet(pidDistance.PIDResult,DELTA_POS_STRAIGHT);
#ifdef _DEBUG_PID
	//RFprint("pid %d,%d\r\n",(int)pidAngle.PIDResult,(int)pidDistance.PIDResult);
	RFprint("pid %d,%d\r\n",(int)signedDistance,(int)pidDistance.PIDResult);
#endif
}
#endif

#ifdef FOLLOW_ALGORITHM_PURE_PERSUIT
void FollowLine(float x, float y, float angle, float m,float c)
{
	float steeringAngle,xg,yg;
	calculateLookAheadPoint(x,y,m,c,&xg,&yg);
	steeringAngle = calculateSteering(x,y,angle,xg,yg);
#if 0
	RFprint("ahead %d,%d,%d\r\n",(int)xg,(int)yg,(int)(angle*RAD2DEGREE));
#endif
	steeringSet(steeringAngle*RAD2PULSES,DELTA_POS_STRAIGHT);
}
#endif

void TurnRight(float x, float y, float angle)
{
	//giam toc do
	//quay
//	if (fabsf(SubMod(angle,angle_dest))>PI/2)
		steeringSet((int32_t)(-TURN_ANGLE*DEGREE2PULSES),DELTA_POS_TURN);
		RFprint("right\r\n");
//	else
//		steeringSet(0,DELTA_POS_TURN);
}
void TurnLeft(float x, float y, float angle)
{
	//giam toc do
	//quay
//	if (fabsf(SubMod(angle,angle_dest))>PI/2)
		steeringSet((int32_t)(TURN_ANGLE*DEGREE2PULSES),DELTA_POS_TURN);
		RFprint("left\r\n");
//	else
//		steeringSet(0,DELTA_POS_TURN);
}

void PathFollow(float x, float y, float angle)
{
	static float angle_dest;
#if 0
		RFprint("%d,%d\r\n",(int)(fabsf(SubMod(angle,angle_dest))*RAD2DEGREE)
				,(int)(distance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])));
#endif
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
					angle_dest = SubMod(angle_orig,PI);
				}
			}
			else
			{
#ifdef FOLLOW_ALGORITHM_PID
				FollowLine(x,y,angle,
					-signedDistance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])+LINE_DISTANCE*D_SCALE*linePassed);
#endif

#ifdef FOLLOW_ALGORITHM_PURE_PERSUIT
				FollowLine(x,y,angle,m_orig,linePassed*delta_c);
#endif
			}
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
					angle_dest = angle_orig;
				}
			}
			else
			{
#ifdef FOLLOW_ALGORITHM_PID
				FollowLine(x,y,angle,
					signedDistance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])-LINE_DISTANCE*D_SCALE*linePassed);
#endif

#ifdef FOLLOW_ALGORITHM_PURE_PERSUIT
				FollowLine(x,y,angle,m_orig,linePassed*delta_c);
#endif
			}
		}
		break;
	}
	case TURN_LEFT:
	{
		TurnLeft(x,y,angle);
		if (fabsf(SubMod(angle,angle_dest))<PI*0.1)//18 degree
//			|| (distance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])
//					>(LINE_DISTANCE*linePassed-0.3)*D_SCALE))
		{
			eState=LINE_FOLLOWING;
			eDir=UP;

			pidReset(&pidAngle);
			pidReset(&pidDistance);
		}
		break;
	}
	case TURN_RIGHT:
	{

		TurnRight(x,y,angle);

		if (fabsf(SubMod(angle,angle_dest))<PI*0.1)
//			|| (distance(x,y,x_corner[0],y_corner[0],x_corner[3],y_corner[3])
//					>(LINE_DISTANCE*linePassed-1.0)*D_SCALE))
		{
			eState=LINE_FOLLOWING;
			eDir=DOWN;

			pidReset(&pidAngle);
			pidReset(&pidDistance);
		}
		break;
	}
	}
	if (eState==PRE_STOP)
	{
		throttleStop();
		SSTOP_STOP;
		//HBridgeDisable();
		RFprint("finish\r\n");
		eState = FINISH;
	}
}
